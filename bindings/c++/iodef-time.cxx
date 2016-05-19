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

#include "prelude-error.hxx"
#include "iodef-time.hxx"

using namespace Iodef;


IODEFTime::~IODEFTime()
{
        if ( _time )
                iodef_time_destroy(_time);
}


IODEFTime::IODEFTime(const IODEFTime &time)
{
        _time = (time._time) ? iodef_time_ref(time._time) : NULL;
}


IODEFTime::IODEFTime()
{
        int ret;

        ret = iodef_time_new_from_gettimeofday(&_time);
        if ( ret < 0 )
                throw PreludeError(ret);

}


IODEFTime::IODEFTime(iodef_time_t *time)
{
        _time = time;
}


IODEFTime::IODEFTime(time_t time)
{
        int ret;

        ret = iodef_time_new_from_time(&_time, &time);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFTime::IODEFTime(const char *str)
{
        int ret;

        ret = iodef_time_new_from_string(&_time, str);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFTime::IODEFTime(const struct timeval *tv)
{
        int ret;

        ret = iodef_time_new_from_timeval(&_time, tv);
        if ( ret < 0 )
                throw PreludeError(ret);
}



void IODEFTime::set(const time_t *t)
{
        iodef_time_set_from_time(_time, t);
}


void IODEFTime::set(const struct timeval *tv)
{
        int ret;

        ret = iodef_time_set_from_timeval(_time, tv);
        if ( ret < 0 )
                throw PreludeError(ret);
}



void IODEFTime::set(const char *str)
{
        int ret;

        ret = iodef_time_set_from_string(_time, str);
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFTime::set()
{
        int ret;

        ret = iodef_time_set_from_gettimeofday(_time);
        if ( ret < 0 )
                throw PreludeError(ret);
}


void IODEFTime::setSec(uint32_t sec)
{
        iodef_time_set_sec(_time, sec);
}


void IODEFTime::setUSec(uint32_t usec)
{
        iodef_time_set_usec(_time, usec);
}


void IODEFTime::setGmtOffset(int32_t gmtoff)
{
        iodef_time_set_gmt_offset(_time, gmtoff);
}


uint32_t IODEFTime::getSec() const
{
        return iodef_time_get_sec(_time);
}


uint32_t IODEFTime::getUSec() const
{
        return iodef_time_get_usec(_time);
}


int32_t IODEFTime::getGmtOffset() const
{
        return iodef_time_get_gmt_offset(_time);
}


IODEFTime IODEFTime::clone() const
{
        int ret;
        iodef_time_t *clone;

        ret = iodef_time_clone(_time, &clone);
        if ( ret < 0 )
                throw PreludeError(ret);

        return IODEFTime(clone);
}


const std::string IODEFTime::toString() const
{
        int ret;
        std::string cs;
        prelude_string_t *str = NULL;

        ret = prelude_string_new(&str);
        if ( ret < 0 )
                throw PreludeError(ret);

        ret = iodef_time_to_string(_time, str);
        if ( ret < 0 )
                throw PreludeError(ret);

        cs = prelude_string_get_string(str);
        prelude_string_destroy(str);

        return cs;
}


bool IODEFTime::operator <= (const IODEFTime &time) const
{
        return ( (double) *this <= (double) time );
}


bool IODEFTime::operator < (const IODEFTime &time) const
{
        return ( (double) *this < (double) time );
}


bool IODEFTime::operator >= (const IODEFTime &time) const
{
        return ( (double) *this >= (double) time );
}


bool IODEFTime::operator > (const IODEFTime &time) const
{
        return ( (double) *this > (double) time );
}


bool IODEFTime::operator != (const IODEFTime &time) const
{
        return ( (double) *this != (double) time );
}


bool IODEFTime::operator == (const IODEFTime &time) const
{
        return ( (double) *this == (double) time );
}


IODEFTime::operator int() const
{
        return getSec();
}


IODEFTime::operator long() const
{
        return getSec();
}


IODEFTime::operator double() const
{
        return getSec() + (getUSec() * 1e-6);
}


IODEFTime::operator const std::string() const
{
        return toString();
}


IODEFTime::operator iodef_time_t *() const
{
        return _time;
}


IODEFTime &IODEFTime::operator=(const IODEFTime &time)
{
        if ( this != &time && _time != time._time ) {
                if ( _time )
                        iodef_time_destroy(_time);

                _time = (time._time) ? iodef_time_ref(time._time) : NULL;
        }

        return *this;
}

