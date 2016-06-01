/*****
*
* Copyright (C) 2014-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEF_IODEF_CLASS_HXX
#define _LIBIODEF_IODEF_CLASS_HXX

#include <string>
#include <vector>

#include "iodef-criteria.hxx"
#include "iodef-path.hxx"

namespace Iodef {
        class IODEFClass {
            private:
                class IODEFClassElem {
                        public:
                                int idx;
                                iodef_class_id_t parent_id;
                };

                int _depth;
                iodef_class_id_t _id;
                std::vector<IODEFClass::IODEFClassElem> _pathelem;

                IODEFClass(IODEFClass &parent, int child_id, int depth=0);
            public:
                IODEFClass(iodef_class_id_t id=IODEF_CLASS_ID_DOCUMENT);
                IODEFClass(const IODEFPath &path);
                IODEFClass(const std::string &path);

                size_t getDepth(void);
                IODEFClass get(int child);
                IODEFClass get(const std::string &name);

                size_t getChildCount() { return iodef_class_get_child_count(_id); };

                /* main object operation */
                bool isList(void);
                bool isKeyedList(void);
                std::string getName(void);
                std::string toString(void);
                Iodef::IODEFValue::IODEFValueTypeEnum getValueType(void);
                std::string getPath(int rootidx=0, int depth=-1, const std::string &sep = ".", const std::string &listidx="");
                std::vector<std::string> getEnumValues(void);
                Iodef::IODEFCriterion::IODEFCriterionOperatorEnum getApplicableOperator(void);
        };
};

#endif
