/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#include "iodef.h"
#include "iodef-object-prv.h"
#include "iodef-object.h"
#include "iodef-class.h"


typedef struct iodef_linked_object iodef_linked_object_t;



iodef_class_id_t iodef_object_get_class(iodef_object_t *obj)
{
        return obj->_iodef_object_id;
}



iodef_object_t *iodef_object_ref(iodef_object_t *obj)
{
        int ret;

        ret = iodef_class_ref(obj->_iodef_object_id, obj);
        prelude_return_val_if_fail(ret == 0, NULL);

        return obj;
}



void iodef_object_destroy(iodef_object_t *obj)
{
        iodef_class_destroy(obj->_iodef_object_id, obj);
}



int iodef_object_compare(iodef_object_t *obj1, iodef_object_t *obj2)
{
        if ( obj1->_iodef_object_id != obj2->_iodef_object_id )
                return -1;

        return iodef_class_compare(obj1->_iodef_object_id, obj1, obj2);
}



int iodef_object_clone(iodef_object_t *obj, iodef_object_t **dst)
{
        return iodef_class_clone(obj->_iodef_object_id, obj, (void **) dst);
}



int iodef_object_copy(iodef_object_t *src, iodef_object_t *dst)
{
        return iodef_class_copy(src->_iodef_object_id, src, dst);
}



int iodef_object_print(iodef_object_t *obj, prelude_io_t *fd)
{
        return iodef_class_print(obj->_iodef_object_id, obj, fd);
}

int iodef_object_print_json(iodef_object_t *obj, prelude_io_t *fd)
{
        return iodef_class_print_json(obj->_iodef_object_id, obj, fd);
}

int iodef_object_print_binary(iodef_object_t *obj, prelude_io_t *fd)
{
        return iodef_class_print_binary(obj->_iodef_object_id, obj, fd);
}

void iodef_object_add(prelude_list_t *head, iodef_object_t *object)
{
        prelude_return_if_fail(iodef_class_is_listed(object->_iodef_object_id));
        prelude_list_add(head, &((iodef_linked_object_t *) object)->_list);
}


void iodef_object_add_tail(prelude_list_t *head, iodef_object_t *object)
{
        prelude_return_if_fail(iodef_class_is_listed(object->_iodef_object_id));
        prelude_list_add_tail(head, &((iodef_linked_object_t *) object)->_list);
}


void iodef_object_del(iodef_object_t *object)
{
        prelude_return_if_fail(iodef_class_is_listed(object->_iodef_object_id));
        prelude_list_del(&((iodef_linked_object_t *) object)->_list);
}


void iodef_object_del_init(iodef_object_t *object)
{
        prelude_return_if_fail(iodef_class_is_listed(object->_iodef_object_id));
        prelude_list_del(&((iodef_linked_object_t *) object)->_list);
        prelude_list_init(&((iodef_linked_object_t *) object)->_list);
}


void *iodef_object_get_list_entry(prelude_list_t *elem)
{
        return prelude_list_entry(elem, iodef_linked_object_t, _list);
}
