/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
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

#ifndef _IODEF_VALUE_TYPE_H
#define _IODEF_VALUE_TYPE_H

#include "iodef-time.h"
#include "iodef-data.h"
#include "libiodef-string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IODEF_VALUE_TYPE_ERROR   =  -1,
        IODEF_VALUE_TYPE_UNKNOWN =   0,
        IODEF_VALUE_TYPE_INT8    =   1,
        IODEF_VALUE_TYPE_UINT8   =   2,
        IODEF_VALUE_TYPE_INT16   =   3,
        IODEF_VALUE_TYPE_UINT16  =   4,
        IODEF_VALUE_TYPE_INT32   =   5,
        IODEF_VALUE_TYPE_UINT32  =   6,
        IODEF_VALUE_TYPE_INT64   =   7,
        IODEF_VALUE_TYPE_UINT64  =   8,
        IODEF_VALUE_TYPE_FLOAT   =   9,
        IODEF_VALUE_TYPE_DOUBLE  =  10,
        IODEF_VALUE_TYPE_STRING  =  11,
        IODEF_VALUE_TYPE_TIME    =  12,
        IODEF_VALUE_TYPE_DATA    =  13,
        IODEF_VALUE_TYPE_ENUM    =  14,
        IODEF_VALUE_TYPE_LIST    =  15,
        IODEF_VALUE_TYPE_CLASS   =  16
} iodef_value_type_id_t;


typedef struct {
        void *object;
        int class_id;
} iodef_value_type_class_t;

typedef struct {
        int value;
        int class_id;
} iodef_value_type_enum_t;


typedef union {
        int8_t int8_val;
        uint8_t uint8_val;
        int16_t int16_val;
        uint16_t uint16_val;
        int32_t int32_val;
        uint32_t uint32_val;
        int64_t int64_val;
        uint64_t uint64_val;
        float float_val;
        double double_val;
        libiodef_string_t *string_val;
        iodef_time_t *time_val;
        iodef_data_t *data_val;
        libiodef_list_t list_val;
        iodef_value_type_enum_t enum_val;
        iodef_value_type_class_t class_val;
} iodef_value_type_data_t;


typedef struct {
        iodef_value_type_id_t id;
        iodef_value_type_data_t data;
} iodef_value_type_t;


#include "iodef-criteria.h"

int iodef_value_type_ref(const iodef_value_type_t *src);

int iodef_value_type_copy(const iodef_value_type_t *src, void *dst);

int iodef_value_type_read(iodef_value_type_t *dst, const char *buf);

int iodef_value_type_write(const iodef_value_type_t *src, libiodef_string_t *out);

void iodef_value_type_destroy(iodef_value_type_t *type);

int iodef_value_type_clone(const iodef_value_type_t *src, iodef_value_type_t *dst);

int iodef_value_type_compare(const iodef_value_type_t *type1, const iodef_value_type_t *type2,
                             iodef_criterion_operator_t op);

int iodef_value_type_check_operator(iodef_value_type_id_t type, iodef_criterion_operator_t op);

int iodef_value_type_get_applicable_operators(iodef_value_type_id_t type, iodef_criterion_operator_t *result);

const char *iodef_value_type_to_string(iodef_value_type_id_t type);

#ifdef __cplusplus
 }
#endif

#endif
