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
#include "iodef-criteria.hxx"
#include "prelude-error.hxx"

using namespace Iodef;

IODEFCriteria::~IODEFCriteria()
{
        if ( _criteria )
                iodef_criteria_destroy(_criteria);
}


IODEFCriteria::IODEFCriteria(const IODEFCriteria &criteria)
{
        _criteria = (criteria._criteria) ? iodef_criteria_ref(criteria._criteria) : NULL;
}


IODEFCriteria::IODEFCriteria(iodef_criteria_t *criteria)
{
        _criteria = criteria;
}


IODEFCriteria::IODEFCriteria(const char *criteria)
{
        int ret;

        ret = iodef_criteria_new_from_string(&_criteria, criteria);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFCriteria::IODEFCriteria(const std::string &criteria)
{
        int ret;

        ret = iodef_criteria_new_from_string(&_criteria, criteria.c_str());
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFCriteria::IODEFCriteria()
{
        int ret;

        ret = iodef_criteria_new(&_criteria);
        if ( ret < 0 )
                throw PreludeError(ret);
}


IODEFCriteria IODEFCriteria::clone() const
{
        int ret;
        iodef_criteria_t *cl;

        ret = iodef_criteria_clone(this->_criteria, &cl);
        if ( ret < 0 )
                throw PreludeError(ret);

        return IODEFCriteria(cl);
}


void IODEFCriteria::andCriteria(const IODEFCriteria &criteria)
{
        iodef_criteria_and_criteria(this->_criteria, criteria._criteria);
}


void IODEFCriteria::orCriteria(const IODEFCriteria &criteria)
{
        iodef_criteria_or_criteria(this->_criteria, criteria._criteria);
}


int IODEFCriteria::match(IODEF *message) const
{
        int ret;

        ret = iodef_criteria_match(this->_criteria, *message);
        if ( ret < 0 )
                throw PreludeError(ret);

        return ret;
}


const std::string IODEFCriteria::toString() const
{
        int ret;
        std::string s;
        prelude_string_t *str;

        ret = prelude_string_new(&str);
        if ( ret < 0 )
                throw PreludeError(ret);

        ret = iodef_criteria_to_string(_criteria, str);
        if ( ret < 0 ) {
                prelude_string_destroy(str);
                throw PreludeError(ret);
        }

        s = prelude_string_get_string(str);
        prelude_string_destroy(str);

        return s;
}


IODEFCriteria::operator const std::string() const
{
        return toString();
}



IODEFCriteria::operator iodef_criteria_t *() const
{
        return _criteria;
}



IODEFCriteria &IODEFCriteria::operator=(const IODEFCriteria &criteria)
{
        if ( this != &criteria && _criteria != criteria._criteria ) {
                if ( _criteria )
                        iodef_criteria_destroy(_criteria);

                _criteria = (criteria._criteria) ? iodef_criteria_ref(criteria._criteria) : NULL;
        }

        return *this;
}
