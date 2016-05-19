/*****
*
* Copyright (C) 2002, 2003, 2004 Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
* All Rights Reserved
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


#ifndef _LIBPRELUDE_IODEF_CLASS_H
#define _LIBPRELUDE_IODEF_CLASS_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef int iodef_class_id_t;
typedef int iodef_class_child_id_t;

#include "iodef-value.h"

/*
 *
 */
prelude_bool_t iodef_class_is_child_list(iodef_class_id_t classid, iodef_class_child_id_t child);

prelude_bool_t iodef_class_is_child_keyed_list(iodef_class_id_t classid, iodef_class_child_id_t child);

prelude_bool_t iodef_class_is_child_union_member(iodef_class_id_t classid, iodef_class_child_id_t child);

int iodef_class_get_child_union_id(iodef_class_id_t classid, iodef_class_child_id_t child);

iodef_class_id_t iodef_class_get_child_class(iodef_class_id_t classid, iodef_class_child_id_t child);

size_t iodef_class_get_child_count(iodef_class_id_t classid);

iodef_value_type_id_t iodef_class_get_child_value_type(iodef_class_id_t classid, iodef_class_child_id_t child);


/*
 *
 */
int iodef_class_enum_to_numeric(iodef_class_id_t classid, const char *val);

const char *iodef_class_enum_to_string(iodef_class_id_t classid, int val);


/*
 *
 */
int iodef_class_get_child(void *ptr, iodef_class_id_t classid, iodef_class_child_id_t child, void **childptr);

int iodef_class_new_child(void *ptr, iodef_class_id_t classid, iodef_class_child_id_t child, int n, void **childptr);

int iodef_class_destroy_child(void *ptr, iodef_class_id_t classid, iodef_class_child_id_t child, int n);


/*
 *
 */
iodef_class_id_t iodef_class_find(const char *name);

iodef_class_child_id_t iodef_class_find_child(iodef_class_id_t classid, const char *name);

/*
 *
 */
const char *iodef_class_get_name(iodef_class_id_t classid);

const char *iodef_class_get_child_name(iodef_class_id_t classid, iodef_class_child_id_t child);


/*
 *
 */
int iodef_class_copy(iodef_class_id_t classid, const void *src, void *dst);

int iodef_class_clone(iodef_class_id_t classid, const void *src, void **dst);

int iodef_class_compare(iodef_class_id_t classid, const void *c1, const void *c2);

int iodef_class_ref(iodef_class_id_t classid, void *obj);

int iodef_class_print(iodef_class_id_t classid, void *obj, prelude_io_t *fd);

int iodef_class_print_json(iodef_class_id_t classid, void *obj, prelude_io_t *fd);

int iodef_class_print_binary(iodef_class_id_t classid, void *obj, prelude_io_t *fd);

int iodef_class_destroy(iodef_class_id_t classid, void *obj);

prelude_bool_t iodef_class_is_listed(iodef_class_id_t classid);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBPRELUDE_IODEF_CLASS_H */
