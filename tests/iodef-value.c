#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libiodef.h"


static void cast_data(iodef_value_t *value)
{
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT64, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT64, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_FLOAT, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DOUBLE, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_TIME, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_ENUM, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_LIST, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_CLASS, -1) < 0);

        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_STRING, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_STRING);
}

static void cast_int8(void)
{
        iodef_value_t *value;

        assert(iodef_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT64, -1) < 0);

        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT16, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_INT16);
        assert(iodef_value_get_int16(value) == INT8_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT32, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_INT32);
        assert(iodef_value_get_int32(value) == INT8_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT64, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_INT64);
        assert(iodef_value_get_int64(value) == INT8_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_FLOAT, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_FLOAT);
        assert(iodef_value_get_float(value) == INT8_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DOUBLE, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DOUBLE);
        assert(iodef_value_get_double(value) == INT8_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int8(&value, INT8_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DATA, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DATA);
        assert((int) iodef_data_get_uint32(iodef_value_get_data(value)) == INT8_MIN);
        iodef_value_destroy(value);
}

static void cast_int16(void)
{
        iodef_value_t *value;

        assert(iodef_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT64, -1) < 0);

        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT32, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_INT32);
        assert(iodef_value_get_int32(value) == INT16_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT64, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_INT64);
        assert(iodef_value_get_int64(value) == INT16_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_FLOAT, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_FLOAT);
        assert(iodef_value_get_float(value) == INT16_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DOUBLE, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DOUBLE);
        assert(iodef_value_get_double(value) == INT16_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int16(&value, INT16_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DATA, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DATA);
        assert((int) iodef_data_get_uint32(iodef_value_get_data(value)) == INT16_MIN);
        iodef_value_destroy(value);
}


static void cast_int32(void)
{
        iodef_value_t *value;

        assert(iodef_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT64, -1) < 0);

        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT64, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_INT64);
        assert(iodef_value_get_int64(value) == INT32_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_FLOAT, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_FLOAT);
        assert(iodef_value_get_float(value) == INT32_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DOUBLE, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DOUBLE);
        assert(iodef_value_get_double(value) == INT32_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int32(&value, INT32_MIN) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DATA, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DATA);
        assert((int) iodef_data_get_uint32(iodef_value_get_data(value)) == INT32_MIN);
        iodef_value_destroy(value);

        assert(iodef_value_new_int32(&value, INT32_MAX) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_TIME, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_TIME);
        assert(iodef_time_get_sec(iodef_value_get_time(value)) == INT32_MAX);
        iodef_value_destroy(value);
}


static void cast_string(void)
{
        iodef_data_t *data;
        iodef_value_t *value;
        libiodef_string_t *str;

        assert(libiodef_string_new_ref(&str, "abcdefgh") == 0);
        assert(iodef_value_new_string(&value, str) == 0);
        iodef_value_dont_have_own_data(value);

        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT8, -1) < 0);
//        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT8, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT16, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT32, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_INT64, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_UINT64, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_FLOAT, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DOUBLE, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_TIME, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_ENUM, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_LIST, -1) < 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_CLASS, -1) < 0);

        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_DATA, -1) == 0);
        assert(iodef_value_get_type(value) == IODEF_VALUE_TYPE_DATA);
        assert(data = iodef_value_get_data(value));
        assert(iodef_data_get_len(data) == (libiodef_string_get_len(str) + 1));
        assert(memcmp(libiodef_string_get_string(str), iodef_data_get_data(data), iodef_data_get_len(data)) == 0);
        libiodef_string_destroy(str);

        cast_data(value);
        iodef_value_destroy(value);

        assert(libiodef_string_new_ref(&str, "2008-01-01 20:42:31") == 0);
        assert(iodef_value_new_string(&value, str) == 0);
        assert(_iodef_value_cast(value, IODEF_VALUE_TYPE_TIME, -1) == 0);
        iodef_value_destroy(value);
}


int main(void)
{
        cast_int8();
        cast_int16();
        cast_int32();
        cast_string();
        exit(0);
}
