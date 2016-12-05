# include "regina_sensor_data.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */


static void p72416_t4_sensordata_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p72416_t4_sensordata_t *record = (p72416_t4_sensordata_t *)data;

        regina_int_serialize(writer, &record->temperature);
        regina_int_serialize(writer, &record->humidity);
        regina_int_serialize(writer, &record->light_sensor);
        regina_int_serialize(writer, &record->infrared);
        regina_boolean_serialize(writer, &record->dc_motor);
        regina_float_serialize(writer, &record->hues);
        regina_float_serialize(writer, &record->saturation);
        regina_float_serialize(writer, &record->brightness);
        regina_boolean_serialize(writer, &record->lamp_switch);
    }
}

static size_t p72416_t4_sensordata_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p72416_t4_sensordata_t *record = (p72416_t4_sensordata_t *)data;

        record_size += regina_int_get_size(&record->temperature);
        record_size += regina_int_get_size(&record->humidity);
        record_size += regina_int_get_size(&record->light_sensor);
        record_size += regina_int_get_size(&record->infrared);
        record_size += regina_boolean_get_size(&record->dc_motor);
        record_size += AVRO_FLOAT_SIZE;
        record_size += AVRO_FLOAT_SIZE;
        record_size += AVRO_FLOAT_SIZE;
        record_size += regina_boolean_get_size(&record->lamp_switch);

        return record_size;
    }

    return 0;
}

p72416_t4_sensordata_t *p72416_t4_sensordata_create()
{
    p72416_t4_sensordata_t *record = 
            (p72416_t4_sensordata_t *)regina_calloc(1, sizeof(p72416_t4_sensordata_t));

    if (record) {
        record->serialize = p72416_t4_sensordata_serialize;
        record->get_size = p72416_t4_sensordata_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

p72416_t4_sensordata_t *p72416_t4_sensordata_deserialize(avro_reader_t reader)
{
    p72416_t4_sensordata_t *record = 
            (p72416_t4_sensordata_t *)regina_malloc(sizeof(p72416_t4_sensordata_t));

    if (record) {

        record->serialize = p72416_t4_sensordata_serialize;
        record->get_size = p72416_t4_sensordata_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_int(reader, &record->temperature);
        avro_binary_encoding.read_int(reader, &record->humidity);
        avro_binary_encoding.read_int(reader, &record->light_sensor);
        avro_binary_encoding.read_int(reader, &record->infrared);
        avro_binary_encoding.read_boolean(reader, &record->dc_motor);
        avro_binary_encoding.read_float(reader, &record->hues);
        avro_binary_encoding.read_float(reader, &record->saturation);
        avro_binary_encoding.read_float(reader, &record->brightness);
        avro_binary_encoding.read_boolean(reader, &record->lamp_switch);
    }

    return record;
}

