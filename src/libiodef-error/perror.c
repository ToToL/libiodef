#include <stdio.h>
#include <stdarg.h>

#include "libiodef-log.h"
#include "libiodef-error.h"


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
