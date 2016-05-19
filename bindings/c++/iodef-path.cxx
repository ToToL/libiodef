/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@prelude-ids.com>
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

#include "iodef.hxx"
#include "iodef-path.hxx"
#include "iodef-class.hxx"
#include "prelude-error.hxx"

#include "iodef-object-prv.h"

using namespace Iodef;


IODEFPath::IODEFPath(const char *buffer)
{
        int ret;

        ret = iodef_path_new_fast(&_path, buffer);
        if ( ret < 0 )
                throw PreludeError(ret);
}



IODEFPath::IODEFPath(IODEF &iodef, const char *buffer)
{
        int ret;
        iodef_object_t *obj = (iodef_object_t *) iodef;

        ret = iodef_path_new_from_root_fast(&_path, obj->_iodef_object_id, buffer);
        if ( ret < 0 )
                throw PreludeError(ret);
}



IODEFPath::IODEFPath(iodef_path_t *path)
{
        _path = path;
}


IODEFPath::IODEFPath(const IODEFPath &path)
{
        _path = (path._path) ? iodef_path_ref(path._path) : NULL;
}


IODEFPath::~IODEFPath()
{
        iodef_path_destroy(_path);
}


IODEFValue IODEFPath::get(const IODEF &message) const
{
        int ret;
        iodef_value_t *value;

        ret = iodef_path_get(_path, message, &value);
        if ( ret < 0 )
                throw PreludeError(ret);

        else if ( ret == 0 )
                return IODEFValue((iodef_value_t *) NULL);

        return IODEFValue(value);
}



Iodef::IODEFValue::IODEFValueTypeEnum IODEFPath::getValueType(int depth) const
{
        return (Iodef::IODEFValue::IODEFValueTypeEnum) iodef_path_get_value_type(_path, depth);
}


int IODEFPath::checkOperator(iodef_criterion_operator_t op) const
{
        return iodef_path_check_operator(_path, op);
}



iodef_criterion_operator_t IODEFPath::getApplicableOperators() const
{
        int ret;
        iodef_criterion_operator_t res;

        ret = iodef_path_get_applicable_operators(_path, &res);
        if ( ret < 0 )
                throw PreludeError(ret);

        return res;
}



void IODEFPath::set(IODEF &message, const std::vector<IODEF> &value) const
{
        int ret;
        IODEFValue v = value;

        ret = iodef_path_set(_path, message, v);
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, IODEF *value) const
{
        int ret;

        if ( ! value )
                ret = iodef_path_set(_path, message, NULL);
        else
                ret = iodef_path_set(_path, message, IODEFValue(value));

        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, IODEFValue *value) const
{
        int ret;

        if ( ! value )
                ret = iodef_path_set(_path, message, NULL);
        else
                ret = iodef_path_set(_path, message, *value);

        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, const std::vector<IODEFValue> &value) const
{
        int ret;
        IODEFValue v = value;

        ret = iodef_path_set(_path, message, v);
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, IODEFValue &value) const
{
        int ret;

        ret = iodef_path_set(_path, message, value);
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, const std::string &value) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(value));
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, const char *value) const
{
        int ret;

        if ( value )
                ret = iodef_path_set(_path, message, IODEFValue(value));
        else
                ret = iodef_path_set(_path, message, (iodef_value_t *) NULL);

        if ( ret < 0 )
                throw PreludeError(ret);
}



void IODEFPath::set(IODEF &message, int32_t value) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(value));
        if ( ret < 0 )
                throw PreludeError(ret);
}



void IODEFPath::set(IODEF &message, int64_t value) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(value));
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, uint64_t value) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(value));
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, float value) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(value));
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, double value) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(value));
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFPath::set(IODEF &message, IODEFTime &time) const
{
        int ret;

        ret = iodef_path_set(_path, message, IODEFValue(time));
        if ( ret < 0 )
                throw PreludeError(ret);
}



IODEFClass IODEFPath::getClass(int depth) const
{
        return IODEFClass(iodef_path_get_class(_path, depth));
}


int IODEFPath::setIndex(unsigned int index, int depth)
{
        if ( depth < 0 )
                depth = getDepth();

        return iodef_path_set_index(_path, depth, index);
}


int IODEFPath::undefineIndex(int depth)
{
        if ( depth < 0 )
                depth = getDepth();

        return iodef_path_undefine_index(_path, depth);
}


int IODEFPath::getIndex(int depth) const
{
        if ( depth < 0 )
                depth = getDepth();

        return iodef_path_get_index(_path, depth);
}


int IODEFPath::makeChild(const char *child_name, unsigned int index=0)
{
        return iodef_path_make_child(_path, child_name, index);
}



int IODEFPath::makeParent()
{
        return iodef_path_make_parent(_path);
}


int IODEFPath::compare(IODEFPath *path, int depth) const
{
        int ret;

        if ( depth < 0 )
                ret = iodef_path_compare(_path, path->_path);
        else
                ret = iodef_path_ncompare(_path, path->_path, depth);

        return ret;
}


IODEFPath IODEFPath::clone() const
{
        int ret;
        iodef_path_t *cpath;

        ret = iodef_path_clone(_path, &cpath);
        if ( ret < 0 )
                throw PreludeError(ret);

        return IODEFPath(cpath);
}


const char *IODEFPath::getName(int depth) const
{
        return iodef_path_get_name(_path, depth);
}


bool IODEFPath::isAmbiguous() const
{
        return iodef_path_is_ambiguous(_path);
}


int IODEFPath::hasLists() const
{
        return iodef_path_has_lists(_path);
}


bool IODEFPath::isList(int depth) const
{
        return iodef_path_is_list(_path, depth);
}


unsigned int IODEFPath::getDepth() const
{
        return iodef_path_get_depth(_path);
}


IODEFPath &IODEFPath::operator=(const IODEFPath &path)
{
        if ( this != &path && _path != path._path ) {
                if ( _path )
                        iodef_path_destroy(_path);

                _path = (path._path) ? iodef_path_ref(path._path) : NULL;
        }

        return *this;
}


IODEFPath::operator iodef_path_t *() const
{
        return _path;
}

