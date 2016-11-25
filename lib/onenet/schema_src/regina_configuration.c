# include "regina_configuration.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */


static void p72416_t1_configuration_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p72416_t1_configuration_t *record = (p72416_t1_configuration_t *)data;

        regina_int_serialize(writer, &record->acquisition_frequency);
    }
}

static size_t p72416_t1_configuration_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p72416_t1_configuration_t *record = (p72416_t1_configuration_t *)data;

        record_size += regina_int_get_size(&record->acquisition_frequency);

        return record_size;
    }

    return 0;
}

p72416_t1_configuration_t *p72416_t1_configuration_create()
{
    p72416_t1_configuration_t *record = 
            (p72416_t1_configuration_t *)regina_calloc(1, sizeof(p72416_t1_configuration_t));

    if (record) {
        record->serialize = p72416_t1_configuration_serialize;
        record->get_size = p72416_t1_configuration_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

p72416_t1_configuration_t *p72416_t1_configuration_deserialize(avro_reader_t reader)
{
    p72416_t1_configuration_t *record = 
            (p72416_t1_configuration_t *)regina_malloc(sizeof(p72416_t1_configuration_t));

    if (record) {

        record->serialize = p72416_t1_configuration_serialize;
        record->get_size = p72416_t1_configuration_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_int(reader, &record->acquisition_frequency);
    }

    return record;
}

