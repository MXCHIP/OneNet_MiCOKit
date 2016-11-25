# include "regina_sensor_data.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */


static void p12_t4_sensor_data_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p12_t4_sensor_data_t *record = (p12_t4_sensor_data_t *)data;

        regina_float_serialize(writer, &record->speed);
        regina_float_serialize(writer, &record->temperature);
    }
}

static size_t p12_t4_sensor_data_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p12_t4_sensor_data_t *record = (p12_t4_sensor_data_t *)data;

        record_size += AVRO_FLOAT_SIZE;
        record_size += AVRO_FLOAT_SIZE;

        return record_size;
    }

    return 0;
}

p12_t4_sensor_data_t *p12_t4_sensor_data_create()
{
    p12_t4_sensor_data_t *record = 
            (p12_t4_sensor_data_t *)regina_calloc(1, sizeof(p12_t4_sensor_data_t));

    if (record) {
        record->serialize = p12_t4_sensor_data_serialize;
        record->get_size = p12_t4_sensor_data_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

p12_t4_sensor_data_t *p12_t4_sensor_data_deserialize(avro_reader_t reader)
{
    p12_t4_sensor_data_t *record = 
            (p12_t4_sensor_data_t *)regina_malloc(sizeof(p12_t4_sensor_data_t));

    if (record) {

        record->serialize = p12_t4_sensor_data_serialize;
        record->get_size = p12_t4_sensor_data_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_float(reader, &record->speed);
        avro_binary_encoding.read_float(reader, &record->temperature);
    }

    return record;
}

