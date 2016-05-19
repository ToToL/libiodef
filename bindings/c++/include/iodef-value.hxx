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

#ifndef _LIBPRELUDE_IODEF_VALUE_HXX
#define _LIBPRELUDE_IODEF_VALUE_HXX

#include <vector>
#include "iodef-time.hxx"

namespace Iodef
{
        class IODEF;
}

namespace Iodef {
        class IODEFValue {
            private:
                void _InitFromString(const char *value, size_t size);
                std::string convert_string() const;

            protected:
                iodef_value_t *_value;
                std::string _myconv;

            public:
                enum IODEFValueTypeEnum {
                        TYPE_UNKNOWN    = IODEF_VALUE_TYPE_UNKNOWN,
                        TYPE_INT8       = IODEF_VALUE_TYPE_INT8,
                        TYPE_UINT8      = IODEF_VALUE_TYPE_UINT8,
                        TYPE_INT16      = IODEF_VALUE_TYPE_INT16,
                        TYPE_UINT16     = IODEF_VALUE_TYPE_UINT16,
                        TYPE_INT32      = IODEF_VALUE_TYPE_INT32,
                        TYPE_UINT32     = IODEF_VALUE_TYPE_UINT32,
                        TYPE_INT64      = IODEF_VALUE_TYPE_INT64,
                        TYPE_UINT64     = IODEF_VALUE_TYPE_UINT64,
                        TYPE_FLOAT      = IODEF_VALUE_TYPE_FLOAT,
                        TYPE_DOUBLE     = IODEF_VALUE_TYPE_DOUBLE,
                        TYPE_STRING     = IODEF_VALUE_TYPE_STRING,
                        TYPE_TIME       = IODEF_VALUE_TYPE_TIME,
                        TYPE_DATA       = IODEF_VALUE_TYPE_DATA,
                        TYPE_ENUM       = IODEF_VALUE_TYPE_ENUM,
                        TYPE_LIST       = IODEF_VALUE_TYPE_LIST,
                        TYPE_CLASS      = IODEF_VALUE_TYPE_CLASS
                };

                IODEFValueTypeEnum getType() const;
                bool isNull() const;

                IODEFValue();
                ~IODEFValue();
                IODEFValue(IODEF *iodef);
                IODEFValue(const std::vector<IODEF> &value);
                IODEFValue(const IODEFValue &value);
                IODEFValue(const std::vector<IODEFValue> &value);
                IODEFValue(iodef_value_t *value);
                IODEFValue(const std::string &value);
                IODEFValue(const char *value);
                IODEFValue(int32_t value);
                IODEFValue(int64_t value);
                IODEFValue(uint64_t value);
                IODEFValue(float value);
                IODEFValue(double value);
                IODEFValue(Iodef::IODEFTime &time);

                int match(const IODEFValue &value, int op) const;
                IODEFValue clone() const;
                const std::string toString() const;

                operator int32_t() const;
                operator uint32_t() const;
                operator int64_t() const;
                operator uint64_t() const;
                operator double() const;
                operator std::vector<IODEFValue>() const;
                operator const char*() const;
                operator float() const;
                operator Iodef::IODEFTime() const;
                operator iodef_value_t *() const;

                IODEFValue & operator=(const IODEFValue &p);

                bool operator == (const std::vector<IODEFValue> &vlist) const;
                bool operator <= (const IODEFValue &value) const;
                bool operator >= (const IODEFValue &value) const;
                bool operator < (const IODEFValue &value) const;
                bool operator > (const IODEFValue &value) const;
                bool operator == (const IODEFValue &value) const;
                bool operator != (const IODEFValue &value) const;
        };
};

#endif
