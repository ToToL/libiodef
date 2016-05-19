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

#include <string.h>
#include <sstream>

#include "prelude-error.hxx"
#include "iodef.hxx"
#include "iodef-value.hxx"
#include "iodef-criteria.hxx"

using namespace Iodef;


IODEFValue::~IODEFValue()
{
        if ( _value )
                iodef_value_destroy(_value);
}


IODEFValue::IODEFValueTypeEnum IODEFValue::getType() const
{
        return (IODEFValue::IODEFValueTypeEnum) iodef_value_get_type(_value);
}


bool IODEFValue::isNull() const
{
        return (! _value) ? TRUE : FALSE;
}


IODEFValue::IODEFValue()
{
        _value = NULL;
}


IODEFValue::IODEFValue(const IODEFValue &value)
{
        iodef_value_t *v = NULL;

        if ( value._value )
                v = iodef_value_ref(value._value);

        _value = v;
}


void IODEFValue::_InitFromString(const char *value, size_t len)
{
        int ret;
        prelude_string_t *str;

        ret = prelude_string_new_dup_fast(&str, value, len);
        if ( ret < 0 )
                throw PreludeError(ret);

        ret = iodef_value_new_string(&_value, str);
        if ( ret < 0 ) {
                prelude_string_destroy(str);
                throw PreludeError(ret);
        }
}


IODEFValue::IODEFValue(const char *value)
{
        _InitFromString(value, strlen(value));
}


IODEFValue::IODEFValue(const std::string &value)
{
        _InitFromString(value.c_str(), value.size());
}


IODEFValue::IODEFValue(int32_t value)
{
        int ret = iodef_value_new_int32(&_value, value);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFValue::IODEFValue(int64_t value)
{
        int ret = iodef_value_new_int64(&_value, value);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFValue::IODEFValue(uint64_t value)
{
        int ret = iodef_value_new_uint64(&_value, value);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFValue::IODEFValue(float value)
{
        int ret = iodef_value_new_float(&_value, value);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFValue::IODEFValue(double value)
{
        int ret = iodef_value_new_double(&_value, value);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFValue::IODEFValue(IODEFTime &time)
{
        int ret = iodef_value_new_time(&_value, iodef_time_ref(time));
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFValue::IODEFValue(IODEF *iodef)
{
        int ret;
        iodef_value_t *v = NULL;

        if (  iodef ) {
                ret = iodef_value_new_class(&v, iodef->getId(), iodef_object_ref((iodef_object_t *) *iodef));
                if ( ret < 0 )
                       throw PreludeError(ret);
        }

        _value = v;
}


IODEFValue::IODEFValue(const std::vector<IODEF> &value)
{
        int ret;
        iodef_value_t *vitem;
        std::vector<Iodef::IODEF>::const_iterator i;

        ret = iodef_value_new_list(&_value);
        if ( ret < 0 )
                throw PreludeError(ret);

        for ( i = value.begin(); i != value.end(); i++ ) {
                ret = iodef_value_new_class(&vitem, i->getId(), iodef_object_ref((iodef_object_t *) *i));
                if ( ret < 0 )
                        throw PreludeError(ret);

                iodef_value_list_add(_value, vitem);
        }
}


IODEFValue::IODEFValue(const std::vector<IODEFValue> &value)
{
        int ret;
        std::vector<Iodef::IODEFValue>::const_iterator i;

        ret = iodef_value_new_list(&_value);
        if ( ret < 0 )
                throw PreludeError(ret);

        for ( i = value.begin(); i != value.end(); i++ )
                iodef_value_list_add(_value, iodef_value_ref(*i));
}

IODEFValue::IODEFValue(iodef_value_t *value)
{
        _value = value;
}


int IODEFValue::match(const IODEFValue &value, int op) const
{
        int ret;

        ret = iodef_value_match(this->_value, value._value, (iodef_criterion_operator_t) op);
        if ( ret < 0 )
                throw PreludeError(ret);

        return ret;
}


IODEFValue IODEFValue::clone() const
{
        int ret;
        iodef_value_t *clone;

        ret = iodef_value_clone(_value, &clone);
        if ( ret < 0 )
                throw PreludeError(ret);

        return IODEFValue(clone);
}


const std::string IODEFValue::toString() const
{
        int ret;
        std::string s;
        prelude_string_t *str;

        ret = prelude_string_new(&str);
        if ( ret < 0 )
                throw PreludeError(ret);

        ret = iodef_value_to_string(_value, str);
        if ( ret < 0 ) {
                prelude_string_destroy(str);
                throw PreludeError(ret);
        }

        s = prelude_string_get_string(str);
        prelude_string_destroy(str);

        return s;
}


static int iterate_cb(iodef_value_t *value, void *extra)
{
        std::vector<IODEFValue> *vlist = (std::vector<IODEFValue> *) extra;

        if ( value )
                value = iodef_value_ref(value);

        vlist->push_back(IODEFValue(value));

        return 0;
}


IODEFValue::operator std::vector<IODEFValue> () const
{
        std::vector<IODEFValue> vlist;

        if ( ! _value )
                return vlist;

        if ( getType() != TYPE_LIST ) {
                std::stringstream s;
                s << "Left value doesn't fit '" << iodef_value_type_to_string((iodef_value_type_id_t) getType()) << "' requirement";
                throw PreludeError(s.str());
        }

        iodef_value_iterate(_value, iterate_cb, &vlist);

        return vlist;
}


IODEFValue::operator IODEFTime () const
{
        prelude_except_if_fail(_value);

        if ( getType() != TYPE_TIME ) {
                std::stringstream s;
                s << "Left value doesn't fit '" << iodef_value_type_to_string((iodef_value_type_id_t) getType()) << "' requirement";
                throw PreludeError(s.str());
        }

        return IODEFTime(iodef_time_ref(iodef_value_get_time(_value)));
}



IODEFValue::operator int32_t () const
{
        IODEFValueTypeEnum vtype;

        prelude_except_if_fail(_value);
        vtype = getType();

        if ( vtype == TYPE_INT8 )
                return iodef_value_get_int8(_value);

        else if ( vtype == TYPE_UINT8 )
                return iodef_value_get_uint8(_value);

        else if ( vtype == TYPE_INT16 )
                return iodef_value_get_int16(_value);

        else if ( vtype == TYPE_UINT16 )
                return iodef_value_get_uint16(_value);

        else if ( vtype == TYPE_INT32 )
                return iodef_value_get_int32(_value);

        else if ( vtype == TYPE_ENUM )
                return iodef_value_get_enum(_value);

        std::stringstream s;
        s << "Left value doesn't fit '" << iodef_value_type_to_string((iodef_value_type_id_t) vtype) << "' requirement";
        throw PreludeError(s.str());
}


IODEFValue::operator uint32_t () const
{
        prelude_except_if_fail(_value);

        if ( getType() == TYPE_UINT32 )
                return iodef_value_get_uint32(_value);
        else
                return (int32_t) *this;
}


IODEFValue::operator int64_t () const
{
        prelude_except_if_fail(_value);

        if ( getType() == TYPE_INT64 )
                return iodef_value_get_int64(_value);
        else
                return (uint32_t) *this;
}


IODEFValue::operator uint64_t () const
{
        prelude_except_if_fail(_value);

        if ( getType() == TYPE_UINT64 )
                return iodef_value_get_uint64(_value);
        else
                return (int64_t) *this;
}


IODEFValue::operator float () const
{
        IODEFValueTypeEnum vtype;

        prelude_except_if_fail(_value);
        vtype = getType();

        if ( vtype == TYPE_FLOAT )
                return iodef_value_get_float(_value);

        else if ( vtype == TYPE_DATA ) {
                iodef_data_t *d = iodef_value_get_data(_value);

                if ( iodef_data_get_type(d) == IODEF_DATA_TYPE_FLOAT )
                        return iodef_data_get_float(d);
        }

        std::stringstream s;
        s << "Left value doesn't fit '" << iodef_value_type_to_string((iodef_value_type_id_t) vtype) << "' requirement";
        throw PreludeError(s.str());
}


IODEFValue::operator double () const
{
        prelude_except_if_fail(_value);

        if ( getType() == TYPE_DOUBLE )
                return iodef_value_get_double(_value);
        else
                return (float) *this;
}


std::string IODEFValue::convert_string() const
{
        std::stringstream s;
        prelude_except_if_fail(_value);

        if ( getType() == TYPE_STRING )
                return prelude_string_get_string(iodef_value_get_string(_value));

        else if ( getType() == TYPE_TIME )
                return IODEFTime(iodef_time_ref(iodef_value_get_time(_value)));

        else if ( getType() == TYPE_ENUM )
                return iodef_class_enum_to_string(iodef_value_get_class(_value), iodef_value_get_enum(_value));

        else if ( getType() == TYPE_DATA ) {
                iodef_data_t *d = iodef_value_get_data(_value);
                iodef_data_type_t t = iodef_data_get_type(d);

                if ( t == IODEF_DATA_TYPE_CHAR_STRING )
                        return (const char *) iodef_data_get_char_string(d);

                else if ( t == IODEF_DATA_TYPE_CHAR ) {
                        s << iodef_data_get_char(d);
                        return s.str();
                }

                else if ( t == IODEF_DATA_TYPE_FLOAT ) {
                        s << iodef_data_get_float(d);
                        return s.str();
                }

                else if ( t == IODEF_DATA_TYPE_INT ) {
                        s << iodef_data_get_int(d);
                        return s.str();
                }

                else {
                        s << "Left value doesn't fit 'data' type '" << t << "' requirement";
                        throw PreludeError(s.str());
                }
        }

        s << "Left value doesn't fit '" << iodef_value_type_to_string((iodef_value_type_id_t) getType()) << "' requirement";
        throw PreludeError(s.str());
}


IODEFValue &IODEFValue::operator=(const IODEFValue &p)
{
        if ( this != &p && _value != p._value ) {
                if ( _value )
                        iodef_value_destroy(_value);

                _value = (p._value) ? iodef_value_ref(p._value) : NULL;
        }

        return *this;
}


IODEFValue::operator const char*() const
{
        static std::string t;
        t = this->convert_string();
        return t.c_str();
}


IODEFValue::operator iodef_value_t *() const
{
        return _value;
}


bool IODEFValue::operator <= (const IODEFValue &value) const
{
        return this->match(value, IODEFCriterion::OPERATOR_LESSER|IODEFCriterion::OPERATOR_EQUAL);
}


bool IODEFValue::operator >= (const IODEFValue &value) const
{
        return this->match(value, IODEFCriterion::OPERATOR_GREATER|IODEFCriterion::OPERATOR_EQUAL);
}


bool IODEFValue::operator < (const IODEFValue &value) const
{
        return this->match(value, IODEFCriterion::OPERATOR_LESSER);
}


bool IODEFValue::operator > (const IODEFValue &value) const
{
        return this->match(value, IODEFCriterion::OPERATOR_GREATER);
}


bool IODEFValue::operator == (const IODEFValue &value) const
{
        return this->match(value, IODEFCriterion::OPERATOR_EQUAL);
}


bool IODEFValue::operator == (const std::vector<IODEFValue> &vlist) const
{
        return this->match(vlist, IODEFCriterion::OPERATOR_EQUAL);
}


bool IODEFValue::operator != (const IODEFValue &value) const
{
        return this->match(value, IODEFCriterion::OPERATOR_NOT|IODEFCriterion::OPERATOR_EQUAL);
}
