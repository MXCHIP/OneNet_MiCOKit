# include "regina_event.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */


static void p12_t2_event_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p12_t2_event_t *record = (p12_t2_event_t *)data;

        regina_float_serialize(writer, &record->pression);
    }
}

static size_t p12_t2_event_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p12_t2_event_t *record = (p12_t2_event_t *)data;

        record_size += AVRO_FLOAT_SIZE;

        return record_size;
    }

    return 0;
}

p12_t2_event_t *p12_t2_event_create()
{
    p12_t2_event_t *record = 
            (p12_t2_event_t *)regina_calloc(1, sizeof(p12_t2_event_t));

    if (record) {
        record->serialize = p12_t2_event_serialize;
        record->get_size = p12_t2_event_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

p12_t2_event_t *p12_t2_event_deserialize(avro_reader_t reader)
{
    p12_t2_event_t *record = 
            (p12_t2_event_t *)regina_malloc(sizeof(p12_t2_event_t));

    if (record) {

        record->serialize = p12_t2_event_serialize;
        record->get_size = p12_t2_event_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_float(reader, &record->pression);
    }

    return record;
}

