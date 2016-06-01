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


/*
 * NOTE ON GENERATION TIME MACROS:
 *
 *  - IODEF_LINKED_OBJECT: use within object that will be a member of the list
 *    always put this field as the first field of the struct.
 *
 *  - LISTED_OBJECT(name, type): include list named 'name' consisted of objects
 *    named 'type' (objects must have IODEF_LINKED_OBJECT member). 'name' should end with '_list'.
 *
 *  - UNION_MEMBER(union, var, val, type, name): use within 'union' clause.
 *    Parameters: 'union' - name of the union macro is used within
 *                'var' - variable controlling selection of memeber form the union
 *                'val' - value of variable 'var' for which this member will be used
 *                'type' - type of member variable
 *                'name' - name of member variable
 *
 * - FORCE_REGISTER(type, class): force parser to treat type 'type' as a 'class',
 *   even if it was not (yet) defined; example: FORCE_REGISTER(my_struct_t, struct)
 *   registers my_struct_t as struct.
 *
 * - TYPE_ID(type, id): set ID number of type 'type' to 'id'
 */


#ifndef _LIBIODEF_IODEF_TREE_H
#define _LIBIODEF_IODEF_TREE_H

#ifndef _GENERATE

#include "libiodef-inttypes.h"

#include "libiodef-hash.h"

#include "libiodef-io.h"
#include "libiodef-message.h"

#define LISTED_OBJECT(name, type) libiodef_list_t name

#define IS_KEY_LISTED(keyfield) IODEF_LINKED_OBJECT; libiodef_string_t *keyfield

#define UNION(type, var) type var; union

#define UNION_MEMBER(value, type, name) type name

#define ENUM(...) typedef enum

#define PRE_DECLARE(type, class)

#define TYPE_ID(type, id) type

#define PRIMITIVE_TYPE(type)
#define PRIMITIVE_TYPE_STRUCT(type)

#define HIDE(type, name) type name

#define REFCOUNT int refcount

#define OPTIONAL_INT(type, name) type name; unsigned int name_ ## is_set:1

#define IDENT(name) uint64_t name


#endif /* _GENERATE */

/*
 * Default value for an enumeration member should always be 0.
 * If there is no default value, no enumeration member should use
 * the 0 value
 */

#define IODEF_VERSION "0.6"

#ifndef _GENERATE
#include "libiodef-string.h"
#include "iodef-time.h"
#include "iodef-data.h"
#endif

PRIMITIVE_TYPE(void)
PRIMITIVE_TYPE(int16_t)
PRIMITIVE_TYPE(int32_t)
PRIMITIVE_TYPE(int64_t)
PRIMITIVE_TYPE(uint16_t)
PRIMITIVE_TYPE(uint32_t)
PRIMITIVE_TYPE(uint64_t)
PRIMITIVE_TYPE(uchar_t)
PRIMITIVE_TYPE(float)

PRIMITIVE_TYPE_STRUCT(libiodef_string_t)
PRIMITIVE_TYPE_STRUCT(iodef_time_t)
PRIMITIVE_TYPE_STRUCT(iodef_data_t)



/*
 * Additionaldata class
*/

ENUM() {
        IODEF_ADDITIONAL_DATA_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_additional_data_restriction_t, 151);


ENUM() {
        IODEF_ADDITIONAL_DATA_TYPE_ERROR = -1,
        IODEF_ADDITIONAL_DATA_TYPE_STRING = 1,
        IODEF_ADDITIONAL_DATA_TYPE_BYTE = 2,
        IODEF_ADDITIONAL_DATA_TYPE_CHARACTER = 3,
        IODEF_ADDITIONAL_DATA_TYPE_DATE_TIME = 4,
        IODEF_ADDITIONAL_DATA_TYPE_INTEGER = 5,
        IODEF_ADDITIONAL_DATA_TYPE_NTPSTAMP = 6,
        IODEF_ADDITIONAL_DATA_TYPE_PORTLIST = 7,
        IODEF_ADDITIONAL_DATA_TYPE_REAL = 8,
        IODEF_ADDITIONAL_DATA_TYPE_BOOLEAN = 9,
        IODEF_ADDITIONAL_DATA_TYPE_BYTE_STRING = 10,
        IODEF_ADDITIONAL_DATA_TYPE_XML = 11,
} TYPE_ID(iodef_additional_data_type_t, 152);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;
        IGNORED(libiodef_bool_t, _type_is_set);

        iodef_additional_data_restriction_t restriction;
        iodef_additional_data_type_t type;
        libiodef_string_t *formatid;
        libiodef_string_t *meaning;
        libiodef_string_t *ext_dtype;
        REQUIRED(iodef_data_t, *data);
} TYPE_ID(iodef_additional_data_t, 1);

/*
 * Email class
*/

ENUM() {
        IODEF_EMAIL_MEANING_ERROR = -1,
} TYPE_ID(iodef_email_meaning_t, 153);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_email_meaning_t meaning;
        REQUIRED(libiodef_string_t, *email);
} TYPE_ID(iodef_email_t, 2);

/*
 * Registryhandle class
*/

ENUM() {
        IODEF_REGISTRY_HANDLE_REGISTRY_ERROR = -1,
        IODEF_REGISTRY_HANDLE_REGISTRY_INTERNIC = 1,
        IODEF_REGISTRY_HANDLE_REGISTRY_APNIC = 2,
        IODEF_REGISTRY_HANDLE_REGISTRY_ARIN = 3,
        IODEF_REGISTRY_HANDLE_REGISTRY_LACNIC = 4,
        IODEF_REGISTRY_HANDLE_REGISTRY_RIPE = 5,
        IODEF_REGISTRY_HANDLE_REGISTRY_AFRINIC = 6,
        IODEF_REGISTRY_HANDLE_REGISTRY_LOCAL = 7,
        IODEF_REGISTRY_HANDLE_REGISTRY_EXT_VALUE = 8,
} TYPE_ID(iodef_registry_handle_registry_t, 154);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_registry_handle_registry_t registry;
        libiodef_string_t *ext_registry;
} TYPE_ID(iodef_registry_handle_t, 3);

/*
 * Postaladdress class
*/

ENUM() {
        IODEF_POSTAL_ADDRESS_LANG_ERROR = -1,
} TYPE_ID(iodef_postal_address_lang_t, 155);


ENUM() {
        IODEF_POSTAL_ADDRESS_MEANING_ERROR = -1,
} TYPE_ID(iodef_postal_address_meaning_t, 156);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        iodef_postal_address_lang_t lang;
        iodef_postal_address_meaning_t meaning;
} TYPE_ID(iodef_postal_address_t, 4);

/*
 * Contact class
*/

ENUM() {
        IODEF_CONTACT_TYPE_ERROR = -1,
        IODEF_CONTACT_TYPE_PERSON = 1,
        IODEF_CONTACT_TYPE_ORGANIZATION = 2,
        IODEF_CONTACT_TYPE_EXT_VALUE = 3,
} TYPE_ID(iodef_contact_type_t, 157);


ENUM() {
        IODEF_CONTACT_ROLE_ERROR = -1,
        IODEF_CONTACT_ROLE_CREATOR = 1,
        IODEF_CONTACT_ROLE_ADMIN = 2,
        IODEF_CONTACT_ROLE_TECH = 3,
        IODEF_CONTACT_ROLE_IRT = 4,
        IODEF_CONTACT_ROLE_CC = 5,
        IODEF_CONTACT_ROLE_EXT_VALUE = 6,
} TYPE_ID(iodef_contact_role_t, 158);


ENUM() {
        IODEF_CONTACT_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_contact_restriction_t, 159);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        libiodef_string_t *fax;
        LISTED_OBJECT(description_list, libiodef_string_t);
        LISTED_OBJECT(telephone_list, libiodef_string_t);
        LISTED_OBJECT(registry_handle_list, iodef_registry_handle_t);
        LISTED_OBJECT(contact_list, iodef_contact_t);
        libiodef_string_t *contact_name;
        libiodef_string_t *timezone;
        iodef_postal_address_t *postal_address;
        LISTED_OBJECT(email_list, iodef_email_t);
        iodef_contact_type_t type;
        iodef_contact_role_t role;
        libiodef_string_t *ext_type;
        libiodef_string_t *ext_role;
        iodef_contact_restriction_t restriction;
} TYPE_ID(iodef_contact_t, 5);

/*
 * Incidentid class
*/

ENUM() {
        IODEF_INCIDENT_ID_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_incident_id_restriction_t, 160);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        libiodef_string_t *instance;
        REQUIRED(libiodef_string_t, *name);
        iodef_incident_id_restriction_t restriction;
} TYPE_ID(iodef_incident_id_t, 6);

/*
 * Alternativeid class
*/

ENUM() {
        IODEF_ALTERNATIVE_ID_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_alternative_id_restriction_t, 161);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(incident_id_list, iodef_incident_id_t);
        iodef_alternative_id_restriction_t restriction;
} TYPE_ID(iodef_alternative_id_t, 7);

/*
 * Relatedactivity class
*/

ENUM() {
        IODEF_RELATED_ACTIVITY_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_related_activity_restriction_t, 162);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(url_list, libiodef_string_t);
        LISTED_OBJECT(incident_id_list, iodef_incident_id_t);
        iodef_related_activity_restriction_t restriction;
} TYPE_ID(iodef_related_activity_t, 8);

/*
 * Historyitem class
*/

ENUM() {
        IODEF_HISTORY_ITEM_ACTION_ERROR = -1,
} TYPE_ID(iodef_history_item_action_t, 163);


ENUM() {
        IODEF_HISTORY_ITEM_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_history_item_restriction_t, 164);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        iodef_incident_id_t *incident_id;
        LISTED_OBJECT(description_list, libiodef_string_t);
        iodef_contact_t *contact;
        REQUIRED(iodef_time_t, *date_time);
        iodef_history_item_action_t action;
        iodef_history_item_restriction_t restriction;
        libiodef_string_t *ext_action;
} TYPE_ID(iodef_history_item_t, 9);

/*
 * History class
*/

ENUM() {
        IODEF_HISTORY_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_history_restriction_t, 165);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(history_item_list, iodef_history_item_t);
        iodef_history_restriction_t restriction;
} TYPE_ID(iodef_history_t, 10);

/*
 * Expectation class
*/

ENUM() {
        IODEF_EXPECTATION_ACTION_ERROR = -1,
        IODEF_EXPECTATION_ACTION_NOTHING = 1,
        IODEF_EXPECTATION_ACTION_CONTACT_SOURCE_SITE = 2,
        IODEF_EXPECTATION_ACTION_CONTACT_TARGET_SITE = 3,
        IODEF_EXPECTATION_ACTION_CONTACT_SENDER = 4,
        IODEF_EXPECTATION_ACTION_INVESTIGATE = 5,
        IODEF_EXPECTATION_ACTION_BLOCK_HOST = 6,
        IODEF_EXPECTATION_ACTION_BLOCK_NETWORK = 7,
        IODEF_EXPECTATION_ACTION_BLOCK_PORT = 8,
        IODEF_EXPECTATION_ACTION_RATE_LIMIT_HOST = 9,
        IODEF_EXPECTATION_ACTION_RATE_LIMIT_NETWORK = 10,
        IODEF_EXPECTATION_ACTION_RATE_LIMIT_PORT = 11,
        IODEF_EXPECTATION_ACTION_REMEDIATE_OTHER = 12,
        IODEF_EXPECTATION_ACTION_STATUS_TRIAGE = 13,
        IODEF_EXPECTATION_ACTION_STATUS_NEW_INFO = 14,
        IODEF_EXPECTATION_ACTION_OTHER = 15,
        IODEF_EXPECTATION_ACTION_EXT_VALUE = 16,
} TYPE_ID(iodef_expectation_action_t, 166);


ENUM() {
        IODEF_EXPECTATION_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_expectation_restriction_t, 167);


ENUM() {
        IODEF_EXPECTATION_SEVERITY_ERROR = -1,
        IODEF_EXPECTATION_SEVERITY_LOW = 1,
        IODEF_EXPECTATION_SEVERITY_MEDIUM = 2,
        IODEF_EXPECTATION_SEVERITY_HIGH = 3,
} TYPE_ID(iodef_expectation_severity_t, 168);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_time_t *end_time;
        LISTED_OBJECT(description_list, libiodef_string_t);
        iodef_time_t *start_time;
        iodef_contact_t *contact;
        iodef_expectation_action_t action;
        iodef_expectation_restriction_t restriction;
        libiodef_string_t *ext_action;
        iodef_expectation_severity_t severity;
} TYPE_ID(iodef_expectation_t, 11);

/*
 * Recordpattern class
*/

ENUM() {
        IODEF_RECORD_PATTERN_OFFSETUNIT_ERROR = -1,
        IODEF_RECORD_PATTERN_OFFSETUNIT_LINE = 1,
        IODEF_RECORD_PATTERN_OFFSETUNIT_BINARY = 2,
        IODEF_RECORD_PATTERN_OFFSETUNIT_EXT_VALUE = 3,
} TYPE_ID(iodef_record_pattern_offsetunit_t, 169);


ENUM() {
        IODEF_RECORD_PATTERN_TYPE_ERROR = -1,
        IODEF_RECORD_PATTERN_TYPE_REGEX = 1,
        IODEF_RECORD_PATTERN_TYPE_BINARY = 2,
        IODEF_RECORD_PATTERN_TYPE_XPATH = 3,
        IODEF_RECORD_PATTERN_TYPE_EXT_VALUE = 4,
} TYPE_ID(iodef_record_pattern_type_t, 170);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_record_pattern_offsetunit_t offsetunit;
        OPTIONAL_INT(uint32_t, instance);
        libiodef_string_t *ext_type;
        OPTIONAL_INT(uint32_t, offset);
        libiodef_string_t *ext_offsetunit;
        iodef_record_pattern_type_t type;
} TYPE_ID(iodef_record_pattern_t, 12);

/*
 * Recorditem class
*/

ENUM() {
        IODEF_RECORD_ITEM_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_record_item_restriction_t, 171);


ENUM() {
        IODEF_RECORD_ITEM_DTYPE_ERROR = -1,
        IODEF_RECORD_ITEM_DTYPE_BOOLEAN = 1,
        IODEF_RECORD_ITEM_DTYPE_BYTE = 2,
        IODEF_RECORD_ITEM_DTYPE_CHARACTER = 3,
        IODEF_RECORD_ITEM_DTYPE_DATE_TIME = 4,
        IODEF_RECORD_ITEM_DTYPE_INTEGER = 5,
        IODEF_RECORD_ITEM_DTYPE_PORTLIST = 6,
        IODEF_RECORD_ITEM_DTYPE_REAL = 7,
        IODEF_RECORD_ITEM_DTYPE_STRING = 8,
        IODEF_RECORD_ITEM_DTYPE_FILE = 9,
        IODEF_RECORD_ITEM_DTYPE_FRAME = 10,
        IODEF_RECORD_ITEM_DTYPE_PACKET = 11,
        IODEF_RECORD_ITEM_DTYPE_IPV4_PACKET = 12,
        IODEF_RECORD_ITEM_DTYPE_IPV6_PACKET = 13,
        IODEF_RECORD_ITEM_DTYPE_PATH = 14,
        IODEF_RECORD_ITEM_DTYPE_URL = 15,
        IODEF_RECORD_ITEM_DTYPE_CSV = 16,
        IODEF_RECORD_ITEM_DTYPE_WINREG = 17,
        IODEF_RECORD_ITEM_DTYPE_XML = 18,
        IODEF_RECORD_ITEM_DTYPE_EXT_VALUE = 19,
} TYPE_ID(iodef_record_item_dtype_t, 172);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_record_item_restriction_t restriction;
        iodef_record_item_dtype_t dtype;
        libiodef_string_t *formatid;
        libiodef_string_t *meaning;
        libiodef_string_t *ext_dtype;
        iodef_data_t *data;
} TYPE_ID(iodef_record_item_t, 13);

/*
 * Application class
*/

struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        libiodef_string_t *url;
        libiodef_string_t *vendor;
        libiodef_string_t *name;
        libiodef_string_t *family;
        libiodef_string_t *swid;
        libiodef_string_t *patch;
        libiodef_string_t *version;
        libiodef_string_t *configid;
} TYPE_ID(iodef_application_t, 14);

/*
 * Recorddata class
*/

ENUM() {
        IODEF_RECORD_DATA_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_record_data_restriction_t, 173);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_additional_data_t *additional_data;
        LISTED_OBJECT(description_list, libiodef_string_t);
        LISTED_OBJECT(record_pattern_list, iodef_record_pattern_t);
        iodef_time_t *date_time;
        iodef_application_t *application;
        LISTED_OBJECT(record_item_list, iodef_record_item_t);
        iodef_record_data_restriction_t restriction;
} TYPE_ID(iodef_record_data_t, 15);

/*
 * Record class
*/

ENUM() {
        IODEF_RECORD_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_record_restriction_t, 174);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(record_data_list, iodef_record_data_t);
        iodef_record_restriction_t restriction;
} TYPE_ID(iodef_record_t, 16);

/*
 * Reference class
*/

struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(url_list, libiodef_string_t);
        LISTED_OBJECT(description_list, libiodef_string_t);
        REQUIRED(libiodef_string_t, *reference_name);
} TYPE_ID(iodef_reference_t, 17);

/*
 * Method class
*/

ENUM() {
        IODEF_METHOD_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_method_restriction_t, 175);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        LISTED_OBJECT(description_list, libiodef_string_t);
        LISTED_OBJECT(reference_list, iodef_reference_t);
        iodef_method_restriction_t restriction;
} TYPE_ID(iodef_method_t, 18);

/*
 * Timeimpact class
*/

ENUM() {
        IODEF_TIME_IMPACT_DURATION_ERROR = -1,
        IODEF_TIME_IMPACT_DURATION_SECOND = 1,
        IODEF_TIME_IMPACT_DURATION_MINUTE = 2,
        IODEF_TIME_IMPACT_DURATION_HOUR = 3,
        IODEF_TIME_IMPACT_DURATION_DAY = 4,
        IODEF_TIME_IMPACT_DURATION_MONTH = 5,
        IODEF_TIME_IMPACT_DURATION_QUARTER = 6,
        IODEF_TIME_IMPACT_DURATION_YEAR = 7,
        IODEF_TIME_IMPACT_DURATION_EXT_VALUE = 8,
} TYPE_ID(iodef_time_impact_duration_t, 176);


ENUM() {
        IODEF_TIME_IMPACT_METRIC_ERROR = -1,
        IODEF_TIME_IMPACT_METRIC_LABOR = 1,
        IODEF_TIME_IMPACT_METRIC_ELAPSED = 2,
        IODEF_TIME_IMPACT_METRIC_DOWNTIME = 3,
        IODEF_TIME_IMPACT_METRIC_EXT_VALUE = 4,
} TYPE_ID(iodef_time_impact_metric_t, 177);


ENUM() {
        IODEF_TIME_IMPACT_SEVERITY_ERROR = -1,
        IODEF_TIME_IMPACT_SEVERITY_LOW = 1,
        IODEF_TIME_IMPACT_SEVERITY_MEDIUM = 2,
        IODEF_TIME_IMPACT_SEVERITY_HIGH = 3,
} TYPE_ID(iodef_time_impact_severity_t, 178);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        libiodef_string_t *ext_metric;
        iodef_time_impact_duration_t duration;
        iodef_time_impact_metric_t metric;
        iodef_time_impact_severity_t severity;
        libiodef_string_t *ext_duration;
} TYPE_ID(iodef_time_impact_t, 19);

/*
 * Impact class
*/

ENUM() {
        IODEF_IMPACT_LANG_ERROR = -1,
} TYPE_ID(iodef_impact_lang_t, 179);


ENUM() {
        IODEF_IMPACT_COMPLETION_ERROR = -1,
        IODEF_IMPACT_COMPLETION_FAILED = 1,
        IODEF_IMPACT_COMPLETION_SUCCEEDED = 2,
} TYPE_ID(iodef_impact_completion_t, 180);


ENUM() {
        IODEF_IMPACT_TYPE_ERROR = -1,
        IODEF_IMPACT_TYPE_ADMIN = 1,
        IODEF_IMPACT_TYPE_DOS = 2,
        IODEF_IMPACT_TYPE_FILE = 3,
        IODEF_IMPACT_TYPE_INFO_LEAK = 4,
        IODEF_IMPACT_TYPE_MISCONFIGURATION = 5,
        IODEF_IMPACT_TYPE_POLICY = 6,
        IODEF_IMPACT_TYPE_RECON = 7,
        IODEF_IMPACT_TYPE_SOCIAL_ENGINEERING = 8,
        IODEF_IMPACT_TYPE_USER = 9,
        IODEF_IMPACT_TYPE_UNKNOWN = 10,
        IODEF_IMPACT_TYPE_EXT_VALUE = 11,
} TYPE_ID(iodef_impact_type_t, 181);


ENUM() {
        IODEF_IMPACT_SEVERITY_ERROR = -1,
        IODEF_IMPACT_SEVERITY_LOW = 1,
        IODEF_IMPACT_SEVERITY_MEDIUM = 2,
        IODEF_IMPACT_SEVERITY_HIGH = 3,
} TYPE_ID(iodef_impact_severity_t, 182);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_impact_lang_t lang;
        iodef_impact_completion_t completion;
        iodef_impact_type_t type;
        iodef_impact_severity_t severity;
        libiodef_string_t *ext_type;
} TYPE_ID(iodef_impact_t, 20);

/*
 * Confidence class
*/

ENUM() {
        IODEF_CONFIDENCE_RATING_ERROR = -1,
        IODEF_CONFIDENCE_RATING_LOW = 1,
        IODEF_CONFIDENCE_RATING_MEDIUM = 2,
        IODEF_CONFIDENCE_RATING_HIGH = 3,
        IODEF_CONFIDENCE_RATING_NUMERIC = 4,
} TYPE_ID(iodef_confidence_rating_t, 183);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        iodef_confidence_rating_t rating;
} TYPE_ID(iodef_confidence_t, 21);

/*
 * Monetaryimpact class
*/

ENUM() {
        IODEF_MONETARY_IMPACT_SEVERITY_ERROR = -1,
        IODEF_MONETARY_IMPACT_SEVERITY_LOW = 1,
        IODEF_MONETARY_IMPACT_SEVERITY_MEDIUM = 2,
        IODEF_MONETARY_IMPACT_SEVERITY_HIGH = 3,
} TYPE_ID(iodef_monetary_impact_severity_t, 184);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        REQUIRED(libiodef_string_t, *currency);
        iodef_monetary_impact_severity_t severity;
} TYPE_ID(iodef_monetary_impact_t, 22);

/*
 * Counter class
*/

ENUM() {
        IODEF_COUNTER_DURATION_ERROR = -1,
} TYPE_ID(iodef_counter_duration_t, 185);


ENUM() {
        IODEF_COUNTER_TYPE_ERROR = -1,
        IODEF_COUNTER_TYPE_BYTE = 1,
        IODEF_COUNTER_TYPE_PACKET = 2,
        IODEF_COUNTER_TYPE_FLOW = 3,
        IODEF_COUNTER_TYPE_SESSION = 4,
        IODEF_COUNTER_TYPE_ALERT = 5,
        IODEF_COUNTER_TYPE_MESSAGE = 6,
        IODEF_COUNTER_TYPE_EVENT = 7,
        IODEF_COUNTER_TYPE_HOST = 8,
        IODEF_COUNTER_TYPE_SITE = 9,
        IODEF_COUNTER_TYPE_ORGANIZATION = 10,
        IODEF_COUNTER_TYPE_EXT_VALUE = 11,
} TYPE_ID(iodef_counter_type_t, 186);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_counter_duration_t duration;
        iodef_counter_type_t type;
        libiodef_string_t *ext_duration;
        libiodef_string_t *ext_type;
} TYPE_ID(iodef_counter_t, 23);

/*
 * Assessment class
*/

ENUM() {
        IODEF_ASSESSMENT_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_assessment_restriction_t, 187);


ENUM() {
        IODEF_ASSESSMENT_OCCURRENCE_ERROR = -1,
        IODEF_ASSESSMENT_OCCURRENCE_ACTUAL = 1,
        IODEF_ASSESSMENT_OCCURRENCE_POTENTIAL = 2,
} TYPE_ID(iodef_assessment_occurrence_t, 188);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(impact_list, iodef_impact_t);
        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        iodef_confidence_t *confidence;
        LISTED_OBJECT(time_impact_list, iodef_time_impact_t);
        LISTED_OBJECT(monetary_impact_list, iodef_monetary_impact_t);
        LISTED_OBJECT(counter_list, iodef_counter_t);
        iodef_assessment_restriction_t restriction;
        iodef_assessment_occurrence_t occurrence;
} TYPE_ID(iodef_assessment_t, 24);

/*
 * Service class
*/

struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        OPTIONAL_INT(uint32_t, proto_code);
        OPTIONAL_INT(uint32_t, proto_flags);
        libiodef_string_t *portlist;
        LISTED_OBJECT(application_list, iodef_application_t);
        OPTIONAL_INT(uint32_t, proto_type);
        OPTIONAL_INT(uint32_t, port);
        OPTIONAL_INT(uint32_t, ip_protocol);
} TYPE_ID(iodef_service_t, 25);

/*
 * Address class
*/

ENUM() {
        IODEF_ADDRESS_CATEGORY_ERROR = -1,
        IODEF_ADDRESS_CATEGORY_ASN = 1,
        IODEF_ADDRESS_CATEGORY_ATM = 2,
        IODEF_ADDRESS_CATEGORY_E_MAIL = 3,
        IODEF_ADDRESS_CATEGORY_IPV4_ADDR = 4,
        IODEF_ADDRESS_CATEGORY_IPV4_NET = 5,
        IODEF_ADDRESS_CATEGORY_IPV4_NET_MASK = 6,
        IODEF_ADDRESS_CATEGORY_IPV6_ADDR = 7,
        IODEF_ADDRESS_CATEGORY_IPV6_NET = 8,
        IODEF_ADDRESS_CATEGORY_IPV6_NET_MASK = 9,
        IODEF_ADDRESS_CATEGORY_MAC = 10,
        IODEF_ADDRESS_CATEGORY_EXT_VALUE = 11,
} TYPE_ID(iodef_address_category_t, 189);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_address_category_t category;
        libiodef_string_t *vlan_name;
        libiodef_string_t *ext_category;
        libiodef_string_t *vlan_num;
} TYPE_ID(iodef_address_t, 27);

/*
 * Noderole class
*/

ENUM() {
        IODEF_NODE_ROLE_CATEGORY_ERROR = -1,
        IODEF_NODE_ROLE_CATEGORY_CLIENT = 1,
        IODEF_NODE_ROLE_CATEGORY_SERVER_INTERNAL = 2,
        IODEF_NODE_ROLE_CATEGORY_SERVER_PUBLIC = 3,
        IODEF_NODE_ROLE_CATEGORY_WWW = 4,
        IODEF_NODE_ROLE_CATEGORY_MAIL = 5,
        IODEF_NODE_ROLE_CATEGORY_MESSAGING = 6,
        IODEF_NODE_ROLE_CATEGORY_STREAMING = 7,
        IODEF_NODE_ROLE_CATEGORY_VOICE = 8,
        IODEF_NODE_ROLE_CATEGORY_FILE = 9,
        IODEF_NODE_ROLE_CATEGORY_FTP = 10,
        IODEF_NODE_ROLE_CATEGORY_P2P = 11,
        IODEF_NODE_ROLE_CATEGORY_NAME = 12,
        IODEF_NODE_ROLE_CATEGORY_DIRECTORY = 13,
        IODEF_NODE_ROLE_CATEGORY_CREDENTIAL = 14,
        IODEF_NODE_ROLE_CATEGORY_PRINT = 15,
        IODEF_NODE_ROLE_CATEGORY_APPLICATION = 16,
        IODEF_NODE_ROLE_CATEGORY_DATABASE = 17,
        IODEF_NODE_ROLE_CATEGORY_INFRA = 18,
        IODEF_NODE_ROLE_CATEGORY_LOG = 19,
        IODEF_NODE_ROLE_CATEGORY_EXT_VALUE = 20,
} TYPE_ID(iodef_node_role_category_t, 190);


ENUM() {
        IODEF_NODE_ROLE_LANG_ERROR = -1,
} TYPE_ID(iodef_node_role_lang_t, 191);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_node_role_category_t category;
        iodef_node_role_lang_t lang;
        libiodef_string_t *ext_category;
} TYPE_ID(iodef_node_role_t, 28);

/*
 * Node class
*/

struct {
        IODEF_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(node_name_list, libiodef_string_t);
        LISTED_OBJECT(counter_list, iodef_counter_t);
        iodef_time_t *date_time;
        LISTED_OBJECT(node_role_list, iodef_node_role_t);
        libiodef_string_t *location;
        LISTED_OBJECT(address_list, iodef_address_t);
} TYPE_ID(iodef_node_t, 29);

/*
 * Operatingsystem class
*/

struct {
        IODEF_OBJECT;
        REFCOUNT;

        libiodef_string_t *url;
        libiodef_string_t *vendor;
        libiodef_string_t *name;
        libiodef_string_t *family;
        libiodef_string_t *swid;
        libiodef_string_t *patch;
        libiodef_string_t *version;
        libiodef_string_t *configid;
} TYPE_ID(iodef_operating_system_t, 30);

/*
 * System class
*/

ENUM() {
        IODEF_SYSTEM_CATEGORY_ERROR = -1,
        IODEF_SYSTEM_CATEGORY_SOURCE = 1,
        IODEF_SYSTEM_CATEGORY_TARGET = 2,
        IODEF_SYSTEM_CATEGORY_INTERMEDIATE = 3,
        IODEF_SYSTEM_CATEGORY_SENSOR = 4,
        IODEF_SYSTEM_CATEGORY_INFRASTRUCTURE = 5,
        IODEF_SYSTEM_CATEGORY_EXT_VALUE = 6,
} TYPE_ID(iodef_system_category_t, 192);


ENUM() {
        IODEF_SYSTEM_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_system_restriction_t, 193);


ENUM() {
        IODEF_SYSTEM_SPOOFED_ERROR = -1,
        IODEF_SYSTEM_SPOOFED_UNKNOWN = 1,
        IODEF_SYSTEM_SPOOFED_YES = 2,
        IODEF_SYSTEM_SPOOFED_NO = 3,
} TYPE_ID(iodef_system_spoofed_t, 194);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        REQUIRED(iodef_node_t, *node);
        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        LISTED_OBJECT(description_list, libiodef_string_t);
        LISTED_OBJECT(service_list, iodef_service_t);
        LISTED_OBJECT(counter_list, iodef_counter_t);
        iodef_operating_system_t *operating_system;
        iodef_system_category_t category;
        iodef_system_restriction_t restriction;
        libiodef_string_t *ext_category;
        iodef_system_spoofed_t spoofed;
        libiodef_string_t *interface;
} TYPE_ID(iodef_system_t, 31);

/*
 * Flow class
*/

struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(system_list, iodef_system_t);
} TYPE_ID(iodef_flow_t, 32);

/*
 * Eventdata class
*/

ENUM() {
        IODEF_EVENT_DATA_RESTRICTION_ERROR = -1,
} TYPE_ID(iodef_event_data_restriction_t, 195);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_time_t *detect_time;
        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        LISTED_OBJECT(description_list, libiodef_string_t);
        LISTED_OBJECT(flow_list, iodef_flow_t);
        LISTED_OBJECT(expectation_list, iodef_expectation_t);
        iodef_record_t *record;
        LISTED_OBJECT(contact_list, iodef_contact_t);
        iodef_time_t *start_time;
        LISTED_OBJECT(event_data_list, iodef_event_data_t);
        iodef_time_t *end_time;
        iodef_assessment_t *assessment;
        LISTED_OBJECT(method_list, iodef_method_t);
        iodef_event_data_restriction_t restriction;
} TYPE_ID(iodef_event_data_t, 33);

/*
 * Incident class
*/

ENUM() {
        IODEF_INCIDENT_LANG_ERROR = -1,
} TYPE_ID(iodef_incident_lang_t, 196);


ENUM() {
        IODEF_INCIDENT_RESTRICTION_ERROR = -1,
        IODEF_INCIDENT_RESTRICTION_PUBLIC = 1,
        IODEF_INCIDENT_RESTRICTION_NEED_TO_KNOW = 2,
        IODEF_INCIDENT_RESTRICTION_PRIVATE = 3,
        IODEF_INCIDENT_RESTRICTION_DEFAULT = 4,
} TYPE_ID(iodef_incident_restriction_t, 197);


ENUM() {
        IODEF_INCIDENT_PURPOSE_ERROR = -1,
        IODEF_INCIDENT_PURPOSE_TRACEBACK = 1,
        IODEF_INCIDENT_PURPOSE_MITIGATION = 2,
        IODEF_INCIDENT_PURPOSE_REPORTING = 3,
        IODEF_INCIDENT_PURPOSE_OTHER = 4,
        IODEF_INCIDENT_PURPOSE_EXT_VALUE = 5,
} TYPE_ID(iodef_incident_purpose_t, 198);


struct {
        IODEF_LINKED_OBJECT;
        REFCOUNT;

        iodef_time_t *detect_time;
        LISTED_OBJECT(additional_data_list, iodef_additional_data_t);
        iodef_time_t *end_time;
        LISTED_OBJECT(description_list, libiodef_string_t);
        LISTED_OBJECT(contact_list, iodef_contact_t);
        iodef_alternative_id_t *alternative_id;
        REQUIRED(iodef_time_t, *report_time);
        iodef_time_t *start_time;
        LISTED_OBJECT(event_data_list, iodef_event_data_t);
        iodef_related_activity_t *related_activity;
        REQUIRED(iodef_incident_id_t, *incident_id);
        LISTED_OBJECT(assessment_list, iodef_assessment_t);
        LISTED_OBJECT(method_list, iodef_method_t);
        iodef_history_t *history;
        iodef_incident_lang_t lang;
        iodef_incident_restriction_t restriction;
        libiodef_string_t *ext_purpose;
        iodef_incident_purpose_t purpose;
} TYPE_ID(iodef_incident_t, 34);

/*
 * Document class
*/

ENUM() {
        IODEF_DOCUMENT_LANG_ERROR = -1,
} TYPE_ID(iodef_document_lang_t, 199);


struct {
        IODEF_OBJECT;
        REFCOUNT;

        LISTED_OBJECT(incident_list, iodef_incident_t);
        iodef_document_lang_t lang;
        libiodef_string_t *formatid;
        REQUIRED(libiodef_string_t, *version);
} TYPE_ID(iodef_document_t, 35);


#endif /* _LIBIODEF_IODEF_TREE_H */
