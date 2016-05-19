/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
* Author: Yoann Vandoorselaere <yoann.v@prelude-ids.com>
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

#ifndef _LIBPRELUDE_IODEF_VALUE_H
#define _LIBPRELUDE_IODEF_VALUE_H


typedef struct iodef_value iodef_value_t;

#include "prelude-io.h"
#include "iodef-value-type.h"
#include "prelude-string.h"
#include "iodef-class.h"
#include "iodef-path.h"

#ifdef __cplusplus
 extern "C" {
#endif

int iodef_value_new_int8(iodef_value_t **value, int8_t val);
int iodef_value_new_uint8(iodef_value_t **value, uint8_t val);
int iodef_value_new_int16(iodef_value_t **value, int16_t val);
int iodef_value_new_uint16(iodef_value_t **value, uint16_t val);
int iodef_value_new_int32(iodef_value_t **value, int32_t val);
int iodef_value_new_uint32(iodef_value_t **value, uint32_t val);
int iodef_value_new_int64(iodef_value_t **value, int64_t val);
int iodef_value_new_uint64(iodef_value_t **value, uint64_t val);
int iodef_value_new_float(iodef_value_t **value, float val);
int iodef_value_new_double(iodef_value_t **value, double val);
int iodef_value_new_string(iodef_value_t **value, prelude_string_t *string);
int iodef_value_new_time(iodef_value_t **value, iodef_time_t *time);
int iodef_value_new_data(iodef_value_t **value, iodef_data_t *data);
int iodef_value_new_class(iodef_value_t **value, iodef_class_id_t classid, void *ptr);
int iodef_value_new_list(iodef_value_t **value);
int iodef_value_new_enum(iodef_value_t **value, iodef_class_id_t classid, const char *buf);
int iodef_value_new_enum_from_string(iodef_value_t **value, iodef_class_id_t classid, const char *buf);
int iodef_value_new_enum_from_numeric(iodef_value_t **value, iodef_class_id_t classid, int val);

int iodef_value_set_int8(iodef_value_t *value, int8_t val);
int iodef_value_set_uint8(iodef_value_t *value, uint8_t val);
int iodef_value_set_int16(iodef_value_t *value, int16_t val);
int iodef_value_set_uint16(iodef_value_t *value, uint16_t val);
int iodef_value_set_int32(iodef_value_t *value, int32_t val);
int iodef_value_set_uint32(iodef_value_t *value, uint32_t val);
int iodef_value_set_int64(iodef_value_t *value, int64_t val);
int iodef_value_set_uint64(iodef_value_t *value, uint64_t val);
int iodef_value_set_float(iodef_value_t *value, float val);
int iodef_value_set_double(iodef_value_t *value, double val);
int iodef_value_set_string(iodef_value_t *value, prelude_string_t *string);
int iodef_value_set_time(iodef_value_t *value, iodef_time_t *time);
int iodef_value_set_data(iodef_value_t *value, iodef_data_t *data);
int iodef_value_set_enum(iodef_value_t *value, iodef_class_id_t classid, const char *buf);
int iodef_value_set_enum_from_string(iodef_value_t *value, iodef_class_id_t classid, const char *buf);
int iodef_value_set_enum_from_numeric(iodef_value_t *value, iodef_class_id_t classid, int no);
int iodef_value_set_class(iodef_value_t *value, iodef_class_id_t classid, void *ptr);

int iodef_value_new(iodef_value_t **value, iodef_value_type_id_t type, void *ptr);
int iodef_value_new_from_path(iodef_value_t **value, iodef_path_t *path, const char *buf);
int iodef_value_new_from_string(iodef_value_t **value, iodef_value_type_id_t type, const char *buf);

int8_t iodef_value_get_int8(const iodef_value_t *val);
uint8_t iodef_value_get_uint8(const iodef_value_t *val);
int16_t iodef_value_get_int16(const iodef_value_t *val);
uint16_t iodef_value_get_uint16(const iodef_value_t *val);
int32_t iodef_value_get_int32(const iodef_value_t *val);
uint32_t iodef_value_get_uint32(const iodef_value_t *val);
int64_t iodef_value_get_int64(const iodef_value_t *val);
uint64_t iodef_value_get_uint64(const iodef_value_t *val);
int iodef_value_get_enum(const iodef_value_t *val);
float iodef_value_get_float(const iodef_value_t *val);
double iodef_value_get_double(const iodef_value_t *val);

iodef_time_t *iodef_value_get_time(const iodef_value_t *val);
iodef_data_t *iodef_value_get_data(const iodef_value_t *val);
prelude_string_t *iodef_value_get_string(const iodef_value_t *val);

int iodef_value_list_add(iodef_value_t *list, iodef_value_t *item);
prelude_bool_t iodef_value_is_list(const iodef_value_t *list);
prelude_bool_t iodef_value_list_is_empty(const iodef_value_t *list);

int iodef_value_have_own_data(iodef_value_t *value);
int iodef_value_dont_have_own_data(iodef_value_t *value);

iodef_value_type_id_t iodef_value_get_type(const iodef_value_t *value);
iodef_class_id_t iodef_value_get_class(const iodef_value_t *value);

void *iodef_value_get_object(const iodef_value_t *value);

int iodef_value_iterate(const iodef_value_t *value, int (*callback)(iodef_value_t *ptr, void *extra), void *extra);

int iodef_value_iterate_reversed(const iodef_value_t *value, int (*callback)(iodef_value_t *ptr, void *extra), void *extra);

int iodef_value_get_nth2(const iodef_value_t *val, int index, iodef_value_t **ret);

iodef_value_t *iodef_value_get_nth(const iodef_value_t *val, int n);

int iodef_value_get_count(const iodef_value_t *val);

int iodef_value_clone(const iodef_value_t *val, iodef_value_t **dst);

iodef_value_t *iodef_value_ref(iodef_value_t *val);

int iodef_value_print(const iodef_value_t *val, prelude_io_t *fd);

int iodef_value_to_string(const iodef_value_t *value, prelude_string_t *out);

int iodef_value_get(const iodef_value_t *val, void *res);

int iodef_value_match(iodef_value_t *val1, iodef_value_t *val2, iodef_criterion_operator_t op);

int iodef_value_check_operator(const iodef_value_t *value, iodef_criterion_operator_t op);

int iodef_value_get_applicable_operators(const iodef_value_t *value, iodef_criterion_operator_t *result);

void iodef_value_destroy(iodef_value_t *val);

#ifndef SWIG

int _iodef_value_copy_internal(const iodef_value_t *val,
                               iodef_value_type_id_t res_type, iodef_class_id_t res_id, void *res);

int _iodef_value_cast(iodef_value_t *val, iodef_value_type_id_t target_type, iodef_class_id_t enum_class);
#endif

#ifdef __cplusplus
 }
#endif

#endif /* _LIBPRELUDE_IODEF_VALUE_H */
