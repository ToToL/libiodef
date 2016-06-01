/*****
*
* Copyright (C) 2009-2016 CS-SI. All Rights Reserved.
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

#include <iostream>

#include <string.h>
#include <iodef.h>
#include <iodef-path.h>
#include <iodef-document-print.h>
#include <iodef-object-prv.h>

#include "libiodef-error.hxx"
#include "iodef-path.hxx"
#include "iodef.hxx"

using namespace Iodef;


IODEF::IODEF() : _object(NULL)
{
        int ret;

        ret = iodef_document_new((iodef_document_t **) &_object);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEF::IODEF(const char *json) : _object(NULL)
{
        int ret;

        ret = iodef_object_new_from_json(&_object, json);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


IODEF::IODEF(const IODEF &iodef)
{
        _object = (iodef._object) ? iodef_object_ref(iodef._object) : NULL;
}


IODEF::IODEF(iodef_object_t *object)
{
        _object = object;
}


IODEF::~IODEF()
{
        if ( _object )
                iodef_object_destroy(_object);
}

void IODEF::set(const char *path, const std::vector<IODEF> &value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, IODEF *value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, const std::vector<IODEFValue> &value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, IODEFValue *value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, IODEFValue &value)
{
        IODEFPath(*this, path).set(*this, &value);
}


void IODEF::set(const char *path, const std::string &value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, const char *value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, int32_t value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, int64_t value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, uint64_t value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, float value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, double value)
{
        IODEFPath(*this, path).set(*this, value);
}


void IODEF::set(const char *path, IODEFTime &value)
{
        IODEFPath(*this, path).set(*this, value);
}


IODEFValue IODEF::get(const char *path)
{
        return IODEFPath(*this, path).get(*this);
}


int IODEF::operator == (const IODEF *iodef)
{
        if ( ! iodef )
                return 0;

        if ( _object == iodef->_object )
                return 1;

        return iodef_object_compare(_object, iodef->_object) == 0 ? 1 : 0;
}


IODEF IODEF::clone()
{
        int ret;
        iodef_object_t *clone;

        ret = iodef_object_clone(_object, &clone);
        if ( ret < 0 )
                throw LibIodefError(ret);

        return IODEF(clone);
}


iodef_class_id_t IODEF::getId() const
{
        return _object->_iodef_object_id;
}


const std::string IODEF::toString() const
{
        int ret;
        std::string str;
        libiodef_io_t *fd;

        ret = libiodef_io_new(&fd);
        if ( ret < 0 )
                throw LibIodefError(ret);

        libiodef_io_set_buffer_io(fd);
        iodef_object_print(_object, fd);

        str.assign((const char *) libiodef_io_get_fdptr(fd), libiodef_io_pending(fd));

        libiodef_io_close(fd);
        libiodef_io_destroy(fd);

        return str;
}


const std::string IODEF::toJSON() const
{
        int ret;
        std::string str;
        libiodef_io_t *fd;

        ret = libiodef_io_new(&fd);
        if ( ret < 0 )
                throw LibIodefError(ret);

        libiodef_io_set_buffer_io(fd);
        iodef_object_print_json(_object, fd);

        str.assign((const char *) libiodef_io_get_fdptr(fd), libiodef_io_pending(fd));

        libiodef_io_close(fd);
        libiodef_io_destroy(fd);

        return str;
}

const std::string IODEF::toBinary() const
{
        int ret;
        std::string str;
        libiodef_io_t *fd;

        ret = libiodef_io_new(&fd);
        if ( ret < 0 )
                throw LibIodefError(ret);

        libiodef_io_set_buffer_io(fd);
        iodef_object_print_binary(_object, fd);

        str.assign((const char *) libiodef_io_get_fdptr(fd), libiodef_io_pending(fd));

        libiodef_io_close(fd);
        libiodef_io_destroy(fd);

        return str;
}


IODEF::operator const std::string() const
{
        return toString();
}


IODEF::operator iodef_object_t *() const
{
        return (iodef_object_t *) _object;
}


IODEF &IODEF::operator = (const IODEF &iodef)
{
        if ( this != &iodef && _object != iodef._object ) {
                if ( _object )
                        iodef_object_destroy(_object);

                _object = (iodef._object) ? iodef_object_ref(iodef._object) : NULL;
        }

        return *this;
}
