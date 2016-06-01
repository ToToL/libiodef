/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann@libiodef-ids.com>
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

#ifndef _LIBIODEF_IODEF_CRITERIA_HXX
#define _LIBIODEF_IODEF_CRITERIA_HXX

#include <string>
#include "iodef.h"
#include "iodef.hxx"

namespace Iodef {
        class IODEFCriterion {
            public:
                enum IODEFCriterionOperatorEnum {
                        OPERATOR_NOT     = IODEF_CRITERION_OPERATOR_NOT,
                        OPERATOR_NOCASE  = IODEF_CRITERION_OPERATOR_NOCASE,

                        OPERATOR_EQUAL   = IODEF_CRITERION_OPERATOR_EQUAL,
                        OPERATOR_EQUAL_NOCASE = IODEF_CRITERION_OPERATOR_EQUAL_NOCASE,
                        OPERATOR_NOT_EQUAL = IODEF_CRITERION_OPERATOR_NOT_EQUAL,
                        OPERATOR_NOT_EQUAL_NOCASE = IODEF_CRITERION_OPERATOR_NOT_EQUAL_NOCASE,

                        OPERATOR_LESSER = IODEF_CRITERION_OPERATOR_LESSER,
                        OPERATOR_LESSER_OR_EQUAL = IODEF_CRITERION_OPERATOR_LESSER_OR_EQUAL,

                        OPERATOR_GREATER = IODEF_CRITERION_OPERATOR_GREATER,
                        OPERATOR_GREATER_OR_EQUAL = IODEF_CRITERION_OPERATOR_GREATER_OR_EQUAL,

                        OPERATOR_SUBSTR = IODEF_CRITERION_OPERATOR_SUBSTR,
                        OPERATOR_SUBSTR_NOCASE = IODEF_CRITERION_OPERATOR_SUBSTR_NOCASE,
                        OPERATOR_NOT_SUBSTR = IODEF_CRITERION_OPERATOR_NOT_SUBSTR,
                        OPERATOR_NOT_SUBSTR_NOCASE = IODEF_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE,

                        OPERATOR_REGEX = IODEF_CRITERION_OPERATOR_REGEX,
                        OPERATOR_REGEX_NOCASE = IODEF_CRITERION_OPERATOR_REGEX_NOCASE,
                        OPERATOR_NOT_REGEX = IODEF_CRITERION_OPERATOR_NOT_REGEX,
                        OPERATOR_NOT_REGEX_NOCASE = IODEF_CRITERION_OPERATOR_NOT_REGEX_NOCASE,

                        OPERATOR_NULL = IODEF_CRITERION_OPERATOR_NULL,
                        OPERATOR_NOT_NULL = IODEF_CRITERION_OPERATOR_NOT_NULL
                };
        };

        class IODEFCriteria {
            private:
                iodef_criteria_t *_criteria;

            public:
                ~IODEFCriteria();
                IODEFCriteria();
                IODEFCriteria(const IODEFCriteria &criteria);
                IODEFCriteria(const char *criteria);
                IODEFCriteria(const std::string &criteria);
                IODEFCriteria(iodef_criteria_t *criteria);

                int match(Iodef::IODEF *message) const;
                IODEFCriteria clone() const;
                void andCriteria(const IODEFCriteria &criteria);
                void orCriteria(const IODEFCriteria &criteria);
                const std::string toString() const;

                operator const std::string() const;
                operator iodef_criteria_t *() const;
                IODEFCriteria &operator=(const IODEFCriteria &criteria);
        };
};

#endif
