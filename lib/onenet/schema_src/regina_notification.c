# include "regina_notification.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

# ifndef P72416_T3_UNION_NULL_OR_FLOAT_C_
# define P72416_T3_UNION_NULL_OR_FLOAT_C_
static void p72416_t3_union_null_or_float_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_1:
        {
            if (regina_union->data) {
                regina_data_destroy(regina_union->data);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t p72416_t3_union_null_or_float_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_1:
        {
            if (regina_union->data) {
                union_size += AVRO_FLOAT_SIZE;
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

static void p72416_t3_union_null_or_float_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_1:
        {
            if (regina_union->data) {
                regina_float_serialize(writer, regina_union->data);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *p72416_t3_union_null_or_float_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = p72416_t3_union_null_or_float_serialize;
        regina_union->get_size = p72416_t3_union_null_or_float_get_size;
        regina_union->destroy = p72416_t3_union_null_or_float_destroy;
    }

    return regina_union;
}

regina_union_t *p72416_t3_union_null_or_float_branch_0_create()
{
    regina_union_t *regina_union = p72416_t3_union_null_or_float_create();
    if (regina_union) {
        regina_union->type = P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *p72416_t3_union_null_or_float_branch_1_create()
{
    regina_union_t *regina_union = p72416_t3_union_null_or_float_create();
    if (regina_union) {
        regina_union->type = P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *p72416_t3_union_null_or_float_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = p72416_t3_union_null_or_float_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_1: {
            regina_union->data = regina_float_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* P72416_T3_UNION_NULL_OR_FLOAT_C_ */

# ifndef P72416_T3_UNION_NULL_OR_BOOLEAN_C_
# define P72416_T3_UNION_NULL_OR_BOOLEAN_C_
static void p72416_t3_union_null_or_boolean_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1:
        {
            if (regina_union->data) {
                regina_data_destroy(regina_union->data);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t p72416_t3_union_null_or_boolean_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1:
        {
            if (regina_union->data) {
                union_size += regina_boolean_get_size(regina_union->data);
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

static void p72416_t3_union_null_or_boolean_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1:
        {
            if (regina_union->data) {
                regina_boolean_serialize(writer, regina_union->data);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *p72416_t3_union_null_or_boolean_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = p72416_t3_union_null_or_boolean_serialize;
        regina_union->get_size = p72416_t3_union_null_or_boolean_get_size;
        regina_union->destroy = p72416_t3_union_null_or_boolean_destroy;
    }

    return regina_union;
}

regina_union_t *p72416_t3_union_null_or_boolean_branch_0_create()
{
    regina_union_t *regina_union = p72416_t3_union_null_or_boolean_create();
    if (regina_union) {
        regina_union->type = P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *p72416_t3_union_null_or_boolean_branch_1_create()
{
    regina_union_t *regina_union = p72416_t3_union_null_or_boolean_create();
    if (regina_union) {
        regina_union->type = P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *p72416_t3_union_null_or_boolean_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = p72416_t3_union_null_or_boolean_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1: {
            regina_union->data = regina_boolean_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* P72416_T3_UNION_NULL_OR_BOOLEAN_C_ */

static void p72416_t3_notification_destroy(void *data)
{
    if (data) {
        p72416_t3_notification_t *record = (p72416_t3_notification_t *)data;

        if (record->hues && record->hues->destroy) {
            record->hues->destroy(record->hues);
        }
        if (record->saturation && record->saturation->destroy) {
            record->saturation->destroy(record->saturation);
        }
        if (record->brightness && record->brightness->destroy) {
            record->brightness->destroy(record->brightness);
        }
        if (record->dc_motor && record->dc_motor->destroy) {
            record->dc_motor->destroy(record->dc_motor);
        }
        if (record->lamp_switch && record->lamp_switch->destroy) {
            record->lamp_switch->destroy(record->lamp_switch);
        }
        regina_data_destroy(record);
    }
}

static void p72416_t3_notification_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        p72416_t3_notification_t *record = (p72416_t3_notification_t *)data;

        record->hues->serialize(writer, record->hues);
        record->saturation->serialize(writer, record->saturation);
        record->brightness->serialize(writer, record->brightness);
        record->dc_motor->serialize(writer, record->dc_motor);
        record->lamp_switch->serialize(writer, record->lamp_switch);
    }
}

static size_t p72416_t3_notification_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        p72416_t3_notification_t *record = (p72416_t3_notification_t *)data;

        record_size += record->hues->get_size(record->hues);
        record_size += record->saturation->get_size(record->saturation);
        record_size += record->brightness->get_size(record->brightness);
        record_size += record->dc_motor->get_size(record->dc_motor);
        record_size += record->lamp_switch->get_size(record->lamp_switch);

        return record_size;
    }

    return 0;
}

p72416_t3_notification_t *p72416_t3_notification_create()
{
    p72416_t3_notification_t *record = 
            (p72416_t3_notification_t *)regina_calloc(1, sizeof(p72416_t3_notification_t));

    if (record) {
        record->serialize = p72416_t3_notification_serialize;
        record->get_size = p72416_t3_notification_get_size;
        record->destroy = p72416_t3_notification_destroy;
    }

    return record;
}

p72416_t3_notification_t *p72416_t3_notification_deserialize(avro_reader_t reader)
{
    p72416_t3_notification_t *record = 
            (p72416_t3_notification_t *)regina_malloc(sizeof(p72416_t3_notification_t));

    if (record) {

        record->serialize = p72416_t3_notification_serialize;
        record->get_size = p72416_t3_notification_get_size;
        record->destroy = p72416_t3_notification_destroy;

        record->hues = p72416_t3_union_null_or_float_deserialize(reader);
        record->saturation = p72416_t3_union_null_or_float_deserialize(reader);
        record->brightness = p72416_t3_union_null_or_float_deserialize(reader);
        record->dc_motor = p72416_t3_union_null_or_boolean_deserialize(reader);
        record->lamp_switch = p72416_t3_union_null_or_boolean_deserialize(reader);
    }

    return record;
}

