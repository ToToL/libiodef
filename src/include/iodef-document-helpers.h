/*****
*
* Copyright (C) 2008-2016 CS-SI. All Rights Reserved.
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


int iodef_document_set_value(iodef_document_t *message, const char *path, iodef_value_t *value);

int iodef_document_get_value(iodef_document_t *message, const char *path, iodef_value_t **value);

int iodef_document_set_string(iodef_document_t *message, const char *path, const char *value);

int iodef_document_get_string(iodef_document_t *message, const char *path, char **result);

int iodef_document_set_number(iodef_document_t *message, const char *path, double number);

int iodef_document_get_number(iodef_document_t *message, const char *path, double *result);

int iodef_document_set_data(iodef_document_t *message, const char *path, const unsigned char *data, size_t size);

int iodef_document_get_data(iodef_document_t *message, const char *path, unsigned char **data, size_t *size);
