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

#ifndef _LIBIODEF_LIBIODEF_LINKED_OBJECT_H
#define _LIBIODEF_LIBIODEF_LINKED_OBJECT_H


#include "libiodef-list.h"


#define LIBIODEF_LINKED_OBJECT    \
        unsigned int _object_id; \
        libiodef_list_t _list


typedef struct {
        LIBIODEF_LINKED_OBJECT;
} libiodef_linked_object_t;



static inline void libiodef_linked_object_del(libiodef_linked_object_t *obj) 
{
        libiodef_list_del(&obj->_list);
}



static inline void libiodef_linked_object_del_init(libiodef_linked_object_t *obj) 
{
        libiodef_list_del(&obj->_list);
        libiodef_list_init(&obj->_list);
}



static inline void libiodef_linked_object_add(libiodef_list_t *head, libiodef_linked_object_t *obj) 
{
        libiodef_list_add(head, &obj->_list);
}



static inline void libiodef_linked_object_add_tail(libiodef_list_t *head, libiodef_linked_object_t *obj) 
{
        libiodef_list_add_tail(head, &obj->_list);
}


static inline void libiodef_linked_object_set_id(libiodef_linked_object_t *obj, unsigned int id)
{
        obj->_object_id = id;
}


static inline unsigned int libiodef_linked_object_get_id(libiodef_linked_object_t *obj)
{
        return obj->_object_id;
}



#define libiodef_linked_object_get_object(object)  \
        (void *) libiodef_list_entry(object, libiodef_linked_object_t, _list)


#endif /* _LIBIODEF_LIBIODEF_LINKED_OBJECT_H */
