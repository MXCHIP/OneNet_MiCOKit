# include "device_status_gen.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

static void regina_device_status_destroy(void *data)
{
    if (data) {
        regina_device_status_t *record = (regina_device_status_t *)data;

        regina_string_destroy(record->reg_code);
        regina_string_destroy(record->auth_code);
        regina_bytes_destroy(record->profile_md5);
        regina_bytes_destroy(record->conf_md5);
        regina_data_destroy(record);
    }
}

static void regina_device_status_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_device_status_t *record = (regina_device_status_t *)data;

        regina_int_serialize(writer, &record->mode);
        regina_string_serialize(writer, record->reg_code);
        regina_string_serialize(writer, record->auth_code);
        regina_long_serialize(writer, &record->device_id);
        regina_long_serialize(writer, &record->notification_sn);
        regina_bytes_serialize(writer, record->profile_md5);
        regina_bytes_serialize(writer, record->conf_md5);
    }
}

static size_t regina_device_status_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_device_status_t *record = (regina_device_status_t *)data;

        record_size += regina_int_get_size(&record->mode);
        record_size += regina_string_get_size(record->reg_code);
        record_size += regina_string_get_size(record->auth_code);
        record_size += regina_long_get_size(&record->device_id);
        record_size += regina_long_get_size(&record->notification_sn);
        record_size += regina_bytes_get_size(record->profile_md5);
        record_size += regina_bytes_get_size(record->conf_md5);

        return record_size;
    }

    return 0;
}

regina_device_status_t *regina_device_status_create()
{
    regina_device_status_t *record = 
            (regina_device_status_t *)regina_calloc(1, sizeof(regina_device_status_t));

    if (record) {
        record->serialize = regina_device_status_serialize;
        record->get_size = regina_device_status_get_size;
        record->destroy = regina_device_status_destroy;
    }

    return record;
}

regina_device_status_t *regina_device_status_deserialize(avro_reader_t reader)
{
    regina_device_status_t *record = 
            (regina_device_status_t *)regina_malloc(sizeof(regina_device_status_t));

    if (record) {

        record->serialize = regina_device_status_serialize;
        record->get_size = regina_device_status_get_size;
        record->destroy = regina_device_status_destroy;

        avro_binary_encoding.read_int(reader, &record->mode);
        record->reg_code = regina_string_deserialize(reader);
        record->auth_code = regina_string_deserialize(reader);
        avro_binary_encoding.read_long(reader, &record->device_id);
        avro_binary_encoding.read_long(reader, &record->notification_sn);
        record->profile_md5 = regina_bytes_deserialize(reader);
        record->conf_md5 = regina_bytes_deserialize(reader);
    }

    return record;
}

