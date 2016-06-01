/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@libiodef-ids.com>
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

#if defined(SWIGPYTHON) || defined(SWIGLUA)
%module iodef
#else
%module Iodef
#endif

%warnfilter(509);
%feature("nothread", "1");

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%{
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <list>
#include <sstream>

#include "libiodef.hxx"
#include "libiodef-error.hxx"
#include "iodef-criteria.hxx"
#include "iodef-value.hxx"
#include "iodef-path.hxx"
#include "iodef-time.hxx"
#include "iodef.hxx"

using namespace Iodef;
%}


typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int64_t;
typedef unsigned long long uint64_t;

%ignore libiodef_error_t;
typedef signed int libiodef_error_t;

%ignore iodef_class_id_t;
typedef int iodef_class_id_t;

typedef long long time_t;


%exception {
        try {
                $action
        } catch(Iodef::LibIodefError &e) {
                SWIG_exception(SWIG_RuntimeError, e.what());
                SWIG_fail;
        }
}


#ifdef SWIGPERL
%include perl/libiodefcpp-perl.i
#endif

#ifdef SWIGPYTHON
%include python/libiodefcpp-python.i
#endif

#ifdef SWIGRUBY
%include ruby/libiodefcpp-ruby.i
#endif

#ifdef SWIGLUA
%include lua/libiodefcpp-lua.i
#endif


%ignore operator <<(std::ostream &os, const Iodef::IODEF &iodef);
%ignore operator >>(std::istream &is, const Iodef::IODEF &iodef);


%template() std::vector<std::string>;
%template() std::vector<Iodef::IODEF>;
%template() std::vector<Iodef::IODEFValue>;
%template() std::vector<Iodef::Connection>;

#ifdef SWIG_COMPILE_LIBIODEF
%extend Iodef::IODEF {
        Iodef::IODEFValue get(const char *path) {
                Iodef::IODEFValue value;
                Iodef::IODEFPath ipath = Iodef::IODEFPath(*self, path);

                value = ipath.get(*self);
                if ( value.isNull() && ipath.isAmbiguous() ) {
                        std::vector<Iodef::IODEFValue> v;
                        return Iodef::IODEFValue(v);
                }

                return value;
        }
}

%extend Iodef::IODEFPath {
        Iodef::IODEFValue get(Iodef::IODEF &message) {
                Iodef::IODEFValue value;

                value = self->get(message);
                if ( value.isNull() && self->isAmbiguous() ) {
                        std::vector<Iodef::IODEFValue> v;
                        return Iodef::IODEFValue(v);
                }

                return value;
        }
}
#endif

%ignore Iodef::IODEF::get;
%ignore Iodef::IODEFPath::get;


%fragment("SWIG_FromBytePtrAndSize", "header", fragment="SWIG_FromCharPtrAndSize") %{
#ifndef SWIG_FromBytePtrAndSize
# define SWIG_FromBytePtrAndSize(arg, len) SWIG_FromCharPtrAndSize(arg, len)
#endif
%}


%fragment("IODEFValue_to_SWIG", "header", fragment="SWIG_From_double",
                                          fragment="SWIG_From_float",
                                          fragment="SWIG_From_int", fragment="SWIG_From_unsigned_SS_int",
                                          fragment="SWIG_From_long_SS_long", fragment="SWIG_From_unsigned_SS_long_SS_long",
                                          fragment="SWIG_FromCharPtr", fragment="SWIG_FromCharPtrAndSize", fragment="SWIG_FromBytePtrAndSize",
                                          fragment="IODEFValueList_to_SWIG") {

int IODEFValue_to_SWIG(TARGET_LANGUAGE_SELF self, const Iodef::IODEFValue &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret)
{
        iodef_value_t *value = result;
        Iodef::IODEFValue::IODEFValueTypeEnum type = result.getType();

        if ( type == Iodef::IODEFValue::TYPE_STRING ) {
                libiodef_string_t *str = iodef_value_get_string(value);
                *ret = SWIG_FromCharPtrAndSize(libiodef_string_get_string(str), libiodef_string_get_len(str));
        }

        else if ( type == Iodef::IODEFValue::TYPE_INT8 )
                *ret = SWIG_From_int(iodef_value_get_int8(value));

        else if ( type == Iodef::IODEFValue::TYPE_UINT8 )
                *ret = SWIG_From_unsigned_SS_int(iodef_value_get_uint8(value));

        else if ( type == Iodef::IODEFValue::TYPE_INT16 )
                *ret = SWIG_From_int(iodef_value_get_int16(value));

        else if ( type == Iodef::IODEFValue::TYPE_UINT16 )
                *ret = SWIG_From_unsigned_SS_int(iodef_value_get_uint16(value));

        else if ( type == Iodef::IODEFValue::TYPE_INT32 )
                *ret = SWIG_From_int(iodef_value_get_int32(value));

        else if ( type == Iodef::IODEFValue::TYPE_UINT32 )
                *ret = SWIG_From_unsigned_SS_int(iodef_value_get_uint32(value));

        else if ( type == Iodef::IODEFValue::TYPE_INT64 )
                *ret = SWIG_From_long_SS_long(iodef_value_get_int64(value));

        else if ( type == Iodef::IODEFValue::TYPE_UINT64 )
                *ret = SWIG_From_unsigned_SS_long_SS_long(iodef_value_get_uint64(value));

        else if ( type == Iodef::IODEFValue::TYPE_FLOAT )
                *ret = SWIG_From_float(iodef_value_get_float(value));

        else if ( type == Iodef::IODEFValue::TYPE_DOUBLE )
                *ret = SWIG_From_double(iodef_value_get_double(value));

        else if ( type == Iodef::IODEFValue::TYPE_ENUM ) {
                const char *s = iodef_class_enum_to_string(iodef_value_get_class(value), iodef_value_get_enum(value));
                *ret = SWIG_FromCharPtr(s);
        }

        else if ( type == Iodef::IODEFValue::TYPE_TIME ) {
                Iodef::IODEFTime t = result;
                *ret = SWIG_NewPointerObj(new Iodef::IODEFTime(t), $descriptor(Iodef::IODEFTime *), 1);
        }

        else if ( type == Iodef::IODEFValue::TYPE_LIST )
                *ret = IODEFValueList_to_SWIG(self, result, extra);

        else if ( type == Iodef::IODEFValue::TYPE_DATA ) {
                iodef_data_t *d = iodef_value_get_data(value);
                iodef_data_type_t t = iodef_data_get_type(d);

                if ( t == IODEF_DATA_TYPE_BYTE || t == IODEF_DATA_TYPE_BYTE_STRING )
                        *ret = SWIG_FromBytePtrAndSize((const char *)iodef_data_get_data(d), iodef_data_get_len(d));

                else if ( t == IODEF_DATA_TYPE_CHAR )
                        *ret = SWIG_FromCharPtrAndSize((const char *)iodef_data_get_data(d), iodef_data_get_len(d));

                else if ( t == IODEF_DATA_TYPE_CHAR_STRING )
                        *ret = SWIG_FromCharPtrAndSize((const char *)iodef_data_get_data(d), iodef_data_get_len(d) - 1);

                else if ( t == IODEF_DATA_TYPE_FLOAT )
                        *ret = SWIG_From_float(iodef_data_get_float(d));

                else if ( t == IODEF_DATA_TYPE_UINT32 || IODEF_DATA_TYPE_INT )
                        *ret = SWIG_From_unsigned_SS_long_SS_long(iodef_data_get_int(d));
        }

        else if ( type == Iodef::IODEFValue::TYPE_CLASS ) {
                iodef_object_t *obj = (iodef_object_t *) iodef_value_get_object(value);
                *ret = SWIG_NewPointerObj(new Iodef::IODEF(iodef_object_ref(obj)), $descriptor(Iodef::IODEF *), 1);
        }

        else return -1;

        return 1;
}
}

%ignore Iodef::IODEFValue::operator const char*() const;
%ignore Iodef::IODEFValue::operator std::vector<IODEFValue>() const;
%ignore Iodef::IODEFValue::operator Iodef::IODEFTime() const;
%ignore Iodef::IODEFValue::operator int8_t() const;
%ignore Iodef::IODEFValue::operator uint8_t() const;
%ignore Iodef::IODEFValue::operator int16_t() const;
%ignore Iodef::IODEFValue::operator uint16_t() const;
%ignore Iodef::IODEFValue::operator int32_t() const;
%ignore Iodef::IODEFValue::operator uint32_t() const;
%ignore Iodef::IODEFValue::operator int64_t() const;
%ignore Iodef::IODEFValue::operator uint64_t() const;
%ignore Iodef::IODEFValue::operator float() const;
%ignore Iodef::IODEFValue::operator double() const;

/*
 * Force SWIG to use the IODEFValue * version of the Set() function,
 * so that the user might provide NULL IODEFValue. Force usage of the
 * std::string value, for binary data.
 */
%ignore Iodef::IODEF::set(char const *, int8_t);
%ignore Iodef::IODEF::set(char const *, uint8_t);
%ignore Iodef::IODEF::set(char const *, int16_t);
%ignore Iodef::IODEF::set(char const *, uint16_t);
%ignore Iodef::IODEF::set(char const *, char const *);
%ignore Iodef::IODEF::set(char const *, Iodef::IODEFValue &value);
%ignore Iodef::IODEFPath::set(Iodef::IODEF &, int8_t) const;
%ignore Iodef::IODEFPath::set(Iodef::IODEF &, uint8_t) const;
%ignore Iodef::IODEFPath::set(Iodef::IODEF &, int16_t) const;
%ignore Iodef::IODEFPath::set(Iodef::IODEF &, uint16_t) const;
%ignore Iodef::IODEFPath::set(Iodef::IODEF &, char const *) const;
%ignore Iodef::IODEFPath::set(Iodef::IODEF &, Iodef::IODEFValue &) const;
%ignore Iodef::IODEFValue::IODEFValue(char const *);

%ignore iodef_path_t;
%ignore iodef_object_t;
%ignore iodef_criteria_t;
%ignore operator iodef_path_t *() const;
%ignore operator iodef_criteria_t *() const;
%ignore operator iodef_object_t *() const;
%ignore operator iodef_document_t *() const;
%ignore operator iodef_time_t *() const;
%ignore operator iodef_value_t *() const;

/*
 * We need to unlock the interpreter lock before calling certain methods
 * because they might acquire internal libiodef mutex that may also be
 * acquired undirectly through the libiodef asynchronous stack.
 *
 * [Thread 2]: Liblibiodef async stack
 * -> Lock internal mutexX
 *    -> libiodef_log()
 *       -> SWIG/C log callback
 *          -> Wait on Interpreter lock [DEADLOCK]
 *             -> Python logging callback (never called)
 *
 * [Thread 1] ConnectionPool::Recv()
 *  -> Acquire Interpreter lock
 *      *** At this time, thread 2 lock internal mutexX
 *      -> Wait on internal mutexX [DEADLOCK]
 *
 * In this situation, [Thread 1] hold the Interpreter lock and is
 * waiting on mutexX, which itself cannot be released by [Thread 2]
 * until [Thread 1] unlock the Interpreter lock.
 *
 * One rule to prevent deadlock is to always acquire mutex in the same
 * order. We thus need to make sure the interpreter lock is released
 * before calling C++ method that are susceptible to lock a mutex that
 * is also triggered from the asynchronous interface.
 *
 * Note that we are not releasing the Interpreter lock in all C++ call,
 * because it come at a performance cost, so we only try to do it when
 * needed.
 */

#ifdef SWIG_COMPILE_LIBIODEF
%feature("exceptionclass") Iodef::LibIodefError;
%feature("kwargs") Iodef::IODEFClass::getPath;
%feature("kwargs") Iodef::IODEFPath::getClass;
%feature("kwargs") Iodef::IODEFPath::getValueType;
%feature("kwargs") Iodef::IODEFPath::setIndex;
%feature("kwargs") Iodef::IODEFPath::getIndex;
%feature("kwargs") Iodef::IODEFPath::undefineIndex;
%feature("kwargs") Iodef::IODEFPath::compare;
%feature("kwargs") Iodef::IODEFPath::getName;
%feature("kwargs") Iodef::IODEFPath::isList;

%include libiodef.hxx
%include libiodef-error.hxx
%include iodef-criteria.hxx
%include iodef-value.hxx
%include iodef-path.hxx
%include iodef-time.hxx
%include iodef-class.hxx
%include iodef.hxx
#endif

