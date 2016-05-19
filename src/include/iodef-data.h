/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
*
* This file is part of the Prelude library.
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

#ifndef _LIBPRELUDE_IODEF_DATA_H
#define _LIBPRELUDE_IODEF_DATA_H

#include "prelude-list.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IODEF_DATA_TYPE_UNKNOWN      = 0,
        IODEF_DATA_TYPE_CHAR         = 1,
        IODEF_DATA_TYPE_BYTE         = 2,
        IODEF_DATA_TYPE_UINT32       = 3,
        IODEF_DATA_TYPE_UINT64       = 4,
        IODEF_DATA_TYPE_INT          = 4,
        IODEF_DATA_TYPE_FLOAT        = 5,
        IODEF_DATA_TYPE_CHAR_STRING  = 6,
        IODEF_DATA_TYPE_BYTE_STRING  = 7,
        IODEF_DATA_TYPE_TIME         = 8
} iodef_data_type_t;


typedef struct {
        int refcount;

        int flags;
        iodef_data_type_t type;
        size_t len;

        union {
                char char_data;
                uint8_t byte_data;
                int64_t int_data;
                float float_data;
                void *rw_data;
                const void *ro_data;
        } data;

        prelude_list_t list;
} iodef_data_t;



int iodef_data_new(iodef_data_t **data);

iodef_data_t *iodef_data_ref(iodef_data_t *data);


int iodef_data_new_char(iodef_data_t **data, char c);
int iodef_data_new_byte(iodef_data_t **data, uint8_t i);
int iodef_data_new_uint32(iodef_data_t **data, uint32_t i) PRELUDE_DEPRECATED;
int iodef_data_new_uint64(iodef_data_t **data, uint64_t i) PRELUDE_DEPRECATED;
int iodef_data_new_int(iodef_data_t **data, int64_t i);
int iodef_data_new_float(iodef_data_t **data, float f);
int iodef_data_new_time(iodef_data_t **data, iodef_time_t *time);

void iodef_data_set_char(iodef_data_t *data, char c);
void iodef_data_set_byte(iodef_data_t *data, uint8_t i);
void iodef_data_set_uint32(iodef_data_t *data, uint32_t i) PRELUDE_DEPRECATED;
void iodef_data_set_uint64(iodef_data_t *data, uint64_t i) PRELUDE_DEPRECATED;
void iodef_data_set_int(iodef_data_t *data, int64_t i);
void iodef_data_set_float(iodef_data_t *data, float f);
void iodef_data_set_time(iodef_data_t *data, iodef_time_t *time);

int iodef_data_set_ptr_dup_fast(iodef_data_t *data, iodef_data_type_t type, const void *ptr, size_t len);
int iodef_data_set_ptr_ref_fast(iodef_data_t *data, iodef_data_type_t type, const void *ptr, size_t len);
int iodef_data_set_ptr_nodup_fast(iodef_data_t *data, iodef_data_type_t type, void *ptr, size_t len);

int iodef_data_new_ptr_dup_fast(iodef_data_t **data, iodef_data_type_t type, const void *ptr, size_t len);
int iodef_data_new_ptr_ref_fast(iodef_data_t **data, iodef_data_type_t type, const void *ptr, size_t len);
int iodef_data_new_ptr_nodup_fast(iodef_data_t **data, iodef_data_type_t type, void *ptr, size_t len);


/*
 * String functions
 */
int iodef_data_set_char_string_dup_fast(iodef_data_t *data, const char *str, size_t len);
int iodef_data_new_char_string_dup_fast(iodef_data_t **data, const char *str, size_t len);
int iodef_data_new_char_string_ref_fast(iodef_data_t **data, const char *ptr, size_t len);
int iodef_data_new_char_string_nodup_fast(iodef_data_t **data, char *ptr, size_t len);
int iodef_data_set_char_string_ref_fast(iodef_data_t *data, const char *ptr, size_t len);
int iodef_data_set_char_string_nodup_fast(iodef_data_t *data, char *ptr, size_t len);
int iodef_data_new_char_string_ref(iodef_data_t **data, const char *ptr);
int iodef_data_new_char_string_dup(iodef_data_t **data, const char *ptr);
int iodef_data_new_char_string_nodup(iodef_data_t **data, char *ptr);
int iodef_data_set_char_string_ref(iodef_data_t *data, const char *ptr);
int iodef_data_set_char_string_dup(iodef_data_t *data, const char *ptr);
int iodef_data_set_char_string_nodup(iodef_data_t *data, char *ptr);

#define iodef_data_set_char_string_constant(string, str)                \
        iodef_data_set_char_string_ref_fast((string), (str), sizeof((str)) - 1)


/*
 * Byte functions
 */
int iodef_data_new_byte_string_ref(iodef_data_t **data, const unsigned char *ptr, size_t len);
int iodef_data_new_byte_string_dup(iodef_data_t **data, const unsigned char *ptr, size_t len);
int iodef_data_new_byte_string_nodup(iodef_data_t **data, unsigned char *ptr, size_t len);
int iodef_data_set_byte_string_ref(iodef_data_t *data, const unsigned char *ptr, size_t len);
int iodef_data_set_byte_string_dup(iodef_data_t *data, const unsigned char *ptr, size_t len);
int iodef_data_set_byte_string_nodup(iodef_data_t *data, unsigned char *ptr, size_t len);



/*
 *
 */

void iodef_data_destroy(iodef_data_t *data);

int iodef_data_copy_ref(const iodef_data_t *src, iodef_data_t *dst);

int iodef_data_copy_dup(const iodef_data_t *src, iodef_data_t *dst);

int iodef_data_clone(const iodef_data_t *src, iodef_data_t **dst);

iodef_data_type_t iodef_data_get_type(const iodef_data_t *data);

size_t iodef_data_get_len(const iodef_data_t *data);

const void *iodef_data_get_data(const iodef_data_t *data);

char iodef_data_get_char(const iodef_data_t *data);

uint8_t iodef_data_get_byte(const iodef_data_t *data);

uint32_t iodef_data_get_uint32(const iodef_data_t *data) PRELUDE_DEPRECATED;

uint64_t iodef_data_get_uint64(const iodef_data_t *data) PRELUDE_DEPRECATED;

int64_t iodef_data_get_int(const iodef_data_t *data);

float iodef_data_get_float(const iodef_data_t *data);

const char *iodef_data_get_char_string(const iodef_data_t *data);

const unsigned char *iodef_data_get_byte_string(const iodef_data_t *data);

prelude_bool_t iodef_data_is_empty(const iodef_data_t *data);

int iodef_data_to_string(const iodef_data_t *data, prelude_string_t *out);

void iodef_data_destroy_internal(iodef_data_t *data);

int iodef_data_compare(const iodef_data_t *data1, const iodef_data_t *data2);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBPRELUDE_IODEF_DATA_H */
