# include "sdk_conf_gen.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

static void regina_bootstrap_access_point_destroy(void *data)
{
    if (data) {
        regina_bootstrap_access_point_t *record = (regina_bootstrap_access_point_t *)data;

        regina_string_destroy(record->host);
        regina_bytes_destroy(record->public_key);
        regina_data_destroy(record);
    }
}

static void regina_bootstrap_access_point_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_bootstrap_access_point_t *record = (regina_bootstrap_access_point_t *)data;

        regina_int_serialize(writer, &record->protocol);
        regina_string_serialize(writer, record->host);
        regina_int_serialize(writer, &record->port);
        regina_bytes_serialize(writer, record->public_key);
    }
}

static size_t regina_bootstrap_access_point_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_bootstrap_access_point_t *record = (regina_bootstrap_access_point_t *)data;

        record_size += regina_int_get_size(&record->protocol);
        record_size += regina_string_get_size(record->host);
        record_size += regina_int_get_size(&record->port);
        record_size += regina_bytes_get_size(record->public_key);

        return record_size;
    }

    return 0;
}

regina_bootstrap_access_point_t *regina_bootstrap_access_point_create()
{
    regina_bootstrap_access_point_t *record = 
            (regina_bootstrap_access_point_t *)regina_calloc(1, sizeof(regina_bootstrap_access_point_t));

    if (record) {
        record->serialize = regina_bootstrap_access_point_serialize;
        record->get_size = regina_bootstrap_access_point_get_size;
        record->destroy = regina_bootstrap_access_point_destroy;
    }

    return record;
}

regina_bootstrap_access_point_t *regina_bootstrap_access_point_deserialize(avro_reader_t reader)
{
    regina_bootstrap_access_point_t *record = 
            (regina_bootstrap_access_point_t *)regina_malloc(sizeof(regina_bootstrap_access_point_t));

    if (record) {

        record->serialize = regina_bootstrap_access_point_serialize;
        record->get_size = regina_bootstrap_access_point_get_size;
        record->destroy = regina_bootstrap_access_point_destroy;

        avro_binary_encoding.read_int(reader, &record->protocol);
        record->host = regina_string_deserialize(reader);
        avro_binary_encoding.read_int(reader, &record->port);
        record->public_key = regina_bytes_deserialize(reader);
    }

    return record;
}


static void regina_sensor_data_limit_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_sensor_data_limit_t *record = (regina_sensor_data_limit_t *)data;

        regina_int_serialize(writer, &record->max_size);
        regina_int_serialize(writer, &record->min_interval);
    }
}

static size_t regina_sensor_data_limit_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_sensor_data_limit_t *record = (regina_sensor_data_limit_t *)data;

        record_size += regina_int_get_size(&record->max_size);
        record_size += regina_int_get_size(&record->min_interval);

        return record_size;
    }

    return 0;
}

regina_sensor_data_limit_t *regina_sensor_data_limit_create()
{
    regina_sensor_data_limit_t *record = 
            (regina_sensor_data_limit_t *)regina_calloc(1, sizeof(regina_sensor_data_limit_t));

    if (record) {
        record->serialize = regina_sensor_data_limit_serialize;
        record->get_size = regina_sensor_data_limit_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

regina_sensor_data_limit_t *regina_sensor_data_limit_deserialize(avro_reader_t reader)
{
    regina_sensor_data_limit_t *record = 
            (regina_sensor_data_limit_t *)regina_malloc(sizeof(regina_sensor_data_limit_t));

    if (record) {

        record->serialize = regina_sensor_data_limit_serialize;
        record->get_size = regina_sensor_data_limit_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_int(reader, &record->max_size);
        avro_binary_encoding.read_int(reader, &record->min_interval);
    }

    return record;
}


static void regina_event_limit_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_event_limit_t *record = (regina_event_limit_t *)data;

        regina_int_serialize(writer, &record->max_size);
        regina_int_serialize(writer, &record->max_count_per_day);
    }
}

static size_t regina_event_limit_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_event_limit_t *record = (regina_event_limit_t *)data;

        record_size += regina_int_get_size(&record->max_size);
        record_size += regina_int_get_size(&record->max_count_per_day);

        return record_size;
    }

    return 0;
}

regina_event_limit_t *regina_event_limit_create()
{
    regina_event_limit_t *record = 
            (regina_event_limit_t *)regina_calloc(1, sizeof(regina_event_limit_t));

    if (record) {
        record->serialize = regina_event_limit_serialize;
        record->get_size = regina_event_limit_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

regina_event_limit_t *regina_event_limit_deserialize(avro_reader_t reader)
{
    regina_event_limit_t *record = 
            (regina_event_limit_t *)regina_malloc(sizeof(regina_event_limit_t));

    if (record) {

        record->serialize = regina_event_limit_serialize;
        record->get_size = regina_event_limit_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_int(reader, &record->max_size);
        avro_binary_encoding.read_int(reader, &record->max_count_per_day);
    }

    return record;
}

static void regina_sdk_config_destroy(void *data)
{
    if (data) {
        regina_sdk_config_t *record = (regina_sdk_config_t *)data;

        regina_string_destroy(record->sdk_uuid);
                    regina_list_destroy(record->bootstrap_access_points, regina_bootstrap_access_point_destroy);
                    if (record->sensor_data_limit && record->sensor_data_limit->destroy) {
            record->sensor_data_limit->destroy(record->sensor_data_limit);
        }
        if (record->event_limit && record->event_limit->destroy) {
            record->event_limit->destroy(record->event_limit);
        }
        regina_data_destroy(record);
    }
}

static void regina_sdk_config_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_sdk_config_t *record = (regina_sdk_config_t *)data;

        regina_string_serialize(writer, record->sdk_uuid);
                    regina_array_serialize(writer, record->bootstrap_access_points, regina_bootstrap_access_point_serialize);
                    record->sensor_data_limit->serialize(writer, record->sensor_data_limit);
        record->event_limit->serialize(writer, record->event_limit);
    }
}

static size_t regina_sdk_config_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_sdk_config_t *record = (regina_sdk_config_t *)data;

        record_size += regina_string_get_size(record->sdk_uuid);
                    record_size += regina_array_get_size(record->bootstrap_access_points, regina_bootstrap_access_point_get_size);
                    record_size += record->sensor_data_limit->get_size(record->sensor_data_limit);
        record_size += record->event_limit->get_size(record->event_limit);

        return record_size;
    }

    return 0;
}

regina_sdk_config_t *regina_sdk_config_create()
{
    regina_sdk_config_t *record = 
            (regina_sdk_config_t *)regina_calloc(1, sizeof(regina_sdk_config_t));

    if (record) {
        record->serialize = regina_sdk_config_serialize;
        record->get_size = regina_sdk_config_get_size;
        record->destroy = regina_sdk_config_destroy;
    }

    return record;
}

regina_sdk_config_t *regina_sdk_config_deserialize(avro_reader_t reader)
{
    regina_sdk_config_t *record = 
            (regina_sdk_config_t *)regina_malloc(sizeof(regina_sdk_config_t));

    if (record) {

        record->serialize = regina_sdk_config_serialize;
        record->get_size = regina_sdk_config_get_size;
        record->destroy = regina_sdk_config_destroy;

        record->sdk_uuid = regina_string_deserialize(reader);
            record->bootstrap_access_points = regina_array_deserialize_wo_ctx(reader, (deserialize_wo_ctx_fn)regina_bootstrap_access_point_deserialize);
            record->sensor_data_limit = regina_sensor_data_limit_deserialize(reader);
        record->event_limit = regina_event_limit_deserialize(reader);
    }

    return record;
}

