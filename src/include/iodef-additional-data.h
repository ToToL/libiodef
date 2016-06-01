/*****
*
* Copyright (C) 2004-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodef-ids.com>
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

#ifndef _LIBIODEF_IODEF_ADDITIONAL_DATA_H
#define _LIBIODEF_IODEF_ADDITIONAL_DATA_H

#ifdef __cplusplus
 extern "C" {
#endif
         
/*
 * basic type
 */
int iodef_additional_data_new_real(iodef_additional_data_t **ret, float data);
int iodef_additional_data_new_byte(iodef_additional_data_t **ret, uint8_t byte);
int iodef_additional_data_new_integer(iodef_additional_data_t **ret, uint32_t data);
int iodef_additional_data_new_boolean(iodef_additional_data_t **ret, libiodef_bool_t data);
int iodef_additional_data_new_character(iodef_additional_data_t **ret, char data);

void iodef_additional_data_set_real(iodef_additional_data_t *ptr, float data);
void iodef_additional_data_set_byte(iodef_additional_data_t *ptr, uint8_t byte);
void iodef_additional_data_set_integer(iodef_additional_data_t *ptr, uint32_t data);
void iodef_additional_data_set_boolean(iodef_additional_data_t *ptr, libiodef_bool_t data);
void iodef_additional_data_set_character(iodef_additional_data_t *ptr, char data);


#define _IODEF_ADDITIONAL_DATA_DECL(name) \
int iodef_additional_data_new_ ## name ## _ref_fast(iodef_additional_data_t **ad, const char *data, size_t len); \
int iodef_additional_data_new_ ## name ## _ref(iodef_additional_data_t **ad, const char *data); \
int iodef_additional_data_set_ ## name ## _ref_fast(iodef_additional_data_t *ad, const char *data, size_t len); \
int iodef_additional_data_set_ ## name ## _ref(iodef_additional_data_t *ad, const char *data); \
int iodef_additional_data_new_ ## name ## _dup_fast(iodef_additional_data_t **ad, const char *data, size_t len); \
int iodef_additional_data_new_ ## name ## _dup(iodef_additional_data_t **ad, const char *data); \
int iodef_additional_data_set_ ## name ## _dup_fast(iodef_additional_data_t *ad, const char *data, size_t len); \
int iodef_additional_data_set_ ## name ## _dup(iodef_additional_data_t *ad, const char *data); \
int iodef_additional_data_new_ ## name ## _nodup_fast(iodef_additional_data_t **ad, char *data, size_t len); \
int iodef_additional_data_new_ ## name ## _nodup(iodef_additional_data_t **ad, char *data); \
int iodef_additional_data_set_ ## name ## _nodup_fast(iodef_additional_data_t *ad, char *data, size_t len); \
int iodef_additional_data_set_ ## name ## _nodup(iodef_additional_data_t *ad, char *data);

_IODEF_ADDITIONAL_DATA_DECL(string)
_IODEF_ADDITIONAL_DATA_DECL(ntpstamp)
_IODEF_ADDITIONAL_DATA_DECL(date_time)
_IODEF_ADDITIONAL_DATA_DECL(portlist)
_IODEF_ADDITIONAL_DATA_DECL(xml)

int iodef_additional_data_new_byte_string_ref(iodef_additional_data_t **ad, const unsigned char *data, size_t len);
int iodef_additional_data_set_byte_string_ref(iodef_additional_data_t *ad, const unsigned char *data, size_t len);
int iodef_additional_data_new_byte_string_dup(iodef_additional_data_t **ad, const unsigned char *data, size_t len);
int iodef_additional_data_set_byte_string_dup(iodef_additional_data_t *ad, const unsigned char *data, size_t len);
int iodef_additional_data_new_byte_string_nodup(iodef_additional_data_t **ad, unsigned char *data, size_t len);
int iodef_additional_data_set_byte_string_nodup(iodef_additional_data_t *ad, unsigned char *data, size_t len);
         

         
/*
 * copy / clone / destroy
 */
int iodef_additional_data_copy_ref(iodef_additional_data_t *src, iodef_additional_data_t *dst);
int iodef_additional_data_copy_dup(iodef_additional_data_t *src, iodef_additional_data_t *dst);

/*
 * Accessors
 */

float iodef_additional_data_get_real(iodef_additional_data_t *data);
uint32_t iodef_additional_data_get_integer(iodef_additional_data_t *data);
libiodef_bool_t iodef_additional_data_get_boolean(iodef_additional_data_t *data);
char iodef_additional_data_get_character(iodef_additional_data_t *data);
uint8_t iodef_additional_data_get_byte(iodef_additional_data_t *data);

size_t iodef_additional_data_get_len(iodef_additional_data_t *data);

libiodef_bool_t iodef_additional_data_is_empty(iodef_additional_data_t *data);

int iodef_additional_data_data_to_string(iodef_additional_data_t *ad, libiodef_string_t *out);

#ifdef __cplusplus
 }
#endif
         
#endif /* _LIBIODEF_IODEF_DATA_H */
