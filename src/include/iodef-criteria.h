/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@prelude-ids.com>
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

#ifndef _LIBPRELUDE_IODEF_CRITERIA_H
#define _LIBPRELUDE_IODEF_CRITERIA_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        IODEF_CRITERION_OPERATOR_NOT               = 0x8000,
        IODEF_CRITERION_OPERATOR_NOCASE            = 0x4000,

        IODEF_CRITERION_OPERATOR_EQUAL             = 0x0001,
        IODEF_CRITERION_OPERATOR_EQUAL_NOCASE      = IODEF_CRITERION_OPERATOR_EQUAL|IODEF_CRITERION_OPERATOR_NOCASE,
        IODEF_CRITERION_OPERATOR_NOT_EQUAL         = IODEF_CRITERION_OPERATOR_NOT|IODEF_CRITERION_OPERATOR_EQUAL,
        IODEF_CRITERION_OPERATOR_NOT_EQUAL_NOCASE  = IODEF_CRITERION_OPERATOR_NOT_EQUAL|IODEF_CRITERION_OPERATOR_EQUAL_NOCASE,

        IODEF_CRITERION_OPERATOR_LESSER            = 0x0002,
        IODEF_CRITERION_OPERATOR_LESSER_OR_EQUAL   = IODEF_CRITERION_OPERATOR_LESSER|IODEF_CRITERION_OPERATOR_EQUAL,

        IODEF_CRITERION_OPERATOR_GREATER           = 0x0004,
        IODEF_CRITERION_OPERATOR_GREATER_OR_EQUAL  = IODEF_CRITERION_OPERATOR_GREATER|IODEF_CRITERION_OPERATOR_EQUAL,

        IODEF_CRITERION_OPERATOR_SUBSTR            = 0x0008,
        IODEF_CRITERION_OPERATOR_SUBSTR_NOCASE     = IODEF_CRITERION_OPERATOR_SUBSTR|IODEF_CRITERION_OPERATOR_NOCASE,
        IODEF_CRITERION_OPERATOR_NOT_SUBSTR        = IODEF_CRITERION_OPERATOR_NOT|IODEF_CRITERION_OPERATOR_SUBSTR,
        IODEF_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE = IODEF_CRITERION_OPERATOR_NOT|IODEF_CRITERION_OPERATOR_SUBSTR_NOCASE,

        IODEF_CRITERION_OPERATOR_REGEX             = 0x0010,
        IODEF_CRITERION_OPERATOR_REGEX_NOCASE      = IODEF_CRITERION_OPERATOR_REGEX|IODEF_CRITERION_OPERATOR_NOCASE,
        IODEF_CRITERION_OPERATOR_NOT_REGEX         = IODEF_CRITERION_OPERATOR_NOT|IODEF_CRITERION_OPERATOR_REGEX,
        IODEF_CRITERION_OPERATOR_NOT_REGEX_NOCASE  = IODEF_CRITERION_OPERATOR_NOT|IODEF_CRITERION_OPERATOR_REGEX_NOCASE,

        IODEF_CRITERION_OPERATOR_NULL              = 0x0020,
        IODEF_CRITERION_OPERATOR_NOT_NULL          = IODEF_CRITERION_OPERATOR_NULL|IODEF_CRITERION_OPERATOR_NOT
} iodef_criterion_operator_t;


typedef struct iodef_criteria iodef_criteria_t;
typedef struct iodef_criterion iodef_criterion_t;

#include "iodef-path.h"
#include "iodef-criterion-value.h"

const char *iodef_criterion_operator_to_string(iodef_criterion_operator_t op);

int iodef_criterion_new(iodef_criterion_t **criterion, iodef_path_t *path,
                        iodef_criterion_value_t *value, iodef_criterion_operator_t op);

void iodef_criterion_destroy(iodef_criterion_t *criterion);
iodef_criteria_t *iodef_criteria_ref(iodef_criteria_t *criteria);
int iodef_criterion_clone(const iodef_criterion_t *criterion, iodef_criterion_t **dst);
int iodef_criterion_print(const iodef_criterion_t *criterion, prelude_io_t *fd);
int iodef_criterion_to_string(const iodef_criterion_t *criterion, prelude_string_t *out);
iodef_path_t *iodef_criterion_get_path(const iodef_criterion_t *criterion);
iodef_criterion_value_t *iodef_criterion_get_value(const iodef_criterion_t *criterion);
iodef_criterion_operator_t iodef_criterion_get_operator(const iodef_criterion_t *criterion);
int iodef_criterion_match(const iodef_criterion_t *criterion, void *object);

int iodef_criteria_new(iodef_criteria_t **criteria);
void iodef_criteria_destroy(iodef_criteria_t *criteria);
int iodef_criteria_clone(iodef_criteria_t *src, iodef_criteria_t **dst);
int iodef_criteria_print(const iodef_criteria_t *criteria, prelude_io_t *fd);
int iodef_criteria_to_string(const iodef_criteria_t *criteria, prelude_string_t *out);
prelude_bool_t iodef_criteria_is_criterion(const iodef_criteria_t *criteria);
iodef_criterion_t *iodef_criteria_get_criterion(const iodef_criteria_t *criteria);
void iodef_criteria_set_criterion(iodef_criteria_t *criteria, iodef_criterion_t *criterion);

void iodef_criteria_or_criteria(iodef_criteria_t *criteria, iodef_criteria_t *criteria2);

int iodef_criteria_and_criteria(iodef_criteria_t *criteria, iodef_criteria_t *criteria2);

int iodef_criteria_match(const iodef_criteria_t *criteria, void *object);

iodef_criteria_t *iodef_criteria_get_or(const iodef_criteria_t *criteria);

iodef_criteria_t *iodef_criteria_get_and(const iodef_criteria_t *criteria);

int iodef_criteria_new_from_string(iodef_criteria_t **criteria, const char *str);

void iodef_criteria_set_negation(iodef_criteria_t *criteria, prelude_bool_t negate);

prelude_bool_t iodef_criteria_get_negation(const iodef_criteria_t *criteria);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBPRELUDE_IODEF_CRITERIA_H */
