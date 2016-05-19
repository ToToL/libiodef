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

#include "config.h"
#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdarg.h>

#define PRELUDE_ERROR_SOURCE_DEFAULT PRELUDE_ERROR_SOURCE_IODEF_CRITERIA
#include "iodef.h"
#include "iodef-criteria.h"


struct iodef_criterion {
        iodef_path_t *path;
        iodef_criterion_value_t *value;
        iodef_criterion_operator_t operator;
};


struct iodef_criteria {
        int refcount;
        prelude_bool_t negated;
        iodef_criterion_t *criterion;
        struct iodef_criteria *or;
        struct iodef_criteria *and;
};



/**
 * iodef_criterion_operator_to_string:
 * @op: #iodef_criterion_operator_t type.
 *
 * Transforms @op to string.
 *
 * Returns: A pointer to an operator string or NULL.
 */
const char *iodef_criterion_operator_to_string(iodef_criterion_operator_t op)
{
        int i;
        const struct {
                iodef_criterion_operator_t operator;
                const char *name;
        } tbl[] = {
                { IODEF_CRITERION_OPERATOR_EQUAL,     "="            },
                { IODEF_CRITERION_OPERATOR_EQUAL_NOCASE, "=*"        },

                { IODEF_CRITERION_OPERATOR_NOT_EQUAL, "!="           },
                { IODEF_CRITERION_OPERATOR_NOT_EQUAL_NOCASE, "!=*"   },

                { IODEF_CRITERION_OPERATOR_LESSER, "<"               },
                { IODEF_CRITERION_OPERATOR_GREATER, ">"              },
                { IODEF_CRITERION_OPERATOR_LESSER_OR_EQUAL, "<="     },
                { IODEF_CRITERION_OPERATOR_GREATER_OR_EQUAL, ">="    },

                { IODEF_CRITERION_OPERATOR_REGEX, "~"                },
                { IODEF_CRITERION_OPERATOR_REGEX_NOCASE, "~*"        },
                { IODEF_CRITERION_OPERATOR_NOT_REGEX, "!~"           },
                { IODEF_CRITERION_OPERATOR_NOT_REGEX_NOCASE, "!~*"   },

                { IODEF_CRITERION_OPERATOR_SUBSTR, "<>"              },
                { IODEF_CRITERION_OPERATOR_SUBSTR_NOCASE, "<>*"      },
                { IODEF_CRITERION_OPERATOR_NOT_SUBSTR, "!<>"         },
                { IODEF_CRITERION_OPERATOR_NOT_SUBSTR_NOCASE, "!<>*" },

                { IODEF_CRITERION_OPERATOR_NOT_NULL, ""              },
                { IODEF_CRITERION_OPERATOR_NULL, "!"                 },
        };

        for ( i = 0; tbl[i].operator != 0; i++ )
                if ( op == tbl[i].operator )
                        return tbl[i].name;

        return NULL;
}



/**
 * iodef_criterion_new:
 * @criterion: Address where to store the created #iodef_criterion_t object.
 * @path: Pointer to an #iodef_path_t object.
 * @value: Pointer to an #iodef_criterion_value_t object.
 * @op: #iodef_criterion_operator_t to use for matching this criterion.
 *
 * Creates a new #iodef_criterion_t object and store it in @criterion.
 * Matching this criterion will result in comparing the object value
 * pointed by @path against the provided @value, using @op.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_criterion_new(iodef_criterion_t **criterion, iodef_path_t *path,
                        iodef_criterion_value_t *value, iodef_criterion_operator_t op)
{
        prelude_return_val_if_fail(path != NULL, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(! (value == NULL && ! (op & IODEF_CRITERION_OPERATOR_NULL)), prelude_error(PRELUDE_ERROR_ASSERTION));

        *criterion = calloc(1, sizeof(**criterion));
        if ( ! *criterion )
                return prelude_error_from_errno(errno);

        (*criterion)->path = path;
        (*criterion)->value = value;
        (*criterion)->operator = op;

        return 0;
}



/**
 * iodef_criterion_destroy:
 * @criterion: Pointer to a #iodef_criterion_t object.
 *
 * Destroys @criterion and its content.
 */
void iodef_criterion_destroy(iodef_criterion_t *criterion)
{
        prelude_return_if_fail(criterion);

        iodef_path_destroy(criterion->path);

        if ( criterion->value ) /* can be NULL if operator is is_null or is_not_null */
                iodef_criterion_value_destroy(criterion->value);

        free(criterion);
}



/**
 * iodef_criterion_clone:
 * @criterion: Pointer to a #iodef_criterion_t object to clone.
 * @dst: Address where to store the cloned #iodef_criterion_t object.
 *
 * Clones @criterion and stores the cloned criterion within @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_criterion_clone(const iodef_criterion_t *criterion, iodef_criterion_t **dst)
{
        int ret;
        iodef_path_t *path;
        iodef_criterion_value_t *value = NULL;

        prelude_return_val_if_fail(criterion, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = iodef_path_clone(criterion->path, &path);
        if ( ret < 0 )
                return ret;

        if ( criterion->value ) {
                ret = iodef_criterion_value_clone(criterion->value, &value);
                if ( ret < 0 ) {
                        iodef_path_destroy(path);
                        return ret;
                }
        }

        ret = iodef_criterion_new(dst, path, value, criterion->operator);
        if ( ret < 0 ) {
                iodef_path_destroy(path);
                iodef_criterion_value_destroy(value);
                return ret;
        }

        return 0;
}



/**
 * iodef_criterion_print:
 * @criterion: Pointer to a #iodef_criterion_t object.
 * @fd: Pointer to a #prelude_io_t object.
 *
 * Dump @criterion to @fd in the form of:
 * [path] [operator] [value]
 *
 * Or if there is no value associated with the criterion:
 * [operator] [path]
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_criterion_print(const iodef_criterion_t *criterion, prelude_io_t *fd)
{
        int ret;
        prelude_string_t *out;

        prelude_return_val_if_fail(criterion, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(fd, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = prelude_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = iodef_criterion_to_string(criterion, out);
        if ( ret < 0 ) {
                prelude_string_destroy(out);
                return ret;
        }

        ret = prelude_io_write(fd, prelude_string_get_string(out), prelude_string_get_len(out));
        prelude_string_destroy(out);

        return ret;
}



/**
 * iodef_criterion_to_string:
 * @criterion: Pointer to a #iodef_criterion_t object.
 * @out: Pointer to a #prelude_string_t object.
 *
 * Dump @criterion as a string to the @out buffer in the form of:
 * [path] [operator] [value]
 *
 * Or if there is no value associated with the criterion:
 * [operator] [path]
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_criterion_to_string(const iodef_criterion_t *criterion, prelude_string_t *out)
{
        const char *name, *operator;

        prelude_return_val_if_fail(criterion, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(out, prelude_error(PRELUDE_ERROR_ASSERTION));

        operator = iodef_criterion_operator_to_string(criterion->operator);
        if ( ! operator )
                return -1;

        name = iodef_path_get_name(criterion->path, -1);

        if ( ! criterion->value )
                return prelude_string_sprintf(out, "%s%s%s", operator, (*operator) ? " " : "", name);

        prelude_string_sprintf(out, "%s %s ", name, operator);

        return iodef_criterion_value_to_string(criterion->value, out);
}



/**
 * iodef_criterion_get_path:
 * @criterion: Pointer to a #iodef_criterion_t object.
 *
 * Used to access the #iodef_path_t object associated with @criterion.
 *
 * Returns: the #iodef_path_t object associated with @criterion.
 */
iodef_path_t *iodef_criterion_get_path(const iodef_criterion_t *criterion)
{
        prelude_return_val_if_fail(criterion, NULL);
        return criterion->path;
}



/**
 * iodef_criterion_get_value:
 * @criterion: Pointer to a #iodef_criterion_t object.
 *
 * Used to access the #iodef_criterion_value_t associated with @criterion.
 * There might be no value specifically if the provided #iodef_criterion_operator_t
 * was IODEF_CRITERION_OPERATOR_NULL or IODEF_CRITERION_OPERATOR_NOT_NULL.
 *
 * Returns: the #iodef_criterion_value_t object associated with @criterion.
 */
iodef_criterion_value_t *iodef_criterion_get_value(const iodef_criterion_t *criterion)
{
        prelude_return_val_if_fail(criterion, NULL);
        return criterion->value;
}




/**
 * iodef_criterion_get_operator:
 * @criterion: Pointer to a #iodef_criterion_t object.
 *
 * Used to access the #iodef_criterion_operator_t enumeration associated with @criterion.
 *
 * Returns: the #iodef_criterion_operator_t associated with @criterion.
 */
iodef_criterion_operator_t iodef_criterion_get_operator(const iodef_criterion_t *criterion)
{
        prelude_return_val_if_fail(criterion, prelude_error(PRELUDE_ERROR_ASSERTION));
        return criterion->operator;
}



/**
 * iodef_criterion_match:
 * @criterion: Pointer to a #iodef_criterion_t object.
 * @object: Pointer to a #iodef_object_t object to match against @criterion.
 *
 * Matches @message against the provided @criterion. This implies retrieving the
 * value associated with @criterion path, and matching it with the @iodef_criterion_value_t
 * object within @criterion.
 *
 * Returns: 1 for a match, 0 for no match, or a negative value if an error occured.
 */
int iodef_criterion_match(const iodef_criterion_t *criterion, void *object)
{
        int ret;
        iodef_value_t *value = NULL;

        prelude_return_val_if_fail(criterion, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(object, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = iodef_path_get(criterion->path, object, &value);
        if ( ret < 0 )
                return ret;

        ret = iodef_criterion_value_match(criterion->value, value, criterion->operator);
        if ( value )
                iodef_value_destroy(value);

        if ( ret < 0 )
                return ret;

        return (ret > 0) ? 1 : 0;
}



/**
 * iodef_criteria_new:
 * @criteria: Address where to store the created #iodef_criteria_t object.
 *
 * Creates a new #iodef_criteria_t object and store it into @criteria.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_criteria_new(iodef_criteria_t **criteria)
{
        *criteria = calloc(1, sizeof(**criteria));
        if ( ! *criteria )
                return prelude_error_from_errno(errno);

        (*criteria)->or = NULL;
        (*criteria)->and = NULL;
        (*criteria)->refcount = 1;

        return 0;
}



/**
 * iodef_criteria_destroy:
 * @criteria: Pointer to a #iodef_criteria_t object.
 *
 * Destroys @criteria and its content.
 */
void iodef_criteria_destroy(iodef_criteria_t *criteria)
{
        prelude_return_if_fail(criteria);

        if ( --criteria->refcount )
                return;

        if ( criteria->criterion )
                iodef_criterion_destroy(criteria->criterion);

        if ( criteria->or )
                iodef_criteria_destroy(criteria->or);

        if ( criteria->and )
                iodef_criteria_destroy(criteria->and);

        free(criteria);
}



/**
 * iodef_criteria_ref:
 * @criteria: Pointer to a #iodef_criteria_t object to reference.
 *
 * Increases @criteria reference count.
 *
 * iodef_criteria_destroy() will decrease the refcount until it reaches
 * 0, at which point @criteria will be destroyed.
 *
 * Returns: @criteria.
 */
iodef_criteria_t *iodef_criteria_ref(iodef_criteria_t *criteria)
{
        prelude_return_val_if_fail(criteria, NULL);

        criteria->refcount++;
        return criteria;
}


/**
 * iodef_criteria_clone:
 * @src: Pointer to a #iodef_criteria_t object to clone.
 * @dst: Address where to store the cloned #iodef_criteria_t object.
 *
 * Clones @src and stores the cloned criteria within @dst.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_criteria_clone(iodef_criteria_t *src, iodef_criteria_t **dst)
{
        int ret;
        iodef_criteria_t *new;

        prelude_return_val_if_fail(src, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = iodef_criteria_new(dst);
        if ( ret < 0 )
                return ret;

        new = *dst;
        new->negated = src->negated;

        if ( src->or ) {
                ret = iodef_criteria_clone(src->or, &new->or);
                if ( ret < 0 ) {
                        iodef_criteria_destroy(new);
                        return ret;
                }
        }

        if ( src->and ) {
                ret = iodef_criteria_clone(src->and, &new->and);
                if ( ret < 0 ) {
                        iodef_criteria_destroy(new);
                        return ret;
                }
        }

        ret = iodef_criterion_clone(src->criterion, &new->criterion);
        if ( ret < 0 ) {
                iodef_criteria_destroy(new);
                return ret;
        }

        return 0;
}



iodef_criteria_t *iodef_criteria_get_or(const iodef_criteria_t *criteria)
{
        prelude_return_val_if_fail(criteria, NULL);
        return criteria->or;
}



iodef_criteria_t *iodef_criteria_get_and(const iodef_criteria_t *criteria)
{
        prelude_return_val_if_fail(criteria, NULL);
        return criteria->and;
}



int iodef_criteria_print(const iodef_criteria_t *criteria, prelude_io_t *fd)
{
        int ret;
        prelude_string_t *out;

        prelude_return_val_if_fail(criteria, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(fd, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = prelude_string_new(&out);
        if ( ret < 0 )
                return ret;

        ret = iodef_criteria_to_string(criteria, out);
        if ( ret < 0 )
                return ret;

        ret = prelude_io_write(fd, prelude_string_get_string(out), prelude_string_get_len(out));
        prelude_string_destroy(out);

        return ret;
}



int iodef_criteria_to_string(const iodef_criteria_t *criteria, prelude_string_t *out)
{
        prelude_return_val_if_fail(criteria, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(out, prelude_error(PRELUDE_ERROR_ASSERTION));

        if ( criteria->or )
                prelude_string_sprintf(out, "((");

        iodef_criterion_to_string(criteria->criterion, out);

        if ( criteria->and ) {
                prelude_string_sprintf(out, " && ");
                iodef_criteria_to_string(criteria->and, out);
        }

        if ( criteria->or ) {
                prelude_string_sprintf(out, ") || (");
                iodef_criteria_to_string(criteria->or, out);
                prelude_string_sprintf(out, "))");
        }

        return 0;
}



prelude_bool_t iodef_criteria_is_criterion(const iodef_criteria_t *criteria)
{
        prelude_return_val_if_fail(criteria, FALSE);
        return (criteria->criterion != NULL) ? TRUE : FALSE;
}



iodef_criterion_t *iodef_criteria_get_criterion(const iodef_criteria_t *criteria)
{
        prelude_return_val_if_fail(criteria, NULL);
        return criteria->criterion;
}



void iodef_criteria_or_criteria(iodef_criteria_t *criteria, iodef_criteria_t *criteria2)
{
        prelude_return_if_fail(criteria);
        prelude_return_if_fail(criteria2);

        while ( criteria->or )
                criteria = criteria->or;

        criteria->or = criteria2;
}



int iodef_criteria_and_criteria(iodef_criteria_t *criteria, iodef_criteria_t *criteria2)
{
        int ret;
        iodef_criteria_t *new, *last = NULL;

        prelude_return_val_if_fail(criteria, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(criteria2, prelude_error(PRELUDE_ERROR_ASSERTION));

        while ( criteria ) {
                last = criteria;

                if ( criteria->or ) {
                        ret = iodef_criteria_clone(criteria2, &new);
                        if ( ret < 0 )
                                return ret;

                        ret = iodef_criteria_and_criteria(criteria->or, new);
                        if ( ret < 0 )
                                return ret;
                }

                criteria = criteria->and;
        }

        last->and = criteria2;

        return 0;
}


void iodef_criteria_set_negation(iodef_criteria_t *criteria, prelude_bool_t negate)
{
        prelude_return_if_fail(criteria);
        criteria->negated = negate;
}


prelude_bool_t iodef_criteria_get_negation(const iodef_criteria_t *criteria)
{
        prelude_return_val_if_fail(criteria, FALSE);
        return criteria->negated;
}


void iodef_criteria_set_criterion(iodef_criteria_t *criteria, iodef_criterion_t *criterion)
{
        prelude_return_if_fail(criteria);
        prelude_return_if_fail(criterion);

        criteria->criterion = criterion;
}




/**
 * iodef_criteria_match:
 * @criteria: Pointer to a #iodef_criteria_t object.
 * @object: Pointer to a #iodef_object_t object.
 *
 * Matches @object against the provided criteria.
 *
 * Returns: 1 if criteria match, 0 if it did not, a negative value if an error occured.
 */
int iodef_criteria_match(const iodef_criteria_t *criteria, void *object)
{
        int ret;

        prelude_return_val_if_fail(criteria, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(object, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = iodef_criterion_match(criteria->criterion, object);
        if ( ret < 0 )
                return ret;

        if ( ret == 1 && criteria->and )
                ret = iodef_criteria_match(criteria->and, object);

        if ( ret == 0 && criteria->or )
                ret = iodef_criteria_match(criteria->or, object);

        if ( ret < 0 )
                return ret;

        return (criteria->negated) ? !ret : ret;
}
