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

#include "libmissing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "prelude-log.h"
#include "prelude-string.h"
#include "prelude-error.h"
#include "prelude-inttypes.h"
#include "iodef.h"
#include "iodef-tree-wrap.h"
#include "iodef-additional-data.h"
#include "iodef-document-id.h"





#define IODEF_ADDITIONAL_DATA_ACCESSOR(name, type)                                                              \
int iodef_additional_data_new_ ## name ## _ref_fast(iodef_additional_data_t **ad, const char *data, size_t len) \
{                                                                                                               \
        return iodef_additional_data_new_ptr_ref_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_new_ ## name ## _ref(iodef_additional_data_t **ad, const char *data)                  \
{                                                                                                               \
        return iodef_additional_data_new_ ## name ## _ref_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_set_ ## name ## _ref_fast(iodef_additional_data_t *ad, const char *data, size_t len)  \
{                                                                                                               \
        return iodef_additional_data_set_ptr_ref_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_set_ ## name ## _ref(iodef_additional_data_t *ad, const char *data)                   \
{                                                                                                               \
        return iodef_additional_data_set_ ## name ## _ref_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_new_ ## name ## _dup_fast(iodef_additional_data_t **ad, const char *data, size_t len) \
{                                                                                                               \
        return iodef_additional_data_new_ptr_dup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_new_ ## name ## _dup(iodef_additional_data_t **ad, const char *data)                  \
{                                                                                                               \
        return iodef_additional_data_new_ ## name ## _dup_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_set_ ## name ## _dup_fast(iodef_additional_data_t *ad, const char *data, size_t len)  \
{                                                                                                               \
        return iodef_additional_data_set_ptr_dup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);  \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_set_ ## name ## _dup(iodef_additional_data_t *ad, const char *data)                   \
{                                                                                                               \
        return iodef_additional_data_set_ ## name ## _dup_fast(ad, data, strlen(data));                         \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_new_ ## name ## _nodup_fast(iodef_additional_data_t **ad, char *data, size_t len)     \
{                                                                                                               \
        return iodef_additional_data_new_ptr_nodup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);\
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_new_ ## name ## _nodup(iodef_additional_data_t **ad, char *data)                      \
{                                                                                                               \
        return iodef_additional_data_new_ ## name ## _nodup_fast(ad, data, strlen(data));                       \
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_set_ ## name ## _nodup_fast(iodef_additional_data_t *ad, char *data, size_t len)      \
{                                                                                                               \
        return iodef_additional_data_set_ptr_nodup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_ ## type, data, len + 1);\
}                                                                                                               \
                                                                                                                \
int iodef_additional_data_set_ ## name ## _nodup(iodef_additional_data_t *ad, char *data)                       \
{                                                                                                               \
        return iodef_additional_data_set_ ## name ## _nodup_fast(ad, data, strlen(data));                       \
}


#define IODEF_ADDITIONAL_DATA_SIMPLE(d_type, d_name, ad_type, c_type, name)                \
int iodef_additional_data_new_ ## name(iodef_additional_data_t **ret, c_type val)        \
{                                                                                       \
        int retval;                                                                     \
                                                                                        \
        retval = iodef_additional_data_new(ret);                                               \
        if ( retval < 0 )                                                                      \
                return retval;                                                                \
                                                                                        \
        iodef_additional_data_set_type(*ret, ad_type);                                        \
        iodef_data_set_ ## d_name(iodef_additional_data_get_data(*ret), val);                \
                                                                                        \
        return retval;                                                                        \
}                                                                                        \
                                                                                        \
void iodef_additional_data_set_ ## name(iodef_additional_data_t *ptr, c_type val)        \
{                                                                                        \
        iodef_additional_data_set_type(ptr, ad_type);                                        \
        iodef_data_set_ ## d_name(iodef_additional_data_get_data(ptr), val);                \
}                                                                                        \
                                                                                        \
c_type iodef_additional_data_get_ ## name(iodef_additional_data_t *ptr)                        \
{                                                                                        \
        return iodef_data_get_ ## d_name(iodef_additional_data_get_data(ptr));                \
}



/*
 * Backward compatibility stuff, remove once 0.9.0 is released.
 */
int iodef_additional_data_new_ptr_ref_fast(iodef_additional_data_t **nd,
                                           iodef_additional_data_type_t type,
                                           const void *ptr, size_t len);

int iodef_additional_data_new_ptr_dup_fast(iodef_additional_data_t **nd,
                                           iodef_additional_data_type_t type,
                                           const void *ptr, size_t len);

int iodef_additional_data_new_ptr_nodup_fast(iodef_additional_data_t **nd,
                                             iodef_additional_data_type_t type,
                                             void *ptr, size_t len);

int iodef_additional_data_set_ptr_ref_fast(iodef_additional_data_t *data,
                                           iodef_additional_data_type_t type, const void *ptr, size_t len);

int iodef_additional_data_set_ptr_dup_fast(iodef_additional_data_t *data,
                                           iodef_additional_data_type_t type, const void *ptr, size_t len);

int iodef_additional_data_set_ptr_nodup_fast(iodef_additional_data_t *data,
                                             iodef_additional_data_type_t type, void *ptr, size_t len);



static const struct {
        iodef_additional_data_type_t ad_type;
        iodef_data_type_t d_type;
        size_t len;
} iodef_additional_data_type_table[] = {
        { IODEF_ADDITIONAL_DATA_TYPE_STRING, IODEF_DATA_TYPE_CHAR_STRING, 0 },
        { IODEF_ADDITIONAL_DATA_TYPE_BYTE, IODEF_DATA_TYPE_BYTE, sizeof(uint8_t) },
        { IODEF_ADDITIONAL_DATA_TYPE_CHARACTER, IODEF_DATA_TYPE_CHAR, sizeof(char) },
        { IODEF_ADDITIONAL_DATA_TYPE_DATE_TIME, IODEF_DATA_TYPE_CHAR_STRING, 0 },
        { IODEF_ADDITIONAL_DATA_TYPE_INTEGER, IODEF_DATA_TYPE_INT, sizeof(uint64_t) },
        { IODEF_ADDITIONAL_DATA_TYPE_NTPSTAMP, IODEF_DATA_TYPE_INT, sizeof(uint64_t) },
        { IODEF_ADDITIONAL_DATA_TYPE_PORTLIST, IODEF_DATA_TYPE_CHAR_STRING, 0 },
        { IODEF_ADDITIONAL_DATA_TYPE_REAL, IODEF_DATA_TYPE_FLOAT, sizeof(float) },
        { IODEF_ADDITIONAL_DATA_TYPE_BOOLEAN, IODEF_DATA_TYPE_BYTE, sizeof(prelude_bool_t) },
        { IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, IODEF_DATA_TYPE_BYTE_STRING, 0 },
        { IODEF_ADDITIONAL_DATA_TYPE_XML, IODEF_DATA_TYPE_CHAR_STRING, 0 }
};



static int check_type(iodef_additional_data_type_t type, const unsigned char *buf, size_t len)
{
        if ( type < 0 || (size_t) type >= sizeof(iodef_additional_data_type_table) / sizeof(*iodef_additional_data_type_table) )
                return prelude_error_verbose(PRELUDE_ERROR_GENERIC, "Invalid IODEFAdditionalData type specified");

        if ( iodef_additional_data_type_table[type].len != 0 &&
             len > iodef_additional_data_type_table[type].len )
                return prelude_error_verbose(PRELUDE_ERROR_GENERIC, "Provided value length does not match specified type length");

        if ( iodef_additional_data_type_table[type].len == 0 && len < 1 )
                return prelude_error_verbose(PRELUDE_ERROR_GENERIC, "Invalid value length for this type");

        if ( type == IODEF_ADDITIONAL_DATA_TYPE_STRING ||
             type == IODEF_ADDITIONAL_DATA_TYPE_DATE_TIME ||
             type == IODEF_ADDITIONAL_DATA_TYPE_PORTLIST ||
             type == IODEF_ADDITIONAL_DATA_TYPE_XML )
                return buf[len - 1] == '\0' ? 0 : prelude_error_verbose(PRELUDE_ERROR_GENERIC, "String is not nul terminated");

        return 0;
}



static iodef_data_type_t iodef_additional_data_type_to_data_type(iodef_additional_data_type_t type)
{
        if ( type < 0 || (size_t) type >= sizeof(iodef_additional_data_type_table) / sizeof(*iodef_additional_data_type_table) )
                return IODEF_DATA_TYPE_UNKNOWN;

        return iodef_additional_data_type_table[type].d_type;
}



int iodef_additional_data_new_ptr_ref_fast(iodef_additional_data_t **nd,
                                           iodef_additional_data_type_t type,
                                           const void *ptr, size_t len)
{
        int ret;
        iodef_data_type_t dtype;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        ret = iodef_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(*nd, type);
        dtype = iodef_additional_data_type_to_data_type(type);

        ret = iodef_data_set_ptr_ref_fast(iodef_additional_data_get_data(*nd), dtype, ptr, len);
        if ( ret < 0 ) {
                iodef_additional_data_destroy(*nd);
                return ret;
        }

        return 0;
}



int iodef_additional_data_new_ptr_dup_fast(iodef_additional_data_t **nd,
                                           iodef_additional_data_type_t type,
                                           const void *ptr, size_t len)
{
        int ret;
        iodef_data_type_t dtype;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        ret = iodef_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(*nd, type);
        dtype = iodef_additional_data_type_to_data_type(type);

        ret = iodef_data_set_ptr_dup_fast(iodef_additional_data_get_data(*nd), dtype, ptr, len);
        if ( ret < 0 ) {
                iodef_additional_data_destroy(*nd);
                return ret;
        }

        return 0;
}



int iodef_additional_data_new_ptr_nodup_fast(iodef_additional_data_t **nd,
                                             iodef_additional_data_type_t type,
                                             void *ptr, size_t len)
{
        int ret;
        iodef_data_type_t dtype;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        ret = iodef_additional_data_new(nd);
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(*nd, type);
        dtype = iodef_additional_data_type_to_data_type(type);

        ret = iodef_data_set_ptr_nodup_fast(iodef_additional_data_get_data(*nd), dtype, ptr, len);
        if ( ret < 0 ) {
                iodef_additional_data_destroy(*nd);
                return ret;
        }

        return ret;
}



int iodef_additional_data_set_ptr_ref_fast(iodef_additional_data_t *data,
                                           iodef_additional_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(data, type);

        return iodef_data_set_ptr_ref_fast(iodef_additional_data_get_data(data),
                                           iodef_additional_data_type_to_data_type(type), ptr, len);
}



int iodef_additional_data_set_ptr_dup_fast(iodef_additional_data_t *data,
                                           iodef_additional_data_type_t type, const void *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(data, type);

        return iodef_data_set_ptr_dup_fast(iodef_additional_data_get_data(data),
                                           iodef_additional_data_type_to_data_type(type), ptr, len);
}



int iodef_additional_data_set_ptr_nodup_fast(iodef_additional_data_t *data,
                                             iodef_additional_data_type_t type, void *ptr, size_t len)
{
        int ret;

        ret = check_type(type, ptr, len);
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(data, type);

        return iodef_data_set_ptr_nodup_fast(iodef_additional_data_get_data(data),
                                             iodef_additional_data_type_to_data_type(type), ptr, len);
}




/*
 * Declare stuff
 */
IODEF_ADDITIONAL_DATA_SIMPLE(IODEF_DATA_TYPE_FLOAT, float, IODEF_ADDITIONAL_DATA_TYPE_REAL, float, real)
IODEF_ADDITIONAL_DATA_SIMPLE(IODEF_DATA_TYPE_INT, int, IODEF_ADDITIONAL_DATA_TYPE_INTEGER, uint32_t, integer)
IODEF_ADDITIONAL_DATA_SIMPLE(IODEF_DATA_TYPE_BYTE, byte, IODEF_ADDITIONAL_DATA_TYPE_BOOLEAN, prelude_bool_t, boolean)
IODEF_ADDITIONAL_DATA_SIMPLE(IODEF_DATA_TYPE_BYTE, byte, IODEF_ADDITIONAL_DATA_TYPE_BYTE, uint8_t, byte)
IODEF_ADDITIONAL_DATA_SIMPLE(IODEF_DATA_TYPE_CHAR, char, IODEF_ADDITIONAL_DATA_TYPE_CHARACTER, char, character)

IODEF_ADDITIONAL_DATA_ACCESSOR(xml, XML)
IODEF_ADDITIONAL_DATA_ACCESSOR(string, STRING)
IODEF_ADDITIONAL_DATA_ACCESSOR(ntpstamp, NTPSTAMP)
IODEF_ADDITIONAL_DATA_ACCESSOR(portlist, PORTLIST)
IODEF_ADDITIONAL_DATA_ACCESSOR(date_time, DATE_TIME)



/*
 * just make a pointer copy of the embedded data
 */
int iodef_additional_data_copy_ref(iodef_additional_data_t *src, iodef_additional_data_t *dst)
{
        int ret;

        ret = prelude_string_copy_ref(iodef_additional_data_get_meaning(src), iodef_additional_data_get_meaning(dst));
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(dst, iodef_additional_data_get_type(src));

        return iodef_data_copy_ref(iodef_additional_data_get_data(src), iodef_additional_data_get_data(dst));
}



/*
 * also copy the content of the embedded data
 */
int iodef_additional_data_copy_dup(iodef_additional_data_t *src, iodef_additional_data_t *dst)
{
        int ret;

        ret = prelude_string_copy_dup(iodef_additional_data_get_meaning(src), iodef_additional_data_get_meaning(dst));
        if ( ret < 0 )
                return ret;

        iodef_additional_data_set_type(dst, iodef_additional_data_get_type(src));

        return iodef_data_copy_dup(iodef_additional_data_get_data(src), iodef_additional_data_get_data(dst));
}



size_t iodef_additional_data_get_len(iodef_additional_data_t *data)
{
        return iodef_data_get_len(iodef_additional_data_get_data(data));
}



prelude_bool_t iodef_additional_data_is_empty(iodef_additional_data_t *data)
{
        return iodef_data_is_empty(iodef_additional_data_get_data(data));
}



int iodef_additional_data_data_to_string(iodef_additional_data_t *ad, prelude_string_t *out)
{
        int ret;
        uint64_t i;
        iodef_data_t *data;

        data = iodef_additional_data_get_data(ad);
        if ( iodef_data_is_empty(data) )
                return 0;

        switch ( iodef_additional_data_get_type(ad) ) {

        case IODEF_ADDITIONAL_DATA_TYPE_NTPSTAMP:
                i = iodef_data_get_int(data);
                ret = prelude_string_sprintf(out, "0x%" PRELUDE_PRIx32 ".0x%" PRELUDE_PRIx32, (uint32_t) (i >> 32), (uint32_t) i);
                break;

        default:
                ret = iodef_data_to_string(data, out);
                break;
        }

        return ret;
}


/*
 * byte-string specific stuff
 */
int iodef_additional_data_new_byte_string_ref(iodef_additional_data_t **ad, const unsigned char *data, size_t len)
{
        return iodef_additional_data_new_ptr_ref_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int iodef_additional_data_set_byte_string_ref(iodef_additional_data_t *ad, const unsigned char *data, size_t len)
{
         return iodef_additional_data_set_ptr_ref_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int iodef_additional_data_new_byte_string_dup(iodef_additional_data_t **ad, const unsigned char *data, size_t len)
{
        return iodef_additional_data_new_ptr_dup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int iodef_additional_data_set_byte_string_dup(iodef_additional_data_t *ad, const unsigned char *data, size_t len)
{
        return iodef_additional_data_set_ptr_dup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int iodef_additional_data_new_byte_string_nodup(iodef_additional_data_t **ad, unsigned char *data, size_t len)
{
        return iodef_additional_data_new_ptr_nodup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

int iodef_additional_data_set_byte_string_nodup(iodef_additional_data_t *ad, unsigned char *data, size_t len)
{
        return iodef_additional_data_set_ptr_nodup_fast(ad, IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING, data, len);
}

