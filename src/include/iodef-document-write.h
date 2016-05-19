
/*****
*
* Copyright (C) 2001-2016 CS-SI. All Rights Reserved.
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

/* Auto-generated by the GenerateIODEFMessageWriteH package */

#ifndef _LIBPRELUDE_IODEF_DOCUMENT_WRITE_H
#define _LIBPRELUDE_IODEF_DOCUMENT_WRITE_H

#include "prelude-inttypes.h"
#include "iodef-time.h"
#include "prelude-string.h"
#include "prelude-io.h"

#ifdef __cplusplus
 extern "C" {
#endif

int iodef_additional_data_write(iodef_additional_data_t *additional_data, prelude_io_t *fd);
int iodef_email_write(iodef_email_t *email, prelude_io_t *fd);
int iodef_registry_handle_write(iodef_registry_handle_t *registry_handle, prelude_io_t *fd);
int iodef_postal_address_write(iodef_postal_address_t *postal_address, prelude_io_t *fd);
int iodef_contact_write(iodef_contact_t *contact, prelude_io_t *fd);
int iodef_incident_id_write(iodef_incident_id_t *incident_id, prelude_io_t *fd);
int iodef_alternative_id_write(iodef_alternative_id_t *alternative_id, prelude_io_t *fd);
int iodef_related_activity_write(iodef_related_activity_t *related_activity, prelude_io_t *fd);
int iodef_history_item_write(iodef_history_item_t *history_item, prelude_io_t *fd);
int iodef_history_write(iodef_history_t *history, prelude_io_t *fd);
int iodef_expectation_write(iodef_expectation_t *expectation, prelude_io_t *fd);
int iodef_record_pattern_write(iodef_record_pattern_t *record_pattern, prelude_io_t *fd);
int iodef_record_item_write(iodef_record_item_t *record_item, prelude_io_t *fd);
int iodef_application_write(iodef_application_t *application, prelude_io_t *fd);
int iodef_record_data_write(iodef_record_data_t *record_data, prelude_io_t *fd);
int iodef_record_write(iodef_record_t *record, prelude_io_t *fd);
int iodef_reference_write(iodef_reference_t *reference, prelude_io_t *fd);
int iodef_method_write(iodef_method_t *method, prelude_io_t *fd);
int iodef_time_impact_write(iodef_time_impact_t *time_impact, prelude_io_t *fd);
int iodef_impact_write(iodef_impact_t *impact, prelude_io_t *fd);
int iodef_confidence_write(iodef_confidence_t *confidence, prelude_io_t *fd);
int iodef_monetary_impact_write(iodef_monetary_impact_t *monetary_impact, prelude_io_t *fd);
int iodef_counter_write(iodef_counter_t *counter, prelude_io_t *fd);
int iodef_assessment_write(iodef_assessment_t *assessment, prelude_io_t *fd);
int iodef_service_write(iodef_service_t *service, prelude_io_t *fd);
int iodef_address_write(iodef_address_t *address, prelude_io_t *fd);
int iodef_node_role_write(iodef_node_role_t *node_role, prelude_io_t *fd);
int iodef_node_write(iodef_node_t *node, prelude_io_t *fd);
int iodef_operating_system_write(iodef_operating_system_t *operating_system, prelude_io_t *fd);
int iodef_system_write(iodef_system_t *system, prelude_io_t *fd);
int iodef_flow_write(iodef_flow_t *flow, prelude_io_t *fd);
int iodef_event_data_write(iodef_event_data_t *event_data, prelude_io_t *fd);
int iodef_incident_write(iodef_incident_t *incident, prelude_io_t *fd);
int iodef_document_write(iodef_document_t *document, prelude_io_t *fd);


#ifdef __cplusplus
 }
#endif

#endif /* _LIBPRELUDE_IODEF_DOCUMENT_WRITE_H */
