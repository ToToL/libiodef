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
#define TARGET_LANGUAGE_SELF PyObject *
#define TARGET_LANGUAGE_OUTPUT_TYPE PyObject **
%}

%include std_list.i

%ignore *::operator int;
%ignore *::operator long;
%ignore *::operator double;
%ignore *::operator const char *;
%ignore *::operator const std::string;
%ignore *::operator ();

%feature("python:slot", "tp_str", functype="reprfunc") *::what;
%feature("python:slot", "tp_repr", functype="reprfunc") *::toString;
%feature("python:slot", "mp_subscript", functype="binaryfunc") *::get;
%feature("python:slot", "mp_ass_subscript", functype="objobjargproc") *::set;
%feature("python:slot", "tp_hash") Iodef::IODEFValue::getType;

/*
 * IODEFClass
 */
%feature("python:slot", "tp_str", functype="reprfunc") Iodef::IODEFClass::getName;
%feature("python:slot", "sq_item", functype="ssizeargfunc") Iodef::IODEFClass::_get2;
%feature("python:slot", "mp_subscript", functype="binaryfunc") Iodef::IODEFClass::get;
%feature("python:slot", "mp_length", functype="lenfunc") Iodef::IODEFClass::getChildCount;

/*
 * IODEFTime
 */
%feature("python:slot", "nb_int", functype="unaryfunc") Iodef::IODEFTime::getSec;
%feature("python:slot", "nb_long", functype="unaryfunc") Iodef::IODEFTime::_getSec2;
%feature("python:slot", "nb_float", functype="unaryfunc") Iodef::IODEFTime::getTime;

/*
 *
 */
%feature("python:slot", "nb_lshift") Iodef::IODEF::readExcept;
%feature("python:slot", "nb_rshift") Iodef::IODEF::write;


%ignore *::operator =;


%fragment("SWIG_FromBytePtrAndSize", "header", fragment="SWIG_FromCharPtrAndSize") %{
#if PY_VERSION_HEX < 0x03000000
# define SWIG_FromBytePtrAndSize(arg, len) PyString_FromStringAndSize(arg, len)
#else
# define SWIG_FromBytePtrAndSize(arg, len) PyBytes_FromStringAndSize(arg, len)
#endif
%}

/* tell squid not to cast void * value */
%typemap(in) void *nocast_file_p %{
#if PY_VERSION_HEX < 0x03000000
        if ( !PyFile_Check((PyObject *) $input) ) {
                const char *errstr = "Argument is not a file object.";
                PyErr_SetString(PyExc_RuntimeError, errstr);
                return NULL;
        }
#else
        extern PyTypeObject PyIOBase_Type;
        if ( ! PyObject_IsInstance((PyObject *) $input, (PyObject *) &PyIOBase_Type) ) {
                const char *errstr = "Argument is not a file object.";
                PyErr_SetString(PyExc_RuntimeError, errstr);
                return NULL;
        }
#endif

        $1 = $input;
%}


%exception {
        try {
                $action
        } catch(Iodef::LibIodefError &e) {
                SWIG_Python_Raise(SWIG_NewPointerObj(new LibIodefError(e),
                                                     SWIGTYPE_p_Iodef__LibIodefError, SWIG_POINTER_OWN),
                                  "LibIodefError", SWIGTYPE_p_Iodef__LibIodefError);
                SWIG_fail;
        }
}


#ifdef SWIG_COMPILE_LIBIODEF

%{
typedef PyObject SwigPyObjectState;
%}

/*
 * This is called on Iodef::IODEF::__getstate__()
 * Store our internal IODEF data in the PyObjet __dict__
 */
%typemap(out) SwigPyObjectState * {
        int ret;
        PyObject *state;
        SwigPyObject *pyobj = (SwigPyObject *) self;

        state = PyDict_New();
        if ( ! state ) {
                Py_XDECREF(result);
                SWIG_fail;
        }

        ret = PyDict_SetItemString(state, "__iodef_data__", result);
        Py_DECREF(result);

        if ( pyobj->dict ) {
                ret = PyDict_Update(state, pyobj->dict);
                if ( ret < 0 ) {
                        Py_XDECREF(state);
                        SWIG_fail;
                }
        }

        if ( ret < 0 )
                throw LibIodefError("error setting internal __iodef_data__ key");

        $result = state;
}

/*
 * This typemap specifically intercept the call to Iodef::IODEF::__setstate__,
 * since at that time (when unpickling), the object __init__ method has not been
 * called, and the underlying Iodef::IODEF object is thus NULL.
 *
 * We manually call tp_init() to handle the underlying object creation here.
 */
%typemap(arginit) (PyObject *state) {
        int ret;
        PyObject *obj;
        static PyTypeObject *pytype = NULL;

        if ( ! pytype ) {
                swig_type_info *sti = SWIG_TypeQuery("Iodef::IODEF *");
                if ( ! sti )
                        throw LibIodefError("could not find type SWIG type info for 'Iodef::IODEF'");

                pytype = ((SwigPyClientData *) sti->clientdata)->pytype;
        }

        obj = PyTuple_New(0);
        ret = pytype->tp_init(self, obj, NULL);
        Py_DECREF(obj);

        if ( ret < 0 )
                throw LibIodefError("error calling Iodef::IODEF tp_init()");

}


/*
 *
 */
%exception Iodef::IODEF::__setstate__ {
        try {
                SwigPyObject *pyobj = (SwigPyObject *) self;

                $function

                /*
                 * This is called at unpickling time, and set our PyObject internal dict.
                 */
                pyobj->dict = arg2;
                Py_INCREF(arg2);
        } catch(Iodef::LibIodefError &e) {
                SWIG_Python_Raise(SWIG_NewPointerObj(new LibIodefError(e),
                                                     SWIGTYPE_p_Iodef__LibIodefError, SWIG_POINTER_OWN),
                                  "LibIodefError", SWIGTYPE_p_Iodef__LibIodefError);
                SWIG_fail;
        }
}


/*
 * When a comparison operator is called, this prevent an exception
 * if the compared operand does not have the correct datatype.
 *
 * By returning Py_NotImplemented, the python code might provide its
 * own comparison method within the compared operand class
 */
%typemap(in) (const Iodef::IODEFTime &time) {
        int ret;
        void *obj;

        ret = SWIG_ConvertPtr($input, &obj, $descriptor(Iodef::IODEFTime *),  0  | 0);
        if ( ! SWIG_IsOK(ret) || ! obj ) {
                Py_INCREF(Py_NotImplemented);
                return Py_NotImplemented;
        }

        $1 = reinterpret_cast< Iodef::IODEFTime * >(obj);
}


/*
 * Workaround SWIG %features bug, which prevent us from applying multiple
 * features to the same method.
 */
%extend Iodef::IODEFTime {
        long _getSec2(void) {
                return self->getSec();
        }
}

%exception Iodef::IODEFClass::_get2 {
        try {
                $action;
        } catch(Iodef::LibIodefError &e) {
                if ( e.getCode() == LIBIODEF_ERROR_IODEF_CLASS_UNKNOWN_CHILD ||
                     e.getCode() == LIBIODEF_ERROR_IODEF_PATH_DEPTH )
                        SWIG_exception_fail(SWIG_IndexError, e.what());
        }
}

%extend Iodef::IODEFClass {
        Iodef::IODEFClass _get2(int i) {
                return self->get(i);
        }
}
#endif

%fragment("IODEFValueList_to_SWIG", "header", fragment="IODEFValue_to_SWIG") {
int IODEFValue_to_SWIG(TARGET_LANGUAGE_SELF self, const Iodef::IODEFValue &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret);

PyObject *IODEFValueList_to_SWIG(TARGET_LANGUAGE_SELF self, const Iodef::IODEFValue &value, void *extra)
{
        int j = 0, ret;
        PyObject *pytuple;
        std::vector<Iodef::IODEFValue> result = value;
        std::vector<Iodef::IODEFValue>::const_iterator i;

        pytuple = PyTuple_New(result.size());

        for ( i = result.begin(); i != result.end(); i++ ) {
                PyObject *val;

                if ( (*i).isNull() ) {
                        Py_INCREF(Py_None);
                        val = Py_None;
                } else {
                        ret = IODEFValue_to_SWIG(self, *i, NULL, &val);
                        if ( ret < 0 )
                                return NULL;
                }

                PyTuple_SetItem(pytuple, j++, val);
        }

        return pytuple;
}
}


%typemap(out, fragment="IODEFValue_to_SWIG") Iodef::IODEFValue {
        int ret;

        if ( $1.isNull() ) {
                Py_INCREF(Py_None);
                $result = Py_None;
        } else {
#ifdef SWIGPYTHON_BUILTIN
                ret = IODEFValue_to_SWIG(self, $1, NULL, &$result);
#else
                ret = IODEFValue_to_SWIG(NULL, $1, NULL, &$result);
#endif
                if ( ret < 0 ) {
                        std::string s = "IODEFValue typemap does not handle value of type '";
                        s += iodef_value_type_to_string((iodef_value_type_id_t) $1.getType());
                        s += "'";
                        SWIG_exception_fail(SWIG_ValueError, s.c_str());
                }
        }
};



