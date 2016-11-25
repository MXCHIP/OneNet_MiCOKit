# include "regina_profile.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

static void p12_t0_profile_destroy(void *data)
{
    if (data) {
        p12_t0_profile_t *record = (p12_t0_profile_t *)data;

        regina_string_destroy(record->ieme);
        regina_data_destroy(record);
    }
}

static void p12_t0_profile_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p12_t0_profile_t *record = (p12_t0_profile_t *)data;

        regina_string_serialize(writer, record->ieme);
    }
}

static size_t p12_t0_profile_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p12_t0_profile_t *record = (p12_t0_profile_t *)data;

        record_size += regina_string_get_size(record->ieme);

        return record_size;
    }

    return 0;
}

p12_t0_profile_t *p12_t0_profile_create()
{
    p12_t0_profile_t *record = 
            (p12_t0_profile_t *)regina_calloc(1, sizeof(p12_t0_profile_t));

    if (record) {
        record->serialize = p12_t0_profile_serialize;
        record->get_size = p12_t0_profile_get_size;
        record->destroy = p12_t0_profile_destroy;
    }

    return record;
}

p12_t0_profile_t *p12_t0_profile_deserialize(avro_reader_t reader)
{
    p12_t0_profile_t *record = 
            (p12_t0_profile_t *)regina_malloc(sizeof(p12_t0_profile_t));

    if (record) {

        record->serialize = p12_t0_profile_serialize;
        record->get_size = p12_t0_profile_get_size;
        record->destroy = p12_t0_profile_destroy;

        record->ieme = regina_string_deserialize(reader);
    }

    return record;
}

