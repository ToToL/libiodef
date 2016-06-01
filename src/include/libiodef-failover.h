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

#ifndef _LIBIODEF_LIBIODEF_FAILOVER_H
#define _LIBIODEF_LIBIODEF_FAILOVER_H

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct libiodef_failover libiodef_failover_t;

void libiodef_failover_destroy(libiodef_failover_t *failover);

int libiodef_failover_new(libiodef_failover_t **ret, const char *dirname);

void libiodef_failover_set_quota(libiodef_failover_t *failover, size_t limit);

int libiodef_failover_save_msg(libiodef_failover_t *failover, libiodef_msg_t *msg);

ssize_t libiodef_failover_get_saved_msg(libiodef_failover_t *failover, libiodef_msg_t **out);

unsigned long libiodef_failover_get_deleted_msg_count(libiodef_failover_t *failover);

unsigned long libiodef_failover_get_available_msg_count(libiodef_failover_t *failover);

void libiodef_failover_enable_transaction(libiodef_failover_t *failover);

void libiodef_failover_disable_transaction(libiodef_failover_t *failover);

int libiodef_failover_commit(libiodef_failover_t *failover, libiodef_msg_t *msg);

int libiodef_failover_rollback(libiodef_failover_t *failover, libiodef_msg_t *msg);

#ifdef __cplusplus
 }
#endif

#endif
