# include "regina_event.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */


static void p72416_t2_event_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p72416_t2_event_t *record = (p72416_t2_event_t *)data;

        regina_boolean_serialize(writer, &record->is_sensor);
    }
}

static size_t p72416_t2_event_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p72416_t2_event_t *record = (p72416_t2_event_t *)data;

        record_size += regina_boolean_get_size(&record->is_sensor);

        return record_size;
    }

    return 0;
}

p72416_t2_event_t *p72416_t2_event_create()
{
    p72416_t2_event_t *record = 
            (p72416_t2_event_t *)regina_calloc(1, sizeof(p72416_t2_event_t));

    if (record) {
        record->serialize = p72416_t2_event_serialize;
        record->get_size = p72416_t2_event_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

p72416_t2_event_t *p72416_t2_event_deserialize(avro_reader_t reader)
{
    p72416_t2_event_t *record = 
            (p72416_t2_event_t *)regina_malloc(sizeof(p72416_t2_event_t));

    if (record) {

        record->serialize = p72416_t2_event_serialize;
        record->get_size = p72416_t2_event_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_boolean(reader, &record->is_sensor);
    }

    return record;
}

