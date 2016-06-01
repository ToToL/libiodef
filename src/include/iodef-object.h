/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#ifndef _LIBIODEF_IODEF_OBJECT_H
#define _LIBIODEF_IODEF_OBJECT_H

#ifdef __cplusplus
  extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libiodef-io.h"
#include "libiodef-list.h"

typedef struct iodef_object iodef_object_t;

iodef_class_id_t iodef_object_get_class(iodef_object_t *obj);

iodef_object_t *iodef_object_ref(iodef_object_t *obj);

void iodef_object_destroy(iodef_object_t *obj);

int iodef_object_compare(iodef_object_t *obj1, iodef_object_t *obj2);

int iodef_object_clone(iodef_object_t *obj, iodef_object_t **dst);

int iodef_object_copy(iodef_object_t *src, iodef_object_t *dst);

int iodef_object_print(iodef_object_t *obj, libiodef_io_t *fd);

int iodef_object_print_json(iodef_object_t *obj, libiodef_io_t *fd);

int iodef_object_print_binary(iodef_object_t *obj, libiodef_io_t *fd);

void iodef_object_add(libiodef_list_t *head, iodef_object_t *obj);

void iodef_object_add_tail(libiodef_list_t *head, iodef_object_t *obj);

void iodef_object_del(iodef_object_t *object);

void iodef_object_del_init(iodef_object_t *object);

void *iodef_object_get_list_entry(libiodef_list_t *listm);

int iodef_object_new_from_json(iodef_object_t **object, const char * json_message);

#ifdef __cplusplus
  }
#endif

#endif /* _LIBIODEF_IODEF_OBJECT_H */
