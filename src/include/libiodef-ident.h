/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
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

#ifndef _LIBIODEF_LIBIODEF_IDENT_H
#define _LIBIODEF_LIBIODEF_IDENT_H

#include "libiodef-inttypes.h"
#include "libiodef-string.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libiodef_ident libiodef_ident_t;

int libiodef_ident_generate(libiodef_ident_t *ident, libiodef_string_t *out);

uint64_t libiodef_ident_inc(libiodef_ident_t *ident);

void libiodef_ident_destroy(libiodef_ident_t *ident);

int libiodef_ident_new(libiodef_ident_t **ret);

#ifdef __cplusplus
 }
#endif

#endif /* _LIBIODEF_LIBIODEF_IDENT_H */
