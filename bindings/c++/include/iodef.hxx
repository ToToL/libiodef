/*****
*
* Copyright (C) 2009-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBPRELUDE_IODEF_HXX
#define _LIBPRELUDE_IODEF_HXX

#include <string>
#include <iostream>
#include "iodef-value.hxx"
#include "iodef-time.hxx"

namespace Iodef
{
        class IODEF;
}

namespace Iodef {
        class IODEF {
            protected:
                iodef_object_t *_object;

            public:
                ~IODEF();
                IODEF();
                IODEF(const char *json);
                IODEF(const IODEF &iodef);
                IODEF(iodef_object_t *message);

                void set(const char *path, const std::vector<Iodef::IODEF> &value);
                void set(const char *path, Iodef::IODEF *value);
                void set(const char *path, const std::vector<Iodef::IODEFValue> &value);
                void set(const char *path, const std::string &value);
                void set(const char *path, Iodef::IODEFValue *value);
                void set(const char *path, Iodef::IODEFValue &value);
                void set(const char *path, Iodef::IODEFTime &value);
                void set(const char *path, const char *value);
                void set(const char *path, int32_t value);
                void set(const char *path, int64_t value);
                void set(const char *path, uint64_t value);
                void set(const char *path, float value);
                void set(const char *path, double value);
                Iodef::IODEFValue get(const char *path);


                IODEF clone();
                iodef_class_id_t getId() const;
                const std::string toString() const;
                const std::string toJSON() const;
                const std::string toBinary() const;

                operator const std::string() const;
                operator iodef_object_t *() const;
                IODEF &operator = (const IODEF &iodef);
                int operator == (const IODEF *iodef);
        };
};

#endif /* __LIBPRELUDE_IODEF_HXX */
