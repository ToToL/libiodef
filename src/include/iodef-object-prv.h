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

#ifndef _LIBIODEF_IODEF_OBJECT_PRV_H
#define _LIBIODEF_IODEF_OBJECT_PRV_H

#define IODEF_OBJECT unsigned int _iodef_object_id
#define IODEF_LINKED_OBJECT IODEF_OBJECT; libiodef_list_t _list

struct iodef_object {
        IODEF_OBJECT;
};

struct iodef_linked_object {
        IODEF_OBJECT;
        libiodef_list_t _list;
};

#define iodef_linked_object_get_object(object) \
        (void *) libiodef_list_entry(object, struct iodef_linked_object, _list)

#endif /* _LIBIODEF_IODEF_OBJECT_PRV_H */
