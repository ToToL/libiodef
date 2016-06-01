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

#ifndef _LIBIODEF_LIBIODEF_IO_H
#define _LIBIODEF_LIBIODEF_IO_H

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include "libiodef-inttypes.h"


typedef struct libiodef_io libiodef_io_t;

/*
 * Object creation / destruction functions.
 */
int libiodef_io_new(libiodef_io_t **ret);

void libiodef_io_destroy(libiodef_io_t *pio);

void libiodef_io_set_file_io(libiodef_io_t *pio, FILE *fd);

void libiodef_io_set_tls_io(libiodef_io_t *pio, void *tls);

void libiodef_io_set_sys_io(libiodef_io_t *pio, int fd);

int libiodef_io_set_buffer_io(libiodef_io_t *pio);


/*
 *
 */
void libiodef_io_set_fdptr(libiodef_io_t *pio, void *ptr);
void libiodef_io_set_write_callback(libiodef_io_t *pio, ssize_t (*write)(libiodef_io_t *io, const void *buf, size_t count));
void libiodef_io_set_read_callback(libiodef_io_t *pio, ssize_t (*read)(libiodef_io_t *io, void *buf, size_t count));
void libiodef_io_set_pending_callback(libiodef_io_t *pio, ssize_t (*pending)(libiodef_io_t *io));


/*
 * IO operations.
 */
int libiodef_io_close(libiodef_io_t *pio);

ssize_t libiodef_io_read(libiodef_io_t *pio, void *buf, size_t count);

ssize_t libiodef_io_read_wait(libiodef_io_t *pio, void *buf, size_t count);

ssize_t libiodef_io_read_delimited(libiodef_io_t *pio, unsigned char **buf);


ssize_t libiodef_io_write(libiodef_io_t *pio, const void *buf, size_t count);

ssize_t libiodef_io_write_delimited(libiodef_io_t *pio, const void *buf, uint16_t count);


ssize_t libiodef_io_forward(libiodef_io_t *dst, libiodef_io_t *src, size_t count);

int libiodef_io_get_fd(libiodef_io_t *pio);

void *libiodef_io_get_fdptr(libiodef_io_t *pio);

ssize_t libiodef_io_pending(libiodef_io_t *pio);

libiodef_bool_t libiodef_io_is_error_fatal(libiodef_io_t *pio, int error);

#ifdef __cplusplus
  }
#endif

#endif /* _LIBIODEF_LIBIODEF_IO_H */
