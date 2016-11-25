# include "regina_configuration.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */


static void p12_t1_configuration_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p12_t1_configuration_t *record = (p12_t1_configuration_t *)data;

        regina_int_serialize(writer, &record->sample_interval);
    }
}

static size_t p12_t1_configuration_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p12_t1_configuration_t *record = (p12_t1_configuration_t *)data;

        record_size += regina_int_get_size(&record->sample_interval);

        return record_size;
    }

    return 0;
}

p12_t1_configuration_t *p12_t1_configuration_create()
{
    p12_t1_configuration_t *record = 
            (p12_t1_configuration_t *)regina_calloc(1, sizeof(p12_t1_configuration_t));

    if (record) {
        record->serialize = p12_t1_configuration_serialize;
        record->get_size = p12_t1_configuration_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

p12_t1_configuration_t *p12_t1_configuration_deserialize(avro_reader_t reader)
{
    p12_t1_configuration_t *record = 
            (p12_t1_configuration_t *)regina_malloc(sizeof(p12_t1_configuration_t));

    if (record) {

        record->serialize = p12_t1_configuration_serialize;
        record->get_size = p12_t1_configuration_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_int(reader, &record->sample_interval);
    }

    return record;
}

