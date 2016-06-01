/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEF_LIBIODEF_LOG_H
#define _LIBIODEF_LIBIODEF_LOG_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libiodef-macros.h"
#include <stdarg.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum {
        LIBIODEF_LOG_CRIT  = -1,
        LIBIODEF_LOG_ERR   =  0,
        LIBIODEF_LOG_WARN  =  1,
        LIBIODEF_LOG_INFO  =  2,
        LIBIODEF_LOG_DEBUG  = 3
} libiodef_log_t;


typedef enum {
        LIBIODEF_LOG_FLAGS_QUIET  = 0x01, /* Drop LIBIODEF_LOG_PRIORITY_INFO */
        LIBIODEF_LOG_FLAGS_SYSLOG = 0x02
} libiodef_log_flags_t;



void _libiodef_log_v(libiodef_log_t level, const char *file,
                    const char *function, int line, const char *fmt, va_list ap)
                    LIBIODEF_FMT_PRINTF(5, 0);

void _libiodef_log(libiodef_log_t level, const char *file,
                  const char *function, int line, const char *fmt, ...)
                  LIBIODEF_FMT_PRINTF(5, 6);


#ifdef HAVE_VARIADIC_MACROS

#define libiodef_log(level, ...) \
        _libiodef_log(level, __FILE__, __LIBIODEF_FUNC__, __LINE__, __VA_ARGS__)

#define libiodef_log_debug(level, ...) \
        _libiodef_log(LIBIODEF_LOG_DEBUG + level, __FILE__, __LIBIODEF_FUNC__, __LINE__, __VA_ARGS__)
#else

void libiodef_log(libiodef_log_t level, const char *fmt, ...)
                 LIBIODEF_FMT_PRINTF(2, 3);

void libiodef_log_debug(libiodef_log_t level, const char *fmt, ...)
                       LIBIODEF_FMT_PRINTF(2, 3);

#endif


#define libiodef_log_v(level, fmt, ap) \
        _libiodef_log_v(level, __FILE__, __LIBIODEF_FUNC__, __LINE__, fmt, ap)

#define libiodef_log_debug_v(level, fmt, ap) \
        _libiodef_log_v(LIBIODEF_LOG_DEBUG + level, __FILE__, __LIBIODEF_FUNC__, __LINE__, fmt, ap)


void libiodef_log_set_level(libiodef_log_t level);

void libiodef_log_set_debug_level(int level);

libiodef_log_flags_t libiodef_log_get_flags(void);

void libiodef_log_set_flags(libiodef_log_flags_t flags);

char *libiodef_log_get_prefix(void);

void libiodef_log_set_prefix(char *prefix);

void libiodef_log_set_callback(void log_cb(libiodef_log_t level, const char *str));

int libiodef_log_set_logfile(const char *filename);

void _libiodef_log_set_abort_level(libiodef_log_t level);

int _libiodef_log_set_abort_level_from_string(const char *level);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEF_LIBIODEF_LOG_H */
