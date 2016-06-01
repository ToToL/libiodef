#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gettext.h>
#include <errno.h>

#include "libiodef-log.h"
#include "libiodef-error.h"

#include "code-to-errno.h"
#include "code-from-errno.h"


#define LIBIODEF_ERROR_CODE_DIM     65536
#define LIBIODEF_ERROR_SOURCE_DIM     256

#define LIBIODEF_ERROR_SOURCE_SHIFT    23
#define LIBIODEF_ERROR_VERBOSE_SHIFT   22

#define LIBIODEF_ERROR_CODE_MASK       (LIBIODEF_ERROR_CODE_DIM - 1)
#define LIBIODEF_ERROR_SOURCE_MASK     (LIBIODEF_ERROR_SOURCE_DIM - 1)
#define LIBIODEF_ERROR_VERBOSE_MASK    (1)


/**
 * libiodef_error_make:
 * @source: Error source.
 * @code: Error code.
 *
 * Create a new #libiodef_error_t error using @source and @code.
 *
 * Returns: The created #libiodef_error_t integer.
 */
libiodef_error_t libiodef_error_make(libiodef_error_source_t source, libiodef_error_code_t code)
{
        return (code == LIBIODEF_ERROR_NO_ERROR) ? code : -((source << LIBIODEF_ERROR_SOURCE_SHIFT) | code);
}


/**
 * libiodef_error_make_from_errno:
 * @source: Error source.
 * @err: errno value.
 *
 * Create a new #libiodef_error_t error using @source and @errno.
 *
 * Returns: The created #libiodef_error_t integer.
 */
libiodef_error_t libiodef_error_make_from_errno(libiodef_error_source_t source, int err)
{
        libiodef_error_code_t code = libiodef_error_code_from_errno(err);
        return libiodef_error_make(source, code);
}



/**
 * libiodef_error_verbose_make_v:
 * @source: Error source.
 * @code: Error code.
 * @fmt: Format string.
 * @ap: Argument list.
 *
 * Create a new error using @source and @code, using the detailed error message
 * specified within @fmt.
 *
 * Returns: The created #libiodef_error_t integer.
 */
libiodef_error_t libiodef_error_verbose_make_v(libiodef_error_source_t source,
                                             libiodef_error_code_t code, const char *fmt, va_list ap)
{
        int ret;

        ret = libiodef_error_make(source, code);
        ret = -ret;
        ret |= (1 << LIBIODEF_ERROR_VERBOSE_SHIFT);

        return -ret;
}



/**
 * libiodef_error_verbose_make:
 * @source: Error source.
 * @code: Error code.
 * @fmt: Format string.
 * @...: Argument list.
 *
 * Create a new error using @source and @code, using the detailed error message
 * specified within @fmt.
 *
 * Returns: The created #libiodef_error_t integer.
 */
libiodef_error_t libiodef_error_verbose_make(libiodef_error_source_t source,
                                           libiodef_error_code_t code, const char *fmt, ...)
{
        int ret;
        va_list ap;

        va_start(ap, fmt);
        ret = libiodef_error_verbose_make_v(source, code, fmt, ap);
        va_end(ap);

        return ret;
}


/**
 * libiodef_error_get_code:
 * @error: A #libiodef_error_t return value.
 *
 * Returns: the #libiodef_code_t code contained within the @libiodef_error_t integer.
 */
libiodef_error_code_t libiodef_error_get_code(libiodef_error_t error)
{
        error = -error;
        return (libiodef_error_code_t) (error & LIBIODEF_ERROR_CODE_MASK);
}


/**
 * libiodef_error_get_source:
 * @error: A #libiodef_error_t return value.
 *
 * Returns: the #libiodef_source_t source contained within the @libiodef_error_t integer.
 */
libiodef_error_source_t libiodef_error_get_source(libiodef_error_t error)
{
        error = -error;
        return (libiodef_error_source_t) ((error >> LIBIODEF_ERROR_SOURCE_SHIFT) & LIBIODEF_ERROR_SOURCE_MASK);
}


/**
 * libiodef_error_is_verbose:
 * @error: A #libiodef_error_t return value.
 *
 * Returns: #LIBIODEF_BOOL_TRUE if there is a detailed message for this error, #LIBIODEF_BOOL_FALSE otherwise.
 */
libiodef_bool_t libiodef_error_is_verbose(libiodef_error_t error)
{
        error = -error;
        return ((error >> LIBIODEF_ERROR_VERBOSE_SHIFT) & LIBIODEF_ERROR_VERBOSE_MASK) ? LIBIODEF_BOOL_TRUE : LIBIODEF_BOOL_FALSE;
}


/**
 * libiodef_error_code_from_errno:
 * @err: errno value.
 *
 * Returns: the #libiodef_error_code_t value corresponding to @err.
 */
libiodef_error_code_t libiodef_error_code_from_errno(int err)
{
        int idx;

        if ( ! err )
                return LIBIODEF_ERROR_NO_ERROR;

        idx = errno_to_idx(err);
        if ( idx < 0 )
                return LIBIODEF_ERROR_UNKNOWN_ERRNO;

        return LIBIODEF_ERROR_SYSTEM_ERROR | err_code_from_index[idx];
}


/**
 * libiodef_error_code_to_errno:
 * @code: Error code.
 *
 * Returns: the errno value corresponding to @code.
 */
int libiodef_error_code_to_errno(libiodef_error_code_t code)
{
        if ( ! (code & LIBIODEF_ERROR_SYSTEM_ERROR) )
                return 0;

        code &= ~LIBIODEF_ERROR_SYSTEM_ERROR;

        if ( code < sizeof(err_code_to_errno) / sizeof(err_code_to_errno[0]) )
                return err_code_to_errno[code];
        else
                return 0;
}



/**
 * libiodef_perror:
 * @error: A #libiodef_error_t return value.
 * @fmt: Format string.
 * @...: Argument list.
 *
 * Print the error to stderr, or to syslog() in case stderr is unavailable.
 */
void libiodef_perror(libiodef_error_t error, const char *fmt, ...)
{
        va_list ap;
        char buf[1024];

        va_start(ap, fmt);
        vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);

        if ( libiodef_error_get_source(error) )
                libiodef_log(LIBIODEF_LOG_WARN, "%s: %s: %s.\n", libiodef_strsource(error), buf, libiodef_strerror(error));
        else
                libiodef_log(LIBIODEF_LOG_WARN, "%s: %s.\n", buf, libiodef_strerror(error));
}
