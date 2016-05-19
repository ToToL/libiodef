/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoannv@gmail.com>
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

#include <string>
#include <iodef.h>
#include <iodef-value-type.h>

#include "libiodef.hxx"


using namespace Iodef;



IODEFClass::IODEFClass(iodef_class_id_t id)
{
        _id = id;
        _depth = 0;
}


IODEFClass::IODEFClass(const IODEFPath &path)
{
        int i;
        IODEFClass root;

        for ( i = 0; i < path.getDepth(); i++ )
                root = root.get(path.getName(i));

        *this = root;
}



IODEFClass::IODEFClass(const std::string &path)
{
        *this = IODEFClass(IODEFPath(path.c_str()));
}


IODEFClass::IODEFClass(IODEFClass &parent, int child_id, int depth)
{
        IODEFClass::IODEFClassElem elem;

        if ( depth >= 16 )
                throw PreludeError(prelude_error(PRELUDE_ERROR_IODEF_PATH_DEPTH));

        _depth = depth;
        _pathelem = parent._pathelem;
        _id = iodef_class_get_child_class(parent._id, child_id);

        elem.parent_id = parent._id;
        elem.idx = child_id;
        _pathelem.push_back(elem);
}


std::string IODEFClass::toString(void)
{
        unsigned int i = 0;
        std::string s  = "IODEFClass(" + getName();

        do {
                if ( i > 0 )
                        s += ", ";

                try {
                        s += get(i++).toString();
                } catch(...) {
                        break;
                }
        } while ( TRUE );

        s += "\n)";

        return s;
}


bool IODEFClass::isList(void)
{
        if ( _pathelem.size() == 0 )
                throw PreludeError("Already in rootclass, cannot retrieve parents info");

        return iodef_class_is_child_list(_pathelem.back().parent_id, _pathelem.back().idx);
}


bool IODEFClass::isKeyedList(void)
{
        if ( _pathelem.size() == 0 )
                throw PreludeError("Already in rootclass, cannot retrieve parents info");

        return iodef_class_is_child_keyed_list(_pathelem.back().parent_id, _pathelem.back().idx);
}


Iodef::IODEFValue::IODEFValueTypeEnum IODEFClass::getValueType(void)
{
        if ( _pathelem.size() == 0 )
                throw PreludeError("Already in rootclass, cannot retrieve parents info");

        return (Iodef::IODEFValue::IODEFValueTypeEnum) iodef_class_get_child_value_type(_pathelem.back().parent_id, _pathelem.back().idx);
}


std::string IODEFClass::getName(void)
{
        if ( _pathelem.size() == 0 )
                return iodef_class_get_name(_id);

        return iodef_class_get_child_name(_pathelem.back().parent_id, _pathelem.back().idx);
}


size_t IODEFClass::getDepth(void)
{
        return _pathelem.size();
}


std::string IODEFClass::getPath(int rootidx, int depth, const std::string &sep, const std::string &listidx)
{
        std::string out;

        if ( depth >= 0 ) {
                if ( (depth + 1) == _pathelem.size() )
                        return getName();

                IODEFClassElem elem = _pathelem[depth];
                return iodef_class_get_child_name(elem.parent_id, elem.idx);
        }

        for ( std::vector<IODEFClassElem>::iterator it = _pathelem.begin() + rootidx; it != _pathelem.end(); it++) {
                out += iodef_class_get_child_name((*it).parent_id, (*it).idx);

                if ( iodef_class_is_child_list((*it).parent_id, (*it).idx) )
                        out += listidx;

                if ( it + 1 != _pathelem.end() )
                        out += sep;
        }

        return out;
}



IODEFClass IODEFClass::get(const std::string &name)
{
        int i = iodef_class_find_child(_id, name.c_str());

        if ( i < 0 )
                throw PreludeError(i);

        return get(i);
}


IODEFClass IODEFClass::get(int i)
{
        iodef_class_id_t cl;
        iodef_value_type_id_t vl;

        cl = iodef_class_get_child_class(_id, i);
        if ( cl < 0 ) {
                vl = (iodef_value_type_id_t) iodef_class_get_child_value_type(_id, i);
                if ( vl < 0 )
                        throw PreludeError(vl);
        }

        return IODEFClass(*this, i, _depth + 1);
}



std::vector<std::string> IODEFClass::getEnumValues(void)
{
        int i = 0;
        const char *ret;
        std::vector<std::string> ev;

        if ( getValueType() != IODEFValue::TYPE_ENUM )
                throw PreludeError("Input class is not enumeration");

        do {
                ret = iodef_class_enum_to_string(_id, i++);
                if ( ret )
                        ev.push_back(ret);

        } while ( ret || i == 1 ); /* entry 0 might be NULL, if the enumeration has no default value */

        return ev;
}



IODEFCriterion::IODEFCriterionOperatorEnum IODEFClass::getApplicableOperator(void)
{
        int ret;
        iodef_criterion_operator_t op;

        ret = iodef_value_type_get_applicable_operators((iodef_value_type_id_t) getValueType(), &op);
        if ( ret < 0 )
                throw PreludeError(ret);

        return (IODEFCriterion::IODEFCriterionOperatorEnum) ret;
}
