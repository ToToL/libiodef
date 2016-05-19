/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
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

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "prelude-error.h"
#include "prelude-inttypes.h"
#include "prelude-string.h"
#include "common.h"
#include "iodef-data.h"


/*
 * Data structure may be free'd
 */
#define IODEF_DATA_OWN_STRUCTURE  0x1

/*
 * Data content may be free'd
 */
#define IODEF_DATA_OWN_DATA       0x2


#define IODEF_DATA_DECL(iodef_data_type, c_type, name, fname)         \
int iodef_data_new_ ## name(iodef_data_t **nd, c_type val)            \
{                                                                     \
        int ret;                                                      \
                                                                      \
        ret = iodef_data_new(nd);                                     \
        if ( ret < 0 )                                                \
                return ret;                                           \
                                                                      \
        iodef_data_set_ ## fname(*nd, val);                           \
                                                                      \
        return ret;                                                   \
}                                                                     \
                                                                      \
void iodef_data_set_ ## name(iodef_data_t *ptr, c_type val)           \
{                                                                     \
        prelude_return_if_fail(ptr);                                  \
        iodef_data_destroy_internal(ptr);                             \
        ptr->type = iodef_data_type;                                  \
        ptr->len = sizeof(val);                                       \
        ptr->data.fname ## _data = val;                               \
}                                                                     \
                                                                      \
c_type iodef_data_get_ ## name(const iodef_data_t *ptr)               \
{                                                                     \
        return ptr->data.fname ## _data;                              \
}


IODEF_DATA_DECL(IODEF_DATA_TYPE_CHAR, char, char, char)
IODEF_DATA_DECL(IODEF_DATA_TYPE_BYTE, uint8_t, byte, byte)
IODEF_DATA_DECL(IODEF_DATA_TYPE_INT, uint32_t, uint32, int)
IODEF_DATA_DECL(IODEF_DATA_TYPE_INT, uint64_t, uint64, int)
IODEF_DATA_DECL(IODEF_DATA_TYPE_INT, int64_t, int, int)
IODEF_DATA_DECL(IODEF_DATA_TYPE_FLOAT, float, float, float)


int iodef_data_new(iodef_data_t **data)
{
        *data = calloc(1, sizeof(**data));
        if ( ! *data )
                return prelude_error_from_errno(errno);

        (*data)->refcount = 1;
        (*data)->flags |= IODEF_DATA_OWN_STRUCTURE;

        return 0;
}


int iodef_data_new_time(iodef_data_t **data, iodef_time_t *time)
{
        int ret;

        ret = iodef_data_new(data);
        if ( ret < 0 )
                return ret;

        (*data)->len = 1;
        (*data)->type = IODEF_DATA_TYPE_TIME;
        (*data)->flags |= IODEF_DATA_OWN_DATA;
        (*data)->data.ro_data = time;

        return 0;
}


void iodef_data_set_time(iodef_data_t *data, iodef_time_t *time)
{
        data->len = 1;
        data->type = IODEF_DATA_TYPE_TIME;
        data->flags |= IODEF_DATA_OWN_DATA;
        data->data.ro_data = time;
}

iodef_data_t *iodef_data_ref(iodef_data_t *data)
{
        prelude_return_val_if_fail(data, NULL);

        data->refcount++;
        return data;
}



int iodef_data_set_ptr_ref_fast(iodef_data_t *data, iodef_data_type_t type, const void *ptr, size_t len)
{
        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));

        iodef_data_destroy_internal(data);

        data->type = type;
        data->data.ro_data = ptr;
        data->len = len;

        return 0;
}



int iodef_data_set_ptr_dup_fast(iodef_data_t *data, iodef_data_type_t type, const void *ptr, size_t len)
{
        void *new;

        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));

        iodef_data_destroy_internal(data);

        new = malloc(len);
        if ( ! new )
                return -1;

        memcpy(new, ptr, len);

        data->type = type;
        data->data.rw_data = new;
        data->len = len;
        data->flags |= IODEF_DATA_OWN_DATA;

        return 0;
}



int iodef_data_set_ptr_nodup_fast(iodef_data_t *data, iodef_data_type_t type, void *ptr, size_t len)
{
        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));

        iodef_data_destroy_internal(data);

        data->type = type;
        data->data.rw_data= ptr;
        data->len = len;
        data->flags |= IODEF_DATA_OWN_DATA;

        return 0;
}



int iodef_data_new_ptr_ref_fast(iodef_data_t **data, iodef_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = iodef_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = iodef_data_set_ptr_ref_fast(*data, type, ptr, len);
        if ( ret < 0 )
                iodef_data_destroy(*data);

        return ret;
}



int iodef_data_new_ptr_dup_fast(iodef_data_t **data, iodef_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = iodef_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = iodef_data_set_ptr_dup_fast(*data, type, ptr, len);
        if ( ret < 0 )
                iodef_data_destroy(*data);

        return ret;
}



int iodef_data_new_ptr_nodup_fast(iodef_data_t **data, iodef_data_type_t type, void *ptr, size_t len)
{
        int ret;

        ret = iodef_data_new(data);
        if ( ret < 0 )
                return ret;

        ret = iodef_data_set_ptr_nodup_fast(*data, type, ptr, len);
        if ( ret < 0 )
                iodef_data_destroy(*data);

        return ret;
}




/**
 * iodef_data_copy_ref:
 * @src: Source #iodef_data_t object.
 * @dst: Destination #iodef_data_t object.
 *
 * Makes @dst reference the same buffer as @src.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_data_copy_ref(const iodef_data_t *src, iodef_data_t *dst)
{
        prelude_return_val_if_fail(src, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(dst, prelude_error(PRELUDE_ERROR_ASSERTION));

        iodef_data_destroy_internal(dst);

        dst->type = src->type;
        dst->len = src->len;
        dst->data = src->data;
        dst->flags &= ~IODEF_DATA_OWN_DATA;

        return 0;
}




/**
 * iodef_data_copy_dup:
 * @src: Source #iodef_data_t object.
 * @dst: Destination #iodef_data_t object.
 *
 * Copies @src to @dst, including the associated buffer.
 * This is an alternative to iodef_data_clone().
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_data_copy_dup(const iodef_data_t *src, iodef_data_t *dst)
{
        int ret;

        prelude_return_val_if_fail(src, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(dst, prelude_error(PRELUDE_ERROR_ASSERTION));

        iodef_data_destroy_internal(dst);

        dst->type = src->type;
        dst->flags |= IODEF_DATA_OWN_DATA;
        dst->len = src->len;

        if ( src->type == IODEF_DATA_TYPE_TIME ) {
                ret = iodef_time_clone((iodef_time_t *) src->data.rw_data, (iodef_time_t **) &dst->data.rw_data);
                if ( ret < 0 )
                        return ret;
        }

        else if ( src->type == IODEF_DATA_TYPE_CHAR_STRING || src->type == IODEF_DATA_TYPE_BYTE_STRING ) {
                dst->data.rw_data = malloc(src->len);
                if ( ! dst->data.rw_data )
                        return -1;

                memcpy(dst->data.rw_data, src->data.ro_data, src->len);
        } else {
                dst->data = src->data;
        }

        return 0;
}



int iodef_data_clone(const iodef_data_t *src, iodef_data_t **dst)
{
        int ret;

        prelude_return_val_if_fail(src, prelude_error(PRELUDE_ERROR_ASSERTION));

        ret = iodef_data_new(dst);
        if ( ret < 0 )
                return ret;

        ret = iodef_data_copy_dup(src, *dst);
        if ( ret < 0 )
                iodef_data_destroy(*dst);

        return ret;
}



const char *iodef_data_get_char_string(const iodef_data_t *data)
{
        prelude_return_val_if_fail(data, NULL);
        return data->data.ro_data;
}



const unsigned char *iodef_data_get_byte_string(const iodef_data_t *data)
{
        prelude_return_val_if_fail(data, NULL);
        return data->data.ro_data;
}



/**
 * iodef_data_get_type
 * @data: Pointer to an #iodef_data_t object.
 *
 * Returns: the type of the embedded data.
 */
iodef_data_type_t iodef_data_get_type(const iodef_data_t *data)
{
        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        return data->type;
}




/**
 * iodef_data_get_len:
 * @data: Pointer to an #iodef_data_t object.
 *
 * Returns: the length of data contained within @data object.
 */
size_t iodef_data_get_len(const iodef_data_t *data)
{
        prelude_return_val_if_fail(data, 0);
        return data->len;
}




/**
 * iodef_data_get_data:
 * @data: Pointer to an #iodef_data_t object.
 *
 * Returns: the data contained within @data object.
 */
const void *iodef_data_get_data(const iodef_data_t *data)
{
        prelude_return_val_if_fail(data, NULL);

        switch ( data->type ) {
        case IODEF_DATA_TYPE_UNKNOWN:
                return NULL;

        case IODEF_DATA_TYPE_CHAR_STRING:
        case IODEF_DATA_TYPE_BYTE_STRING:
        case IODEF_DATA_TYPE_TIME:
                return data->data.ro_data;

        default:
                return &data->data;
        }

        return NULL;
}



/**
 * iodef_data_is_empty:
 * @data: Pointer to an #iodef_data_t object.
 *
 * Returns: TRUE if empty, FALSE otherwise.
 */
prelude_bool_t iodef_data_is_empty(const iodef_data_t *data)
{
        prelude_return_val_if_fail(data, TRUE);
        return (data->len == 0) ? TRUE : FALSE;
}



static int bytes_to_string(prelude_string_t *out, const unsigned char *src, size_t size)
{
        char c;
        int ret;
        static const char b64tbl[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        while ( size ) {
                ret = prelude_string_ncat(out, &b64tbl[(src[0] >> 2) & 0x3f], 1);
                if ( ret < 0 )
                        return ret;

                c = b64tbl[((src[0] << 4) + ((--size) ? src[1] >> 4 : 0)) & 0x3f];

                ret = prelude_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                c = (size) ? b64tbl[((src[1] << 2) + ((--size) ? src[2] >> 6 : 0)) & 0x3f] : '=';

                ret = prelude_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                c = (size && size--) ? b64tbl[src[2] & 0x3f] : '=';

                ret = prelude_string_ncat(out, &c, 1);
                if ( ret < 0 )
                        return ret;

                src += 3;
        }

        return 0;
}




/**
 * iodef_data_to_string:
 * @data: Pointer to an #iodef_data_t object.
 * @out: Pointer to a #prelude_string_t to store the formated data into.
 *
 * Formats data contained within @data to be printable,
 * and stores the result in the provided @out buffer.
 *
 * Returns: 0 on success, a negative value if an error occured.
 */
int iodef_data_to_string(const iodef_data_t *data, prelude_string_t *out)
{
        int ret = 0;

        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(out, prelude_error(PRELUDE_ERROR_ASSERTION));

        switch ( data->type ) {
        case IODEF_DATA_TYPE_UNKNOWN:
                return 0;

        case IODEF_DATA_TYPE_CHAR:
                ret = prelude_string_sprintf(out, "%c", data->data.char_data);
                break;

        case IODEF_DATA_TYPE_BYTE:
                /*
                 * %hh convertion specifier is not portable.
                 */
                ret = prelude_string_sprintf(out, "%u", (unsigned int) data->data.byte_data);
                break;

        case IODEF_DATA_TYPE_UINT32:
        case IODEF_DATA_TYPE_INT:
                ret = prelude_string_sprintf(out, "%" PRELUDE_PRId64, data->data.int_data);
                break;

        case IODEF_DATA_TYPE_FLOAT:
                ret = prelude_string_sprintf(out, "%f", data->data.float_data);
                break;

        case IODEF_DATA_TYPE_CHAR_STRING:
                ret = prelude_string_sprintf(out, "%s", (const char *) data->data.ro_data);
                break;

        case IODEF_DATA_TYPE_BYTE_STRING:
                ret = bytes_to_string(out, data->data.ro_data, data->len);
                break;

        case IODEF_DATA_TYPE_TIME:
                ret = iodef_time_to_string(data->data.ro_data, out);
                break;
        }

        return ret;
}



/*
 *  This function cannot be declared static because it is invoked
 *  from iodef-tree-wrap.c
 */
void iodef_data_destroy_internal(iodef_data_t *ptr)
{
        prelude_return_if_fail(ptr);

        if ( ptr->type == IODEF_DATA_TYPE_TIME && ptr->flags & IODEF_DATA_OWN_DATA )
                iodef_time_destroy(ptr->data.rw_data);

        else if ( (ptr->type == IODEF_DATA_TYPE_CHAR_STRING || ptr->type == IODEF_DATA_TYPE_BYTE_STRING) &&
             ptr->flags & IODEF_DATA_OWN_DATA ) {
                free(ptr->data.rw_data);
                ptr->data.rw_data = NULL;
        }

        /*
         * free() should be done by the caller
         */
}




/**
 * iodef_data_destroy:
 * @data: Pointer to an #iodef_data_t object.
 *
 * Frees @data. The buffer pointed by @data will be freed if
 * the @data object is marked as _dup or _nodup.
 */
void iodef_data_destroy(iodef_data_t *data)
{
        prelude_return_if_fail(data);

        if ( --data->refcount )
                return;

        iodef_data_destroy_internal(data);

        if ( data->flags & IODEF_DATA_OWN_STRUCTURE )
                free(data);
}


int iodef_data_new_char_string_ref_fast(iodef_data_t **data, const char *ptr, size_t len)
{
        return iodef_data_new_ptr_ref_fast(data, IODEF_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodef_data_new_char_string_dup_fast(iodef_data_t **data, const char *ptr, size_t len)
{
        return iodef_data_new_ptr_dup_fast(data, IODEF_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodef_data_new_char_string_nodup_fast(iodef_data_t **data, char *ptr, size_t len)
{
        return iodef_data_new_ptr_nodup_fast(data, IODEF_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodef_data_set_char_string_ref_fast(iodef_data_t *data, const char *ptr, size_t len)
{
        return iodef_data_set_ptr_ref_fast(data, IODEF_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodef_data_set_char_string_dup_fast(iodef_data_t *data, const char *ptr, size_t len)
{
        return iodef_data_set_ptr_dup_fast(data, IODEF_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}

int iodef_data_set_char_string_nodup_fast(iodef_data_t *data, char *ptr, size_t len)
{
        return iodef_data_set_ptr_nodup_fast(data, IODEF_DATA_TYPE_CHAR_STRING, ptr, len + 1);
}


int iodef_data_new_char_string_ref(iodef_data_t **data, const char *ptr)
{
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));
        return iodef_data_new_char_string_ref_fast(data, ptr, strlen(ptr));
}

int iodef_data_new_char_string_dup(iodef_data_t **data, const char *ptr)
{
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));
        return iodef_data_new_char_string_dup_fast(data, ptr, strlen(ptr));
}

int iodef_data_new_char_string_nodup(iodef_data_t **data, char *ptr)
{
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));
        return iodef_data_new_char_string_nodup_fast(data, ptr, strlen(ptr));
}

int iodef_data_set_char_string_ref(iodef_data_t *data, const char *ptr)
{
        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));

        return iodef_data_set_char_string_ref_fast(data, ptr, strlen(ptr));
}

int iodef_data_set_char_string_dup(iodef_data_t *data, const char *ptr)
{
        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));

        return iodef_data_set_char_string_dup_fast(data, ptr, strlen(ptr));
}

int iodef_data_set_char_string_nodup(iodef_data_t *data, char *ptr)
{
        prelude_return_val_if_fail(data, prelude_error(PRELUDE_ERROR_ASSERTION));
        prelude_return_val_if_fail(ptr, prelude_error(PRELUDE_ERROR_ASSERTION));

        return iodef_data_set_char_string_nodup_fast(data, ptr, strlen(ptr));
}


/*
 *
 */
int iodef_data_new_byte_string_ref(iodef_data_t **data, const unsigned char *ptr, size_t len)
{
        return iodef_data_new_ptr_ref_fast(data, IODEF_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodef_data_new_byte_string_dup(iodef_data_t **data, const unsigned char *ptr, size_t len)
{
        return iodef_data_new_ptr_dup_fast(data, IODEF_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodef_data_new_byte_string_nodup(iodef_data_t **data, unsigned char *ptr, size_t len)
{
        return iodef_data_new_ptr_nodup_fast(data, IODEF_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodef_data_set_byte_string_ref(iodef_data_t *data, const unsigned char *ptr, size_t len)
{
        return iodef_data_set_ptr_ref_fast(data, IODEF_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodef_data_set_byte_string_dup(iodef_data_t *data, const unsigned char *ptr, size_t len)
{
        return iodef_data_set_ptr_dup_fast(data, IODEF_DATA_TYPE_BYTE_STRING, ptr, len);
}


int iodef_data_set_byte_string_nodup(iodef_data_t *data, unsigned char *ptr, size_t len)
{
        return iodef_data_set_ptr_nodup_fast(data, IODEF_DATA_TYPE_BYTE_STRING, ptr, len);
}



int iodef_data_compare(const iodef_data_t *data1, const iodef_data_t *data2)
{
        if ( ! data1 && ! data2 )
                return 0;

        else if ( ! data1 || ! data2 )
                return (data1) ? 1 : -1;

        else if ( data1->len != data2->len )
                return (data1->len > data2->len) ? 1 : -1;

        else if ( data1->type != data2->type )
                return -1;

        if ( data1->type == IODEF_DATA_TYPE_TIME )
                return iodef_time_compare(data1->data.ro_data, data2->data.ro_data);

        else if ( data1->type == IODEF_DATA_TYPE_CHAR_STRING || data1->type == IODEF_DATA_TYPE_BYTE_STRING )
                return memcmp(data1->data.ro_data, data2->data.ro_data, data1->len);

        else
                return memcmp(&data1->data.char_data, &data2->data.char_data, data1->len);
}
