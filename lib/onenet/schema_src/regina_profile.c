# include "regina_profile.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

static void p72416_t0_profile_destroy(void *data)
{
    if (data) {
        p72416_t0_profile_t *record = (p72416_t0_profile_t *)data;

        regina_string_destroy(record->mac);
        regina_data_destroy(record);
    }
}

static void p72416_t0_profile_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p72416_t0_profile_t *record = (p72416_t0_profile_t *)data;

        regina_string_serialize(writer, record->mac);
    }
}

static size_t p72416_t0_profile_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p72416_t0_profile_t *record = (p72416_t0_profile_t *)data;

        record_size += regina_string_get_size(record->mac);

        return record_size;
    }

    return 0;
}

p72416_t0_profile_t *p72416_t0_profile_create()
{
    p72416_t0_profile_t *record = 
            (p72416_t0_profile_t *)regina_calloc(1, sizeof(p72416_t0_profile_t));

    if (record) {
        record->serialize = p72416_t0_profile_serialize;
        record->get_size = p72416_t0_profile_get_size;
        record->destroy = p72416_t0_profile_destroy;
    }

    return record;
}

p72416_t0_profile_t *p72416_t0_profile_deserialize(avro_reader_t reader)
{
    p72416_t0_profile_t *record = 
            (p72416_t0_profile_t *)regina_malloc(sizeof(p72416_t0_profile_t));

    if (record) {

        record->serialize = p72416_t0_profile_serialize;
        record->get_size = p72416_t0_profile_get_size;
        record->destroy = p72416_t0_profile_destroy;

        record->mac = regina_string_deserialize(reader);
    }

    return record;
}

