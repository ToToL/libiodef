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
#include "../../libmissing/glthread/thread.h"
%}

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
%ignore *::operator const std::string() const;
%ignore *::operator const char *() const;


%header %{
#define TARGET_LANGUAGE_SELF SV *
#define TARGET_LANGUAGE_OUTPUT_TYPE SV **
%}

%fragment("IODEFValueList_to_SWIG", "header") {
int IODEFValue_to_SWIG(TARGET_LANGUAGE_SELF self, const LibIodef::IODEFValue &result, void *extra, TARGET_LANGUAGE_OUTPUT_TYPE ret);

SV *IODEFValueList_to_SWIG(TARGET_LANGUAGE_SELF self, const LibIodef::IODEFValue &value, void *extra)
{
        int j = 0, ret;
        std::vector<LibIodef::IODEFValue> result = value;
        std::vector<LibIodef::IODEFValue>::const_iterator i;

        AV *myav;
        SV *svret, **svs = new SV*[result.size()];

        for ( i = result.begin(); i != result.end(); i++ ) {
                if ( (*i).isNull() ) {
                        SvREFCNT_inc(& PL_sv_undef);
                        svs[j++] = &PL_sv_undef;
                } else {
                        ret = IODEFValue_to_SWIG(self, *i, NULL, &svs[j++]);
                        if ( ret < 0 )
                                return NULL;
                }
        }

        myav = av_make(result.size(), svs);
        delete[] svs;

        svret = newRV_noinc((SV*) myav);
        sv_2mortal(svret);

        return svret;
}
}

/* tell squid not to cast void * value */
%typemap(in) void *nocast_p {
        $1 = $input;
}

%fragment("TransitionFunc", "header") {
static SV *__libiodef_log_func;
static gl_thread_t __initial_thread;


static void _cb_perl_log(int level, const char *str)
{
        if ( (gl_thread_t) gl_thread_self() != __initial_thread )
                return;

        dSP;
        ENTER;
        SAVETMPS;

        PUSHMARK(SP);
        XPUSHs(SWIG_From_int(level));
        XPUSHs(SWIG_FromCharPtr(str));
        PUTBACK;

        perl_call_sv(__libiodef_log_func, G_VOID);

        FREETMPS;
        LEAVE;
}


static int _cb_perl_write(libiodef_msgbuf_t *fd, libiodef_msg_t *msg)
{
        ssize_t ret;
        PerlIO *io = (PerlIO *) libiodef_msgbuf_get_data(fd);

        ret = PerlIO_write(io, (const char *) libiodef_msg_get_message_data(msg), libiodef_msg_get_len(msg));
        if ( ret != libiodef_msg_get_len(msg) )
                return libiodef_error_from_errno(errno);

        libiodef_msg_recycle(msg);

        return 0;
}


static ssize_t _cb_perl_read(libiodef_io_t *fd, void *buf, size_t size)
{
        int ret;
        PerlIO *io = (PerlIO *) libiodef_io_get_fdptr(fd);

        ret = PerlIO_read(io, buf, size);
        if ( ret < 0 )
                ret = libiodef_error_from_errno(errno);

        else if ( ret == 0 )
                ret = libiodef_error(LIBIODEF_ERROR_EOF);

        return ret;
}
};

%typemap(in, fragment="TransitionFunc") void (*log_cb)(int level, const char *log) {
        if ( __libiodef_log_func )
                SvREFCNT_dec(__libiodef_log_func);

        __libiodef_log_func = $input;
        SvREFCNT_inc($input);

        $1 = _cb_perl_log;
};


%exception read(void *nocast_p) {
        try {
                $action
        } catch(LibIodef::LibIodefError &e) {
                if ( e.getCode() == LIBIODEF_ERROR_EOF )
                        result = 0;
                else
                        SWIG_exception_fail(SWIG_RuntimeError, e.what());
        }
}


%extend LibIodef::IODEF {
        void write(void *nocast_p) {
                PerlIO *io = IoIFP(sv_2io((SV *) nocast_p));
                self->_genericWrite(_cb_perl_write, io);
        }

        int read(void *nocast_p) {
                PerlIO *io = IoIFP(sv_2io((SV *) nocast_p));
                self->_genericRead(_cb_perl_read, io);
                return 1;
        }
}


%typemap(out, fragment="IODEFValue_to_SWIG") LibIodef::IODEFValue {
        int ret;

        if ( $1.isNull() ) {
                SvREFCNT_inc (& PL_sv_undef);
                $result = &PL_sv_undef;
        } else {
                SV *mysv;

                ret = IODEFValue_to_SWIG(NULL, $1, NULL, &mysv);
                if ( ret < 0 ) {
                        std::stringstream s;
                        s << "IODEFValue typemap does not handle value of type '" << iodef_value_type_to_string((iodef_value_type_id_t) $1.getType()) << "'";
                        SWIG_exception_fail(SWIG_ValueError, s.str().c_str());
                }

                $result = mysv;
        }

        argvi++;
};


%init {
        STRLEN len;
        char **argv;
        int j, argc = 1, ret;
        AV *pargv = get_av("ARGV", FALSE);

        __initial_thread = (gl_thread_t) gl_thread_self();

        ret = av_len(pargv);
        if ( ret >= 0 )
                argc += ret + 1;

        if ( argc + 1 < 0 )
                throw LibIodefError("Invalide argc length");

        argv = (char **) malloc((argc + 1) * sizeof(char *));
        if ( ! argv )
                throw LibIodefError("Allocation failure");

        argv[0] = SvPV(get_sv("0", FALSE), len);

        for ( j = 0; j < ret + 1; j++ )
                argv[j + 1] = SvPV(*av_fetch(pargv, j, FALSE), len);

        argv[j + 1] = NULL;

        ret = libiodef_init(&argc, argv);
        if ( ret < 0 ) {
                free(argv);
                throw LibIodefError(ret);
        }

        free(argv);
}
