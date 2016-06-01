/*****
*
* Copyright (C) 2003-2016 CS-SI. All Rights Reserved.
* Author: Nicolas Delon <nicolas.delon@libiodef-ids.com>
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

#ifndef _LIBIODEF_IODEF_TIME_H
#define _LIBIODEF_IODEF_TIME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef __cplusplus
 extern "C" {
#endif

struct iodef_time {
        /* <private> */
        int refcount;
        uint32_t sec;
        uint32_t usec;
        int32_t gmt_offset;
};

typedef struct iodef_time iodef_time_t;

iodef_time_t *iodef_time_ref(iodef_time_t *time);
int iodef_time_new(iodef_time_t **time);

int iodef_time_new_from_time(iodef_time_t **time, const time_t *t);
int iodef_time_new_from_gettimeofday(iodef_time_t **time);
int iodef_time_new_from_string(iodef_time_t **time, const char *buf);
int iodef_time_new_from_ntpstamp(iodef_time_t **time, const char *buf);
int iodef_time_new_from_timeval(iodef_time_t **time, const struct timeval *tv);

void iodef_time_set_from_time(iodef_time_t *time, const time_t *t);
int iodef_time_set_from_gettimeofday(iodef_time_t *time);
int iodef_time_set_from_string(iodef_time_t *time, const char *buf);
int iodef_time_set_from_ntpstamp(iodef_time_t *time, const char *buf);
int iodef_time_set_from_timeval(iodef_time_t *time, const struct timeval *tv);

void iodef_time_destroy_internal(iodef_time_t *time);
void iodef_time_destroy(iodef_time_t *time);

int iodef_time_clone(const iodef_time_t *src, iodef_time_t **dst);
int iodef_time_copy(const iodef_time_t *src, iodef_time_t *dst);

void iodef_time_set_sec(iodef_time_t *time, uint32_t sec);
void iodef_time_set_usec(iodef_time_t *time, uint32_t usec);
void iodef_time_set_gmt_offset(iodef_time_t *time, int32_t gmtoff);

uint32_t iodef_time_get_sec(const iodef_time_t *time);
uint32_t iodef_time_get_usec(const iodef_time_t *time);
int32_t iodef_time_get_gmt_offset(const iodef_time_t *time);

int iodef_time_to_string(const iodef_time_t *time, libiodef_string_t *out);
int iodef_time_to_ntpstamp(const iodef_time_t *time, libiodef_string_t *out);

int iodef_time_compare(const iodef_time_t *time1, const iodef_time_t *time2);
         
#ifdef __cplusplus
 }
#endif
         
#endif /* _LIBIODEF_IODEF_TIME_H */
