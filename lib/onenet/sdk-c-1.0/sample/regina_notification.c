# include "regina_notification.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

# ifndef P12_T3_UNION_NULL_OR_STRING_C_
# define P12_T3_UNION_NULL_OR_STRING_C_
static void p12_t3_union_null_or_string_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case P12_T3_UNION_NULL_OR_STRING_BRANCH_1:
        {
            if (regina_union->data) {
                regina_string_destroy(regina_union->data);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t p12_t3_union_null_or_string_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case P12_T3_UNION_NULL_OR_STRING_BRANCH_1:
        {
            if (regina_union->data) {
                union_size += regina_string_get_size(regina_union->data);
            }
            break;
        }
        default:
            break;
        }

        return union_size;
    }

    return 0;
}

static void p12_t3_union_null_or_string_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case P12_T3_UNION_NULL_OR_STRING_BRANCH_1:
        {
            if (regina_union->data) {
                regina_string_serialize(writer, regina_union->data);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *p12_t3_union_null_or_string_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = p12_t3_union_null_or_string_serialize;
        regina_union->get_size = p12_t3_union_null_or_string_get_size;
        regina_union->destroy = p12_t3_union_null_or_string_destroy;
    }

    return regina_union;
}

regina_union_t *p12_t3_union_null_or_string_branch_0_create()
{
    regina_union_t *regina_union = p12_t3_union_null_or_string_create();
    if (regina_union) {
        regina_union->type = P12_T3_UNION_NULL_OR_STRING_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *p12_t3_union_null_or_string_branch_1_create()
{
    regina_union_t *regina_union = p12_t3_union_null_or_string_create();
    if (regina_union) {
        regina_union->type = P12_T3_UNION_NULL_OR_STRING_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *p12_t3_union_null_or_string_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = p12_t3_union_null_or_string_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case P12_T3_UNION_NULL_OR_STRING_BRANCH_1: {
            regina_union->data = regina_string_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* P12_T3_UNION_NULL_OR_STRING_C_ */

static void p12_t3_notification_destroy(void *data)
{
    if (data) {
        p12_t3_notification_t *record = (p12_t3_notification_t *)data;

        regina_string_destroy(record->news);
        if (record->confirmation && record->confirmation->destroy) {
            record->confirmation->destroy(record->confirmation);
        }
        regina_data_destroy(record);
    }
}

static void p12_t3_notification_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p12_t3_notification_t *record = (p12_t3_notification_t *)data;

        regina_string_serialize(writer, record->news);
        record->confirmation->serialize(writer, record->confirmation);
    }
}

static size_t p12_t3_notification_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p12_t3_notification_t *record = (p12_t3_notification_t *)data;

        record_size += regina_string_get_size(record->news);
        record_size += record->confirmation->get_size(record->confirmation);

        return record_size;
    }

    return 0;
}

p12_t3_notification_t *p12_t3_notification_create()
{
    p12_t3_notification_t *record = 
            (p12_t3_notification_t *)regina_calloc(1, sizeof(p12_t3_notification_t));

    if (record) {
        record->serialize = p12_t3_notification_serialize;
        record->get_size = p12_t3_notification_get_size;
        record->destroy = p12_t3_notification_destroy;
    }

    return record;
}

p12_t3_notification_t *p12_t3_notification_deserialize(avro_reader_t reader)
{
    p12_t3_notification_t *record = 
            (p12_t3_notification_t *)regina_malloc(sizeof(p12_t3_notification_t));

    if (record) {

        record->serialize = p12_t3_notification_serialize;
        record->get_size = p12_t3_notification_get_size;
        record->destroy = p12_t3_notification_destroy;

        record->news = regina_string_deserialize(reader);
        record->confirmation = p12_t3_union_null_or_string_deserialize(reader);
    }

    return record;
}

