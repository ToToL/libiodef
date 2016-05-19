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

#ifndef _LIBPRELUDE_IODEF_PATH_HXX
#define _LIBPRELUDE_IODEF_PATH_HXX

#include "iodef.h"
#include "iodef-path.h"

#include "iodef.hxx"
#include "prelude-error.hxx"
#include "iodef-value.hxx"


namespace Iodef {
        class IODEFClass;

        class IODEFPath {
            private:
                iodef_path_t *_path;

            public:
                IODEFPath(const char *buffer);
                IODEFPath(Iodef::IODEF &iodef, const char *buffer);

                IODEFPath(iodef_path_t *path);
                IODEFPath(const IODEFPath &path);
                ~IODEFPath();

                Iodef::IODEFValue get(const Iodef::IODEF &message) const;
                void set(Iodef::IODEF &message, const std::vector<Iodef::IODEF> &value) const;
                void set(Iodef::IODEF &message, Iodef::IODEF *value) const;
                void set(Iodef::IODEF &message, const std::vector<Iodef::IODEFValue> &value) const;
                void set(Iodef::IODEF &message, Iodef::IODEFValue *value) const;
                void set(Iodef::IODEF &message, Iodef::IODEFValue &value) const;
                void set(Iodef::IODEF &message, Iodef::IODEFTime &time) const;
                void set(Iodef::IODEF &message, const std::string &value) const;
                void set(Iodef::IODEF &message, const char *value) const;
                void set(Iodef::IODEF &message, int32_t value) const;
                void set(Iodef::IODEF &message, int64_t value) const;
                void set(Iodef::IODEF &message, uint64_t value) const;
                void set(Iodef::IODEF &message, float value) const;
                void set(Iodef::IODEF &message, double value) const;

                Iodef::IODEFClass getClass(int depth=-1) const;
                Iodef::IODEFValue::IODEFValueTypeEnum getValueType(int depth=-1) const;
                int setIndex(unsigned int index, int depth=-1);
                int undefineIndex(int depth=-1);
                int getIndex(int depth=-1) const;
                int makeChild(const char *child_name, unsigned int index);
                int makeParent();
                int compare(IODEFPath *path, int depth=-1) const;
                IODEFPath clone() const;

                int checkOperator(iodef_criterion_operator_t op) const;
                iodef_criterion_operator_t getApplicableOperators() const;

                const char *getName(int depth=-1) const;
                bool isAmbiguous() const;
                int hasLists() const;
                bool isList(int depth=-1) const;
                unsigned int getDepth() const;

                IODEFPath &operator = (const IODEFPath &path);
                operator iodef_path_t*() const;
        };
};

#endif
