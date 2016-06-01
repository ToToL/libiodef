/*****
*
* Copyright (C) 2002-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodef-ids.com>
* Author: Krzysztof Zaraska
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#include "libmissing.h"
#include "libiodef-list.h"
#include "libiodef-log.h"
#include "libiodef-inttypes.h"
#include "libiodef-string.h"

#define LIBIODEF_ERROR_SOURCE_DEFAULT LIBIODEF_ERROR_SOURCE_IODEF_TYPE
#include "libiodef-error.h"

#include "iodef-time.h"
#include "iodef-data.h"
#include "iodef-value.h"
#include "iodef-class.h"

#include "iodef-tree-wrap.h"
#include "iodef-document-print.h"
#include "iodef-document-print-json.h"
#include "iodef-document-write.h"
#include "iodef-tree-data.h"
#include "iodef-path.h"



static inline int is_class_valid(iodef_class_id_t class)
{
        if ( class < 0 || (size_t) class >= sizeof(object_data) / sizeof(*object_data) )
                return libiodef_error_verbose(LIBIODEF_ERROR_IODEF_CLASS_UNKNOWN, "Unknown IODEF class '%d'", (int) class);

        return 0;
}


static inline int is_child_valid(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        if ( child < 0 || (size_t) child >= object_data[class].children_list_elem )
                return libiodef_error_verbose(LIBIODEF_ERROR_IODEF_CLASS_UNKNOWN_CHILD, "Unknown IODEF child '%d' for class '%s'",
                                             (int) child, object_data[class].name);

        return 0;
}




iodef_class_child_id_t iodef_class_find_child(iodef_class_id_t class, const char *name)
{
        int ret;
        size_t i;
        const children_list_t *list;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        list = object_data[class].children_list;

        if ( list ) {
                for ( i = 0; i < object_data[class].children_list_elem; i++ )
                        if ( strcasecmp(list[i].name, name) == 0)
                                return i;
        }

        return libiodef_error_verbose(LIBIODEF_ERROR_IODEF_CLASS_UNKNOWN_CHILD, "Unknown IODEF child '%s' for class '%s'", name, iodef_class_get_name(class));
}




libiodef_bool_t iodef_class_is_child_union_member(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return (object_data[class].children_list[child].union_id > 0) ? TRUE : FALSE;
}



libiodef_bool_t iodef_class_is_child_list(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].list;
}



libiodef_bool_t iodef_class_is_child_keyed_list(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].keyed_list;
}



iodef_value_type_id_t iodef_class_get_child_value_type(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list[child].type;
}



size_t iodef_class_get_child_count(iodef_class_id_t class)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].children_list_elem;
}



int iodef_class_get_child_union_id(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;
        const children_list_t *c;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        c = &object_data[class].children_list[child];
        if ( ! c->union_id )
                return libiodef_error(LIBIODEF_ERROR_IODEF_CLASS_CHILD_NOT_UNION);

        return c->union_id;
}



iodef_class_id_t iodef_class_get_child_class(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;
        const children_list_t *c;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        c = &object_data[class].children_list[child];
        if ( c->type != IODEF_VALUE_TYPE_CLASS && c->type != IODEF_VALUE_TYPE_ENUM )
                return libiodef_error(LIBIODEF_ERROR_IODEF_CLASS_CHILD_NOT_CLASS);

        return c->class;
}



const char *iodef_class_get_child_name(iodef_class_id_t class, iodef_class_child_id_t child)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return NULL;

        return object_data[class].children_list[child].name;
}




iodef_class_id_t iodef_class_find(const char *name)
{
        iodef_class_id_t i;

        for ( i = 0; object_data[i].name != NULL; i++ )
                if ( strcasecmp(object_data[i].name, name) == 0 )
                        return i;

        return libiodef_error_verbose(LIBIODEF_ERROR_IODEF_CLASS_UNKNOWN_NAME, "Unknown IODEF class '%s'", name);
}


int iodef_class_enum_to_numeric(iodef_class_id_t class, const char *val)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        if ( ! object_data[class].to_numeric )
                return -1;

            return object_data[class].to_numeric(val);
}


const char *iodef_class_enum_to_string(iodef_class_id_t class, int val)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return NULL;

        if ( ! object_data[class].to_string )
                return NULL;

        return object_data[class].to_string(val);
}


int iodef_class_get_child(void *ptr, iodef_class_id_t class, iodef_class_child_id_t child, void **childptr)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].get_child(ptr, child, childptr);
}




int iodef_class_new_child(void *ptr, iodef_class_id_t class, iodef_class_child_id_t child, int n, void **childptr)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].new_child(ptr, child, n, childptr);
}



int iodef_class_destroy_child(void *ptr, iodef_class_id_t class, iodef_class_child_id_t child, int n)
{
        int ret;

        ret = is_child_valid(class, child);
        if ( ret < 0 )
                return ret;

        return object_data[class].destroy_child(ptr, child, n);
}


int iodef_class_copy(iodef_class_id_t class, const void *src, void *dst)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].copy(src, dst);
}



int iodef_class_clone(iodef_class_id_t class, const void *src, void **dst)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].clone(src, dst);
}



int iodef_class_compare(iodef_class_id_t class, const void *c1, const void *c2)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].compare(c1, c2);
}



int iodef_class_destroy(iodef_class_id_t class, void *obj)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        object_data[class].destroy(obj);

        return 0;
}



int iodef_class_ref(iodef_class_id_t class, void *obj)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        object_data[class].ref(obj);

        return 0;
}



int iodef_class_print(iodef_class_id_t class, void *obj, libiodef_io_t *fd)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print(obj, fd);
}


int iodef_class_print_json(iodef_class_id_t class, void *obj, libiodef_io_t *fd)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print_json(obj, fd);
}


int iodef_class_print_binary(iodef_class_id_t class, void *obj, libiodef_io_t *fd)
{
        int ret;
        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].print_binary(obj, fd);
}


const char *iodef_class_get_name(iodef_class_id_t class)
{
        if ( is_class_valid(class) < 0 )
                return NULL;

        return object_data[class].name;
}



libiodef_bool_t iodef_class_is_listed(iodef_class_id_t class)
{
        int ret;

        ret = is_class_valid(class);
        if ( ret < 0 )
                return ret;

        return object_data[class].is_listed;
}
