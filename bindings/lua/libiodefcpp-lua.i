/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
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

%begin %{
#include "config.h"
#include "glthread/thread.h"
%}

/*
 * Lua overloading fixes
 */
%ignore IODEFCriteria(std::string const &);
%ignore IODEFValue(int8_t);
%ignore IODEFValue(uint8_t);
%ignore IODEFValue(int16_t);
%ignore IODEFValue(uint16_t);
%ignore IODEFValue(int32_t);
%ignore IODEFValue(uint32_t);
%ignore IODEFValue(int64_t);
%ignore IODEFValue(uint64_t);
%ignore IODEFValue(float);
%ignore IODEFValue(std::string);
%ignore Set(LibIodef::IODEF &, int8_t);
%ignore Set(LibIodef::IODEF &, uint8_t);
%ignore Set(LibIodef::IODEF &, int16_t);
%ignore Set(LibIodef::IODEF &, uint16_t);
%ignore Set(LibIodef::IODEF &, int32_t);
%ignore Set(LibIodef::IODEF &, uint32_t);
%ignore Set(LibIodef::IODEF &, int64_t);
%ignore Set(LibIodef::IODEF &, uint64_t);
%ignore Set(LibIodef::IODEF &, float);
%ignore Set(LibIodef::IODEF &, std::string);
%ignore Set(char const *, int8_t);
%ignore Set(char const *, uint8_t);
%ignore Set(char const *, int16_t);
%ignore Set(char const *, uint16_t);
%ignore Set(char const *, int32_t);
%ignore Set(char const *, uint32_t);
%ignore Set(char const *, int64_t);
%ignore Set(char const *, uint64_t);
%ignore Set(char const *, float);
%ignore Set(char const *, std::string);

/*
 * Conversion not allowed
 */
%ignore *::operator =;

%ignore *::operator int() const;
%ignore *::operator long() const;
%ignore *::operator int32_t() const;
%ignore *::operator uint32_t() const;
%ignore *::operator int64_t() const;
%ignore *::operator uint64_t() const;
%ignore *::operator float() const;
%ignore *::operator double() const;
%ignore *::operator LibIodef::IODEFTime() const;
%ignore operator <<;
%ignore operator >>;

%rename (__str__) *::operator const char *() const;

%begin %{
#define TARGET_LANGUAGE_SELF void *
#define TARGET_LANGUAGE_OUTPUT_TYPE int *
%}

%header {
        char *my_strdup(const char *in)
        {
                char *out = new char[strlen(in) + 1];
                strcpy(out, in);
                return out;
        }
}

%ignore LibIodef::IODEFCriteria::operator const std::string() const;
%newobject LibIodef::IODEFCriteria::__str__;
%extend LibIodef::IODEFCriteria {
        char *__str__() { return my_strdup(self->toString().c_str()); }
}

%ignore LibIodef::IODEFTime::operator const std::string() const;
%newobject LibIodef::IODEFTime::__str__;
%extend LibIodef::IODEFTime {
        char *__str__() { return my_strdup(self->toString().c_str()); }
}

%ignore LibIodef::IODEF::operator const std::string() const;
%newobject LibIodef::IODEF::__str__;
%extend LibIodef::IODEF {
        char *__str__() { return my_strdup(self->toString().c_str()); }
}



%header %{
int integer2lua(lua_State *L, lua_Number result)
{
        lua_pushnumber(L, (lua_Number) result);
        return 0;
}

int str2lua(lua_State *L, const char *str)
{
        lua_pushstring(L, str);
        return 0;
}

int strsize2lua(lua_State *L, const char *str, size_t len)
{
        lua_pushlstring(L, str, len);
        return 0;
}


int _SWIG_LibIodef_NewPointerObj(lua_State *L, void *obj, swig_type_info *objtype, int val)
{
        SWIG_NewPointerObj(L, obj, objtype, val);
        return 0;
}
%}

/* tell squid not to cast void * value */
%typemap(in) void *nocast_p {
        FILE **pf;
        pf = (FILE **)lua_touserdata(L, $input);
        if (pf == NULL) {
                lua_pushstring(L,"Argument is not a file");
                SWIG_fail;
        }
        $1 = *pf;
}

%fragment("TransitionFunc", "header") {
static int __libiodef_log_func;
static lua_State *__lua_state = NULL;
static gl_thread_t __initial_thread;

static void _cb_lua_log(int level, const char *str)
{
        if ( (gl_thread_t) gl_thread_self() != __initial_thread )
                return;

        lua_rawgeti(__lua_state, LUA_REGISTRYINDEX, __libiodef_log_func);
        lua_pushnumber(__lua_state, level);
        lua_pushstring(__lua_state, str);
        lua_call(__lua_state, 2, 0);
}


static int _cb_lua_write(libiodef_msgbuf_t *fd, libiodef_msg_t *msg)
{
        size_t ret;
        FILE *f = (FILE *) libiodef_msgbuf_get_data(fd);

        ret = fwrite((const char *)libiodef_msg_get_message_data(msg), 1, libiodef_msg_get_len(msg), f);
        if ( ret != libiodef_msg_get_len(msg) )
                return libiodef_error_from_errno(errno);

        libiodef_msg_recycle(msg);
        return 0;
}


static ssize_t _cb_lua_read(libiodef_io_t *fd, void *buf, size_t size)
{
        ssize_t ret;
        FILE *f = (FILE *) libiodef_io_get_fdptr(fd);

        ret = fread(buf, 1, size, f);
        if ( ret < 0 )
                ret = libiodef_error_from_errno(errno);

        else if ( ret == 0 )
                ret = libiodef_error(LIBIODEF_ERROR_EOF);

        return ret;
}

};


%typemap(in, fragment="TransitionFunc") void (*log_cb)(int level, const char *log) {
        if ( ! lua_isfunction(L, -1) )
                SWIG_exception(SWIG_ValueError, "Argument should be a function");

        if ( __lua_state )
                luaL_unref(L, LUA_REGISTRYINDEX, __libiodef_log_func);

        __libiodef_log_func = luaL_ref(L, LUA_REGISTRYINDEX);
        $1 = _cb_lua_log;
        __lua_state = L;
};


%exception {
        try {
                $action
        } catch(LibIodef::LibIodefError &e) {
                SWIG_exception(SWIG_RuntimeError, e.what());
                SWIG_fail;
        }
}

%exception read(void *nocast_p) {
        try {
                $action
        } catch(LibIodef::LibIodefError &e) {
                if ( e.getCode() == LIBIODEF_ERROR_EOF )
                        return 0;
                else {
                        SWIG_exception(SWIG_RuntimeError, e.what());
                        SWIG_fail;
                }
        }
}


%extend LibIodef::IODEF {
        void write(void *nocast_p) {
                self->_genericWrite(_cb_lua_write, nocast_p);
        }


        int read(void *nocast_p) {
                self->_genericRead(_cb_lua_read, nocast_p);
                return 1;
        }
}


%fragment("SWIG_NewPointerObj", "header") {
#define SWIG_NewPointerObj(obj, objtype, val) _SWIG_LibIodef_NewPointerObj((lua_State *) extra, obj, objtype, val);
}

%fragment("SWIG_FromCharPtr", "header") {
#define SWIG_FromCharPtr(result) str2lua((lua_State *) extra, result)
}

%fragment("SWIG_FromBytePtrAndSize", "header") {
#define SWIG_FromBytePtrAndSize(result, len) strsize2lua((lua_State *) extra, result, len)
}

%fragment("SWIG_FromCharPtrAndSize", "header") {
#define SWIG_FromCharPtrAndSize(result, len) strsize2lua((lua_State *) extra, result, len)
}

%fragment("SWIG_From_float", "header") {
#define SWIG_From_float(result) integer2lua((lua_State *) extra, result)
}

%fragment("SWIG_From_int", "header") {
#define SWIG_From_int(result) integer2lua((lua_State *) extra, result)
}

%fragment("SWIG_From_double", "header") {
#define SWIG_From_double(result) integer2lua((lua_State *) extra, result)
}

%fragment("SWIG_From_unsigned_SS_int", "header") {
#define SWIG_From_unsigned_SS_int(result) integer2lua((lua_State *) extra, result)
}

%fragment("SWIG_From_long_SS_long", "header") {
#define SWIG_From_long_SS_long(result) integer2lua((lua_State *) extra, result)
}

%fragment("SWIG_From_unsigned_SS_long_SS_long", "header") {
#define SWIG_From_unsigned_SS_long_SS_long(result) integer2lua((lua_State *) extra, result)
}



%fragment("IODEFValueList_to_SWIG", "header") {
int IODEFValue_to_SWIG(TARGET_LANGUAGE_SELF self, const LibIodef::IODEFValue &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret);

int IODEFValueList_to_SWIG(TARGET_LANGUAGE_SELF self, const LibIodef::IODEFValue &value, void *extra)
{
        bool is_list;
        int index = 0, ret, unused;
        std::vector<LibIodef::IODEFValue> result = value;
        std::vector<LibIodef::IODEFValue>::const_iterator i;

        lua_newtable((lua_State *) extra);

        for ( i = result.begin(); i != result.end(); i++ ) {
                ret = lua_checkstack((lua_State *) extra, 2);
                if ( ret < 0 )
                        return ret;

                if ( (*i).isNull() ) {
                        lua_pushnil((lua_State *) extra);
                        lua_rawseti((lua_State *) extra, -2, ++index);
                } else {
                        is_list = (i->getType() == IODEFValue::TYPE_LIST);
                        if ( is_list )
                                lua_pushnumber((lua_State *) extra, ++index);

                        ret = IODEFValue_to_SWIG(&unused, *i, extra, &unused);
                        if ( ret < 0 )
                                return -1;

                        if ( is_list )
                                lua_settable((lua_State *) extra, -3);
                        else
                                lua_rawseti((lua_State *) extra, -2, ++index);
                }
        }

        return 1;
}
}

%typemap(out, fragment="IODEFValue_to_SWIG") LibIodef::IODEFValue {
        int ret, unused;

        if ( $1.isNull() ) {
                lua_pushnil(L);
                SWIG_arg = 1;
        } else {
                SWIG_arg = IODEFValue_to_SWIG(&unused, $1, L, &unused);
                if ( SWIG_arg < 0 ) {
                        std::stringstream s;
                        s << "IODEFValue typemap does not handle value of type '" << iodef_value_type_to_string((iodef_value_type_id_t) $1.getType()) << "'";
                        SWIG_exception(SWIG_ValueError, s.str().c_str());
                }
        }
};



#define MAX(x, y) ((x) > (y) ? (x) : (y))
%init {
        int argc = 0, ret;
        static char *argv[1024];

        __initial_thread = (gl_thread_t) gl_thread_self();

        lua_getglobal(L, "arg");
        if ( ! lua_istable(L, -1) )
                return;

        lua_pushnil(L);

        while ( lua_next(L, -2) != 0 ) {
                int idx;
                const char *val;

                idx = lua_tonumber(L, -2);
                val = lua_tostring(L, -1);
                lua_pop(L, 1);

                if ( idx < 0 )
                        continue;

                if ( idx >= ((sizeof(argv) / sizeof(char *)) - 1) )
                        throw LibIodefError("Argument index too large");

                argv[idx] = strdup(val);
                argc = MAX(idx, argc);
        }

        argc++;
        argv[argc] = NULL;

        ret = libiodef_init(&argc, argv);
        if ( ret < 0 )
                throw LibIodefError(ret);
}


