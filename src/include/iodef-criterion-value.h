/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@prelude-ids.com>
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

#ifndef _LIBPRELUDE_IODEF_CRITERION_VALUE_H
#define _LIBPRELUDE_IODEF_CRITERION_VALUE_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct iodef_criterion_value iodef_criterion_value_t;

typedef enum {
        IODEF_CRITERION_VALUE_TYPE_ERROR            = -1,
        IODEF_CRITERION_VALUE_TYPE_VALUE            =  0,
        IODEF_CRITERION_VALUE_TYPE_REGEX            =  1,
        IODEF_CRITERION_VALUE_TYPE_BROKEN_DOWN_TIME =  2
} iodef_criterion_value_type_t;

         
#include "iodef-criteria.h"
#include "iodef-value.h"
 

int iodef_criterion_value_new(iodef_criterion_value_t **cv);

int iodef_criterion_value_new_regex(iodef_criterion_value_t **cv, const char *regex, iodef_criterion_operator_t op);

int iodef_criterion_value_new_value(iodef_criterion_value_t **cv, iodef_value_t *value,
                                    iodef_criterion_operator_t op);

int iodef_criterion_value_new_from_string(iodef_criterion_value_t **cv, iodef_path_t *path,
                                          const char *value, iodef_criterion_operator_t op);

int iodef_criterion_value_new_broken_down_time(iodef_criterion_value_t **cv, const char *time, iodef_criterion_operator_t op);
         
int iodef_criterion_value_clone(const iodef_criterion_value_t *src, iodef_criterion_value_t **dst);

void iodef_criterion_value_destroy(iodef_criterion_value_t *value);

int iodef_criterion_value_print(iodef_criterion_value_t *value, prelude_io_t *fd);

int iodef_criterion_value_to_string(iodef_criterion_value_t *value, prelude_string_t *out);

int iodef_criterion_value_match(iodef_criterion_value_t *cv, iodef_value_t *value, iodef_criterion_operator_t op);

const iodef_value_t *iodef_criterion_value_get_value(iodef_criterion_value_t *cv);

const char *iodef_criterion_value_get_regex(iodef_criterion_value_t *cv);

const struct tm *iodef_criterion_value_get_broken_down_time(iodef_criterion_value_t *cv);
         
iodef_criterion_value_type_t iodef_criterion_value_get_type(iodef_criterion_value_t *cv);

#ifdef __cplusplus
 }
#endif

         
#endif /* _LIBPRELUDE_IODEF_CRITERION_VALUE_H */
