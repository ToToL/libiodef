/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodef-ids.com>
*
* This file is part of the LibIodef library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

#include "config.h"
#include "libmissing.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/ioctl.h>

#include "libiodef-inttypes.h"

#ifdef HAVE_SYS_FILIO_H
# include <sys/filio.h>
#endif


#include "libiodef-log.h"
#include "libiodef-io.h"
#include "common.h"


#define LIBIODEF_ERROR_SOURCE_DEFAULT LIBIODEF_ERROR_SOURCE_IO
#include "libiodef-error.h"


#define CHUNK_SIZE 1024

#ifndef MIN
# define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

struct libiodef_io {

        int fd;
        void *fd_ptr;

        size_t size;
        size_t rindex;

        int (*close)(libiodef_io_t *pio);
        ssize_t (*read)(libiodef_io_t *pio, void *buf, size_t count);
        ssize_t (*write)(libiodef_io_t *pio, const void *buf, size_t count);
        ssize_t (*pending)(libiodef_io_t *pio);
};



/*
 * Buffer IO functions.
 */
static ssize_t buffer_read(libiodef_io_t *pio, void *buf, size_t count)
{
        if ( pio->size - pio->rindex == 0 )
                return 0;

        count = MIN(count, pio->size - pio->rindex);

        memcpy(buf, (unsigned char *) pio->fd_ptr + pio->rindex, count);
        pio->rindex += count;

        return count;
}



static ssize_t buffer_write(libiodef_io_t *pio, const void *buf, size_t count)
{
        unsigned char *new;

        if ( pio->size + count <= pio->size )
                return -1;

        new = _libiodef_realloc(pio->fd_ptr, pio->size + count);
        if ( ! new )
                return libiodef_error_from_errno(errno);

        memcpy(new + pio->size, buf, count);

        pio->fd_ptr = new;
        pio->size += count;

        return count;
}



static int buffer_close(libiodef_io_t *pio)
{
        if ( pio->fd_ptr ) {
                free(pio->fd_ptr);
                pio->fd_ptr = NULL;
                pio->size = pio->rindex = 0;
        }

        return 0;
}



static ssize_t buffer_pending(libiodef_io_t *pio)
{
        return pio->size - pio->rindex;
}




/*
 * System IO functions.
 */
static ssize_t sys_read(libiodef_io_t *pio, void *buf, size_t count)
{
        ssize_t ret;

        do {
                ret = read(pio->fd, buf, count);
        } while ( ret < 0 && errno == EINTR );

        if ( ret == 0 )
                return libiodef_error(LIBIODEF_ERROR_EOF);

        if ( ret < 0 ) {
#ifdef ECONNRESET
                if ( errno == ECONNRESET )
                        return libiodef_error(LIBIODEF_ERROR_EOF);
#endif
                return libiodef_error_from_errno(errno);
        }

        return ret;
}



static ssize_t sys_write(libiodef_io_t *pio, const void *buf, size_t count)
{
        ssize_t ret;

        do {
                ret = write(pio->fd, buf, count);
        } while ( ret < 0 && errno == EINTR );

        if ( ret < 0 )
                return libiodef_error_from_errno(errno);

        return ret;
}



static int sys_close(libiodef_io_t *pio)
{
        int ret;

        do {
                ret = close(pio->fd);
        } while ( ret < 0 && errno == EINTR );

        return (ret >= 0) ? ret : libiodef_error_from_errno(errno);
}



static ssize_t sys_pending(libiodef_io_t *pio)
{
        long ret = 0;

        if ( ioctl(pio->fd, FIONREAD, &ret) < 0 )
                return libiodef_error_from_errno(errno);

        return ret;
}




/*
 * Buffered IO functions.
 */
static ssize_t file_read(libiodef_io_t *pio, void *buf, size_t count)
{
        FILE *fd;
        size_t ret;

        /*
         * ferror / clearerror can be macro that might dereference fd_ptr.
         */
        fd = pio->fd_ptr;
        libiodef_return_val_if_fail(fd, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        ret = fread(buf, count, 1, fd);
        if ( ret <= 0 ) {
                ret = ferror(fd) ? libiodef_error_from_errno(errno) : libiodef_error(LIBIODEF_ERROR_EOF);
                clearerr(fd);
                return ret;
        }

        /*
         * fread return the number of *item* read.
         */
        return count;
}



static ssize_t file_write(libiodef_io_t *pio, const void *buf, size_t count)
{
        size_t ret;

        libiodef_return_val_if_fail(pio->fd_ptr, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        ret = fwrite(buf, count, 1, pio->fd_ptr);
        if ( ret <= 0 )
                return ret;

        /*
         * fwrite return the number of *item* written.
         */
        return count;
}



static int file_close(libiodef_io_t *pio)
{
        libiodef_return_val_if_fail(pio->fd_ptr, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        return fclose(pio->fd_ptr);
}



static ssize_t file_pending(libiodef_io_t *pio)
{
#ifdef ENOTSUP
        return libiodef_error_from_errno(ENOTSUP);
#else
        return libiodef_error(LIBIODEF_ERROR_GENERIC);
#endif
}




/*
 * Forward data from one fd to another using copy.
 */
static ssize_t copy_forward(libiodef_io_t *dst, libiodef_io_t *src, size_t count)
{
        int ret;
        size_t scount;
        unsigned char buf[8192];

        scount = count;

        while ( count ) {

                ret = (count < sizeof(buf)) ? count : sizeof(buf);

                ret = libiodef_io_read(src, buf, ret);
                if ( ret <= 0 )
                        return ret;

                count -= ret;

                ret = libiodef_io_write(dst, buf, ret);
                if ( ret < 0 )
                        return ret;
        }

        return scount;
}





/**
 * libiodef_io_forward:
 * @src: Pointer to a #libiodef_io_t object.
 * @dst: Pointer to a #libiodef_io_t object.
 * @count: Number of byte to forward from @src to @dst.
 *
 * libiodef_io_forward() attempts to transfer up to @count bytes from
 * the file descriptor identified by @src into the file descriptor
 * identified by @dst.
 *
 * Returns: If the transfer was successful, the number of bytes written
 * to @dst is returned.  On error, -1 is returned, and errno is set appropriately.
 */
ssize_t libiodef_io_forward(libiodef_io_t *dst, libiodef_io_t *src, size_t count)
{
        libiodef_return_val_if_fail(dst, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(src, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        return copy_forward(dst, src, count);
}




/**
 * libiodef_io_read:
 * @pio: Pointer to a #libiodef_io_t object.
 * @buf: Pointer to the buffer to store data into.
 * @count: Number of bytes to read.
 *
 * libiodef_io_read() attempts to read up to @count bytes from the
 * file descriptor identified by @pio into the buffer starting at @buf.
 *
 * If @count is zero, libiodef_io_read() returns zero and has no other
 * results. If @count is greater than SSIZE_MAX, the result is unspecified.
 *
 * The case where the read function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes read is returned (zero
 * indicates end of file). It is not an error if this number is smaller
 * than the number of bytes requested; this may happen for example because
 * fewer bytes are actually available right now or because read() was
 * interrupted by a signal.
 *
 * On error, a negative value is returned. In this case it is left unspecified
 * whether the file position (if any) changes.
 */
ssize_t libiodef_io_read(libiodef_io_t *pio, void *buf, size_t count)
{
        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(pio->read, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(buf, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        return pio->read(pio, buf, count);
}




/**
 * libiodef_io_read_wait:
 * @pio: Pointer to a #libiodef_io_t object.
 * @buf: Pointer to the buffer to store data into.
 * @count: Number of bytes to read.
 *
 * libiodef_io_read_wait() attempts to read up to @count bytes from the
 * file descriptor identified by @pio into the buffer starting at @buf.
 *
 * If @count is zero, libiodef_io_read() returns zero and has no other
 * results. If @count is greater than SSIZE_MAX, the result is unspecified.
 *
 * The case where the read function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * libiodef_io_read_wait() always return the number of bytes requested.
 * Be carefull that this function is blocking.
 *
 * Returns: On success, the number of bytes read is returned (zero
 * indicates end of file).
 *
 * On error, -1 is returned, and errno is set appropriately. In this
 * case it is left unspecified whether the file position (if any) changes.
 */
ssize_t libiodef_io_read_wait(libiodef_io_t *pio, void *buf, size_t count)
{
        ssize_t ret;
        size_t n = 0;
        struct pollfd pfd;
        unsigned char *in = buf;

        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(buf, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        pfd.fd = libiodef_io_get_fd(pio);
        pfd.events = POLLIN;

        do {
                ret = poll(&pfd, 1, -1);
                if ( ret < 0 )
                        return libiodef_error_from_errno(errno);

                if ( ! (pfd.revents & POLLIN) )
                        return libiodef_error_verbose(LIBIODEF_ERROR_GENERIC, "expected POLLIN event");

                ret = libiodef_io_read(pio, &in[n], count - n);
                if ( ret < 0 )
                        return ret;

                n += (size_t) ret;

        } while ( n != count );

        return (ssize_t) n;
}



/**
 * libiodef_io_read_delimited:
 * @pio: Pointer to a #libiodef_io_t object.
 * @buf: Pointer to the address of a buffer to store address of data into.
 *
 * libiodef_io_read_delimited() read message written by libiodef_write_delimited().
 * Theses messages are sents along with the len of the message.
 *
 * Uppon return the @buf argument is updated to point on a newly allocated
 * buffer containing the data read. The @count argument is set to the number of
 * bytes the message was containing.
 *
 * The case where the read function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes read is returned (zero
 * indicates end of file).
 *
 * On error, -1 is returned, and errno is set appropriately. In this
 * case it is left unspecified whether the file position (if any) changes.
 */
ssize_t libiodef_io_read_delimited(libiodef_io_t *pio, unsigned char **buf)
{
        ssize_t ret;
        size_t count;
        uint16_t msglen;

        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        ret = libiodef_io_read_wait(pio, &msglen, sizeof(msglen));
        if ( ret <= 0 )
                return ret;

        count = ntohs(msglen);

        *buf = malloc(count);
        if ( ! *buf )
                return libiodef_error_from_errno(errno);

        ret = libiodef_io_read_wait(pio, *buf, count);
        if ( ret < 0 )
                return ret;

        return count;
}




/**
 * libiodef_io_write:
 * @pio: Pointer to a #libiodef_io_t object.
 * @buf: Pointer to the buffer to write data from.
 * @count: Number of bytes to write.
 *
 * libiodef_io_write() writes up to @count bytes to the file descriptor
 * identified by @pio from the buffer starting at @buf. POSIX requires
 * that a read() which can be proved to occur after a write() has returned
 * returns the new data. Note that not all file systems are POSIX conforming.
 *
 * The case where the write() function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes written are returned (zero
 * indicates nothing was written). On error, -1 is returned, and errno
 * is set appropriately. If @count is zero and the file descriptor refers
 * to a regular file, 0 will be returned without causing any other effect.
 * For a special file, the results are not portable.
 */
ssize_t libiodef_io_write(libiodef_io_t *pio, const void *buf, size_t count)
{
        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(pio->write, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(buf, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        return pio->write(pio, buf, count);
}



/**
 * libiodef_io_write_delimited:
 * @pio: Pointer to a #libiodef_io_t object.
 * @buf: Pointer to the buffer to write data from.
 * @count: Number of bytes to write.
 *
 * libiodef_io_write_delimited() writes up to @count bytes to the file descriptor
 * identified by @pio from the buffer starting at @buf. POSIX requires
 * that a read() which can be proved to occur after a write() has returned
 * returns the new data. Note that not all file systems are POSIX conforming.
 *
 * libiodef_io_write_delimited() also write the len of the data to be sent.
 * which allow libiodef_io_read_delimited() to safely know if it got all the
 * data a given write contain.
 *
 * The case where the write() function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * Returns: On success, the number of bytes written are returned (zero
 * indicates nothing was written). On error, -1 is returned, and errno
 * is set appropriately.
 */
ssize_t libiodef_io_write_delimited(libiodef_io_t *pio, const void *buf, uint16_t count)
{
        int ret;
        uint16_t nlen;

        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(buf, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        nlen = htons(count);

        ret = libiodef_io_write(pio, &nlen, sizeof(nlen));
        if ( ret <= 0 )
                return ret;

        ret = libiodef_io_write(pio, buf, count);
        if ( ret <= 0 )
                return ret;

        return count;
}




/**
 * libiodef_io_close:
 * @pio: Pointer to a #libiodef_io_t object.
 *
 * libiodef_io_close() closes the file descriptor indentified by @pio,
 *
 * The case where the close() function would be interrupted by a signal is
 * handled internally. So you don't have to check for EINTR.
 *
 * However, and especially when the underlaying layer is TLS, libiodef_io_close()
 * might return error. If this happen, you should continue calling the function
 * until it return zero.
 *
 * Returns: zero on success, or -1 if an error occurred.
 */
int libiodef_io_close(libiodef_io_t *pio)
{
        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        libiodef_return_val_if_fail(pio->close, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        return pio->close(pio);
}




/**
 * libiodef_io_new:
 * @ret: Pointer where to store the created #libiodef_io_t object.
 *
 * Create a new libiodef IO object.
 *
 * Returns: 0 on success, or a negative value if an error occur.
 */
int libiodef_io_new(libiodef_io_t **ret)
{
        *ret = calloc(1, sizeof(**ret));
        if ( ! *ret )
                return libiodef_error_from_errno(errno);

        return 0;
}



/**
 * libiodef_io_set_file_io:
 * @pio: A pointer on the #libiodef_io_t object.
 * @fd: File descriptor identifying a file.
 *
 * Setup the @pio object to work with file I/O function.
 * The @pio object is then associated with @fd.
 */
void libiodef_io_set_file_io(libiodef_io_t *pio, FILE *fdptr)
{
        libiodef_return_if_fail(pio);
        libiodef_return_if_fail(fdptr);

        pio->fd = fileno(fdptr);
        pio->fd_ptr = fdptr;
        pio->read = file_read;
        pio->write = file_write;
        pio->close = file_close;
        pio->pending = file_pending;
}





/**
 * libiodef_io_set_sys_io:
 * @pio: A pointer on the #libiodef_io_t object.
 * @fd: A file descriptor.
 *
 * Setup the @pio object to work with system based I/O function.
 * The @pio object is then associated with @fd.
 */
void libiodef_io_set_sys_io(libiodef_io_t *pio, int fd)
{
        libiodef_return_if_fail(pio);

        pio->fd = fd;
        pio->fd_ptr = NULL;
        pio->read = sys_read;
        pio->write = sys_write;
        pio->close = sys_close;
        pio->pending = sys_pending;
}



int libiodef_io_set_buffer_io(libiodef_io_t *pio)
{
        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));

        pio->fd_ptr = NULL;
        pio->size = pio->rindex = 0;

        pio->read = buffer_read;
        pio->write = buffer_write;
        pio->close = buffer_close;
        pio->pending = buffer_pending;

        return 0;
}



/**
 * libiodef_io_get_fd:
 * @pio: A pointer on a #libiodef_io_t object.
 *
 * Returns: The FD associated with this object.
 */
int libiodef_io_get_fd(libiodef_io_t *pio)
{
        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        return pio->fd;
}



/**
 * libiodef_io_get_fdptr:
 * @pio: A pointer on a #libiodef_io_t object.
 *
 * Returns: Pointer associated with this object (file, tls, buffer, or NULL).
 */
void *libiodef_io_get_fdptr(libiodef_io_t *pio)
{
        libiodef_return_val_if_fail(pio, NULL);
        return pio->fd_ptr;
}



/**
 * libiodef_io_destroy:
 * @pio: Pointer to a #libiodef_io_t object.
 *
 * Destroy the @pio object.
 */
void libiodef_io_destroy(libiodef_io_t *pio)
{
        libiodef_return_if_fail(pio);
        free(pio);
}




/**
 * libiodef_io_pending:
 * @pio: Pointer to a #libiodef_io_t object.
 *
 * libiodef_io_pending return the number of bytes waiting to
 * be read on an TLS or socket fd.
 *
 * Returns: Number of byte waiting to be read on @pio, or -1
 * if @pio is not of type TLS or socket.
 */
ssize_t libiodef_io_pending(libiodef_io_t *pio)
{
        libiodef_return_val_if_fail(pio, libiodef_error(LIBIODEF_ERROR_ASSERTION));
        return pio->pending(pio);
}




/**
 * libiodef_io_is_error_fatal:
 * @pio: Pointer to a #libiodef_io_t object.
 * @error: Error returned by one of the #libiodef_io_t function.
 *
 * Check whether the returned error is fatal, or not.
 *
 * Returns: TRUE if error is fatal, FALSE if it is not.
 */
libiodef_bool_t libiodef_io_is_error_fatal(libiodef_io_t *pio, int error)
{
        libiodef_error_code_t code;

        libiodef_return_val_if_fail(pio, FALSE);

        if ( ! error )
                return FALSE;

        code = libiodef_error_get_code(error);
        if ( code == LIBIODEF_ERROR_EAGAIN || code == LIBIODEF_ERROR_EINTR || code == LIBIODEF_ERROR_TLS_WARNING_ALERT )
                return FALSE;

        return TRUE;
}



/**
 * libiodef_io_set_write_callback:
 * @pio: Pointer to a #libiodef_io_t object.
 * @write: Callback function to be called on libiodef_io_write().
 *
 * Set an user defined write callback function to be called on
 * libiodef_io_write().
 */
void libiodef_io_set_write_callback(libiodef_io_t *pio, ssize_t (*write)(libiodef_io_t *io, const void *buf, size_t count))
{
        pio->write = write;
}


/**
 * libiodef_io_set_read_callback:
 * @pio: Pointer to a #libiodef_io_t object.
 * @read: Callback function to be called on libiodef_io_read().
 *
 * Set an user defined read callback function to be called on
 * libiodef_io_read().
 */
void libiodef_io_set_read_callback(libiodef_io_t *pio, ssize_t (*read)(libiodef_io_t *io, void *buf, size_t count))
{
        pio->read = read;
}



/**
 * libiodef_io_set_pending_callback:
 * @pio: Pointer to a #libiodef_io_t object.
 * @pending: Callback function to be called on libiodef_io_pending().
 *
 * Set an user defined read callback function to be called on
 * libiodef_io_pending().
 */
void libiodef_io_set_pending_callback(libiodef_io_t *pio, ssize_t (*pending)(libiodef_io_t *io))
{
        pio->pending = pending;
}



/**
 * libiodef_io_set_fdptr:
 * @pio: Pointer to a #libiodef_io_t object.
 * @ptr: Pointer to user defined data.
 *
 * Set an user defined pointer that might be retrieved using
 * libiodef_io_get_fdptr().
 */
void libiodef_io_set_fdptr(libiodef_io_t *pio, void *ptr)
{
        pio->fd_ptr = ptr;
}
