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

#ifndef _LIBIODEF_LIBIODEF_DOCUMENT_ID_H
#define _LIBIODEF_LIBIODEF_DOCUMENT_ID_H

/*
 * Top level message tag (to be used with libiodef_msg_new()).
 */
#define LIBIODEF_MSG_IODEF          0
#define LIBIODEF_MSG_ID             3
#define LIBIODEF_MSG_AUTH           4
#define LIBIODEF_MSG_CM             5 
#define LIBIODEF_MSG_CONNECTION_CAPABILITY    6
#define LIBIODEF_MSG_OPTION_REQUEST 7
#define LIBIODEF_MSG_OPTION_REPLY   8

/*
 * LIBIODEF_MSG_ID submessage
 */
#define LIBIODEF_MSG_ID_DECLARE  0

/*
 * authentication msg
 */
#define LIBIODEF_MSG_AUTH_SUCCEED   6
#define LIBIODEF_MSG_AUTH_FAILED    7


/*
 * LIBIODEF_MSG_CM submessages
 */
#define LIBIODEF_MSG_CM_FIREWALL 0
#define LIBIODEF_MSG_CM_THROTTLE 1
#define LIBIODEF_MSG_CM_ISLAND   2
#define LIBIODEF_MSG_CM_FEATURE  3

/*
 * LIBIODEF_MSG_OPTION submessages
 */
#define LIBIODEF_MSG_OPTION_TARGET_ID        0
#define LIBIODEF_MSG_OPTION_LIST             2
#define LIBIODEF_MSG_OPTION_VALUE            3
#define LIBIODEF_MSG_OPTION_SET              4
#define LIBIODEF_MSG_OPTION_GET              5
#define LIBIODEF_MSG_OPTION_REQUEST_ID       6
#define LIBIODEF_MSG_OPTION_ERROR            7
#define LIBIODEF_MSG_OPTION_START            8
#define LIBIODEF_MSG_OPTION_END              9
#define LIBIODEF_MSG_OPTION_NAME            10
#define LIBIODEF_MSG_OPTION_DESC            11
#define LIBIODEF_MSG_OPTION_HAS_ARG         12
#define LIBIODEF_MSG_OPTION_HELP            13
#define LIBIODEF_MSG_OPTION_INPUT_VALIDATION 14
#define LIBIODEF_MSG_OPTION_INPUT_TYPE       15
#define LIBIODEF_MSG_OPTION_ARG              16
#define LIBIODEF_MSG_OPTION_TYPE             17
#define LIBIODEF_MSG_OPTION_DESTROY          18
#define LIBIODEF_MSG_OPTION_COMMIT           19
#define LIBIODEF_MSG_OPTION_HOP              20
#define LIBIODEF_MSG_OPTION_TARGET_INSTANCE_ID 21

#endif /* _LIBIODEF_LIBIODEF_DOCUMENT_ID_H */
