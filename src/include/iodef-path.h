/*****
*
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodef-ids.com>
* Author: Krzysztof Zaraska <kzaraska@student.uci.agh.edu.pl>
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

#ifndef _LIBIODEF_IODEF_PATH_H
#define _LIBIODEF_IODEF_PATH_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

typedef struct iodef_path iodef_path_t;

#include <stdarg.h>
#include "libiodef-macros.h"
#include "iodef-value.h"
#include "iodef-tree-wrap.h"

int iodef_path_get(const iodef_path_t *path, void *object, iodef_value_t **ret);

int iodef_path_set(const iodef_path_t *path, void *object, iodef_value_t *value);

int iodef_path_new(iodef_path_t **path, const char *format, ...) LIBIODEF_FMT_PRINTF(2, 3);

int iodef_path_new_v(iodef_path_t **path, const char *format, va_list args) LIBIODEF_FMT_PRINTF(2, 0);

int iodef_path_new_from_root_fast(iodef_path_t **path, iodef_class_id_t rootclass, const char *buffer);

int iodef_path_new_fast(iodef_path_t **path, const char *buffer);

iodef_class_id_t iodef_path_get_class(const iodef_path_t *path, int depth);

iodef_value_type_id_t iodef_path_get_value_type(const iodef_path_t *path, int depth);

int iodef_path_set_index(iodef_path_t *path, unsigned int depth, int index);

int iodef_path_undefine_index(iodef_path_t *path, unsigned int depth);

int iodef_path_get_index(const iodef_path_t *path, unsigned int depth);

int iodef_path_get_key(const iodef_path_t *path, unsigned int depth, const char **key);

int iodef_path_make_child(iodef_path_t *path, const char *child_name, int index);

int iodef_path_make_parent(iodef_path_t *path);

void iodef_path_destroy(iodef_path_t *path);

int iodef_path_ncompare(const iodef_path_t *p1, const iodef_path_t *p2, unsigned int depth);

int iodef_path_compare(const iodef_path_t *p1, const iodef_path_t *p2);

int iodef_path_clone(const iodef_path_t *src, iodef_path_t **dst);

iodef_path_t *iodef_path_ref(iodef_path_t *path);

const char *iodef_path_get_name(const iodef_path_t *path, int depth);

libiodef_bool_t iodef_path_is_ambiguous(const iodef_path_t *path);

int iodef_path_has_lists(const iodef_path_t *path);

libiodef_bool_t iodef_path_is_list(const iodef_path_t *path, int depth);

unsigned int iodef_path_get_depth(const iodef_path_t *path);

int iodef_path_check_operator(const iodef_path_t *path, iodef_criterion_operator_t op);

int iodef_path_get_applicable_operators(const iodef_path_t *path, iodef_criterion_operator_t *result);

#ifndef SWIG
void _iodef_path_cache_lock(void);

void _iodef_path_cache_reinit(void);

void _iodef_path_cache_unlock(void);

void _iodef_path_cache_destroy(void);
#endif

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEF_IODEF_PATH_H */
