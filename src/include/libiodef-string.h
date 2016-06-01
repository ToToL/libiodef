/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEF_LIBIODEF_STRING_H
#define _LIBIODEF_LIBIODEF_STRING_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdarg.h>

#include "libiodef-list.h"
#include "libiodef-macros.h"
#include "libiodef-inttypes.h"
#include "libiodef-linked-object.h"

#ifdef __cplusplus
 extern "C" {
#endif


typedef struct libiodef_string libiodef_string_t;


int libiodef_string_new(libiodef_string_t **string);

int libiodef_string_new_nodup(libiodef_string_t **string, char *str);

int libiodef_string_new_ref(libiodef_string_t **string, const char *str);

int libiodef_string_new_dup(libiodef_string_t **string, const char *str);

int libiodef_string_new_dup_fast(libiodef_string_t **string, const char *str, size_t len);

void libiodef_string_destroy(libiodef_string_t *string);

void libiodef_string_destroy_internal(libiodef_string_t *string);

int libiodef_string_new_nodup_fast(libiodef_string_t **string, char *str, size_t len);

int libiodef_string_new_ref_fast(libiodef_string_t **string, const char *str, size_t len);

int libiodef_string_set_dup_fast(libiodef_string_t *string, const char *buf, size_t len);

int libiodef_string_set_dup(libiodef_string_t *string, const char *buf);

int libiodef_string_set_nodup_fast(libiodef_string_t *string, char *buf, size_t len);

int libiodef_string_set_nodup(libiodef_string_t *string, char *buf);

int libiodef_string_set_ref_fast(libiodef_string_t *string, const char *buf, size_t len);

int libiodef_string_set_ref(libiodef_string_t *string, const char *buf);

int libiodef_string_copy_ref(const libiodef_string_t *src, libiodef_string_t *dst);

int libiodef_string_copy_dup(const libiodef_string_t *src, libiodef_string_t *dst);

libiodef_string_t *libiodef_string_ref(libiodef_string_t *string);

int libiodef_string_clone(const libiodef_string_t *src, libiodef_string_t **dst);

size_t libiodef_string_get_len(const libiodef_string_t *string);

const char *libiodef_string_get_string_or_default(const libiodef_string_t *string, const char *def);

const char *libiodef_string_get_string(const libiodef_string_t *string);

int libiodef_string_get_string_released(libiodef_string_t *string, char **outptr);

libiodef_bool_t libiodef_string_is_empty(const libiodef_string_t *string);

int libiodef_string_truncate(libiodef_string_t *string, size_t len);

void libiodef_string_clear(libiodef_string_t *string);

/*
 * string operation
 */
int libiodef_string_cat(libiodef_string_t *dst, const char *str);
int libiodef_string_ncat(libiodef_string_t *dst, const char *str, size_t len);

int libiodef_string_sprintf(libiodef_string_t *string, const char *fmt, ...)
                           LIBIODEF_FMT_PRINTF(2, 3);

int libiodef_string_vprintf(libiodef_string_t *string, const char *fmt, va_list ap)
                           LIBIODEF_FMT_PRINTF(2, 0);

int libiodef_string_compare(const libiodef_string_t *str1, const libiodef_string_t *str2);

#define libiodef_string_set_constant(string, str)                         \
        libiodef_string_set_ref_fast((string), (str), strlen((str)))

#define libiodef_string_new_constant(string, str)                         \
        libiodef_string_new_ref_fast((string), (str), strlen((str)))

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEF_LIBIODEF_STRING_H */
