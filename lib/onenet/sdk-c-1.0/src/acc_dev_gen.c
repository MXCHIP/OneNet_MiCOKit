# include "acc_dev_gen.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

# ifndef REGINA_UNION_NULL_OR_STRING_C_
# define REGINA_UNION_NULL_OR_STRING_C_
static void regina_union_null_or_string_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_STRING_BRANCH_1:
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

static size_t regina_union_null_or_string_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_STRING_BRANCH_1:
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

static void regina_union_null_or_string_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_STRING_BRANCH_1:
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
static regina_union_t *regina_union_null_or_string_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_null_or_string_serialize;
        regina_union->get_size = regina_union_null_or_string_get_size;
        regina_union->destroy = regina_union_null_or_string_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_null_or_string_branch_0_create()
{
    regina_union_t *regina_union = regina_union_null_or_string_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NULL_OR_STRING_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_null_or_string_branch_1_create()
{
    regina_union_t *regina_union = regina_union_null_or_string_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NULL_OR_STRING_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *regina_union_null_or_string_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_null_or_string_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_STRING_BRANCH_1: {
            regina_union->data = regina_string_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_NULL_OR_STRING_C_ */

# ifndef REGINA_UNION_LONG_OR_NULL_C_
# define REGINA_UNION_LONG_OR_NULL_C_
static void regina_union_long_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_LONG_OR_NULL_BRANCH_0:
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

static size_t regina_union_long_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_LONG_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                union_size += regina_long_get_size(regina_union->data);
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

static void regina_union_long_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_LONG_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_long_serialize(writer, regina_union->data);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_long_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_long_or_null_serialize;
        regina_union->get_size = regina_union_long_or_null_get_size;
        regina_union->destroy = regina_union_long_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_long_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_long_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_LONG_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_long_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_long_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_LONG_OR_NULL_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *regina_union_long_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_long_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_LONG_OR_NULL_BRANCH_0: {
            regina_union->data = regina_long_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_LONG_OR_NULL_C_ */

static void regina_sync_response_ack_destroy(void *data)
{
    if (data) {
        regina_sync_response_ack_t *record = (regina_sync_response_ack_t *)data;

        if (record->cmd_ack && record->cmd_ack->destroy) {
            record->cmd_ack->destroy(record->cmd_ack);
        }
        if (record->notify_ack && record->notify_ack->destroy) {
            record->notify_ack->destroy(record->notify_ack);
        }
        if (record->config_ack && record->config_ack->destroy) {
            record->config_ack->destroy(record->config_ack);
        }
        if (record->software_ack && record->software_ack->destroy) {
            record->software_ack->destroy(record->software_ack);
        }
        regina_data_destroy(record);
    }
}

static void regina_sync_response_ack_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_sync_response_ack_t *record = (regina_sync_response_ack_t *)data;

        record->cmd_ack->serialize(writer, record->cmd_ack);
        record->notify_ack->serialize(writer, record->notify_ack);
        record->config_ack->serialize(writer, record->config_ack);
        record->software_ack->serialize(writer, record->software_ack);
    }
}

static size_t regina_sync_response_ack_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_sync_response_ack_t *record = (regina_sync_response_ack_t *)data;

        record_size += record->cmd_ack->get_size(record->cmd_ack);
        record_size += record->notify_ack->get_size(record->notify_ack);
        record_size += record->config_ack->get_size(record->config_ack);
        record_size += record->software_ack->get_size(record->software_ack);

        return record_size;
    }

    return 0;
}

regina_sync_response_ack_t *regina_sync_response_ack_create()
{
    regina_sync_response_ack_t *record = 
            (regina_sync_response_ack_t *)regina_calloc(1, sizeof(regina_sync_response_ack_t));

    if (record) {
        record->serialize = regina_sync_response_ack_serialize;
        record->get_size = regina_sync_response_ack_get_size;
        record->destroy = regina_sync_response_ack_destroy;
    }

    return record;
}

regina_sync_response_ack_t *regina_sync_response_ack_deserialize(avro_reader_t reader)
{
    regina_sync_response_ack_t *record = 
            (regina_sync_response_ack_t *)regina_malloc(sizeof(regina_sync_response_ack_t));

    if (record) {

        record->serialize = regina_sync_response_ack_serialize;
        record->get_size = regina_sync_response_ack_get_size;
        record->destroy = regina_sync_response_ack_destroy;

        record->cmd_ack = regina_union_null_or_string_deserialize(reader);
        record->notify_ack = regina_union_null_or_string_deserialize(reader);
        record->config_ack = regina_union_long_or_null_deserialize(reader);
        record->software_ack = regina_union_long_or_null_deserialize(reader);
    }

    return record;
}

static void regina_profile_sync_request_destroy(void *data)
{
    if (data) {
        regina_profile_sync_request_t *record = (regina_profile_sync_request_t *)data;

        regina_bytes_destroy(record->profile_body);
        regina_data_destroy(record);
    }
}

static void regina_profile_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_profile_sync_request_t *record = (regina_profile_sync_request_t *)data;

        regina_bytes_serialize(writer, record->profile_body);
    }
}

static size_t regina_profile_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_profile_sync_request_t *record = (regina_profile_sync_request_t *)data;

        record_size += regina_bytes_get_size(record->profile_body);

        return record_size;
    }

    return 0;
}

regina_profile_sync_request_t *regina_profile_sync_request_create()
{
    regina_profile_sync_request_t *record = 
            (regina_profile_sync_request_t *)regina_calloc(1, sizeof(regina_profile_sync_request_t));

    if (record) {
        record->serialize = regina_profile_sync_request_serialize;
        record->get_size = regina_profile_sync_request_get_size;
        record->destroy = regina_profile_sync_request_destroy;
    }

    return record;
}



regina_profile_sync_response_t *regina_profile_sync_response_deserialize(avro_reader_t reader)
{
    regina_profile_sync_response_t *record = 
            (regina_profile_sync_response_t *)regina_malloc(sizeof(regina_profile_sync_response_t));

    if (record) {
        int64_t __regina_enum_value;

        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
    }

    return record;
}


static void regina_notify_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_notify_sync_request_t *record = (regina_notify_sync_request_t *)data;

        regina_long_serialize(writer, &record->seq_num);
    }
}

static size_t regina_notify_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_notify_sync_request_t *record = (regina_notify_sync_request_t *)data;

        record_size += regina_long_get_size(&record->seq_num);

        return record_size;
    }

    return 0;
}

regina_notify_sync_request_t *regina_notify_sync_request_create()
{
    regina_notify_sync_request_t *record = 
            (regina_notify_sync_request_t *)regina_calloc(1, sizeof(regina_notify_sync_request_t));

    if (record) {
        record->serialize = regina_notify_sync_request_serialize;
        record->get_size = regina_notify_sync_request_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

static void regina_cmd_sync_response_destroy(void *data)
{
    if (data) {
        regina_cmd_sync_response_t *record = (regina_cmd_sync_response_t *)data;

        regina_string_destroy(record->uuid);
        regina_bytes_destroy(record->cmd_body);
        regina_data_destroy(record);
    }
}


regina_cmd_sync_response_t *regina_cmd_sync_response_deserialize(avro_reader_t reader)
{
    regina_cmd_sync_response_t *record = 
            (regina_cmd_sync_response_t *)regina_malloc(sizeof(regina_cmd_sync_response_t));

    if (record) {

        record->destroy = regina_cmd_sync_response_destroy;

        record->uuid = regina_string_deserialize(reader);
        record->cmd_body = regina_bytes_deserialize(reader);
    }

    return record;
}

# ifndef REGINA_UNION_STRING_OR_NULL_C_
# define REGINA_UNION_STRING_OR_NULL_C_
static void regina_union_string_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_STRING_OR_NULL_BRANCH_0:
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

static size_t regina_union_string_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_STRING_OR_NULL_BRANCH_0:
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

static void regina_union_string_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_STRING_OR_NULL_BRANCH_0:
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
static regina_union_t *regina_union_string_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_string_or_null_serialize;
        regina_union->get_size = regina_union_string_or_null_get_size;
        regina_union->destroy = regina_union_string_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_string_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_string_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_STRING_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_string_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_string_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_STRING_OR_NULL_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *regina_union_string_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_string_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_STRING_OR_NULL_BRANCH_0: {
            regina_union->data = regina_string_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_STRING_OR_NULL_C_ */

# ifndef REGINA_UNION_BYTES_OR_NULL_C_
# define REGINA_UNION_BYTES_OR_NULL_C_
static void regina_union_bytes_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_BYTES_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_bytes_destroy(regina_union->data);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_bytes_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_BYTES_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                union_size += regina_bytes_get_size(regina_union->data);
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

static void regina_union_bytes_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_BYTES_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_bytes_serialize(writer, regina_union->data);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_bytes_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_bytes_or_null_serialize;
        regina_union->get_size = regina_union_bytes_or_null_get_size;
        regina_union->destroy = regina_union_bytes_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_bytes_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_bytes_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_BYTES_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_bytes_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_bytes_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_BYTES_OR_NULL_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *regina_union_bytes_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_bytes_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_BYTES_OR_NULL_BRANCH_0: {
            regina_union->data = regina_bytes_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_BYTES_OR_NULL_C_ */

static void regina_notify_sync_response_destroy(void *data)
{
    if (data) {
        regina_notify_sync_response_t *record = (regina_notify_sync_response_t *)data;

        if (record->uuid && record->uuid->destroy) {
            record->uuid->destroy(record->uuid);
        }
        if (record->notification_body && record->notification_body->destroy) {
            record->notification_body->destroy(record->notification_body);
        }
        regina_data_destroy(record);
    }
}


regina_notify_sync_response_t *regina_notify_sync_response_deserialize(avro_reader_t reader)
{
    regina_notify_sync_response_t *record = 
            (regina_notify_sync_response_t *)regina_malloc(sizeof(regina_notify_sync_response_t));

    if (record) {

        record->destroy = regina_notify_sync_response_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
        record->uuid = regina_union_string_or_null_deserialize(reader);
        record->notification_body = regina_union_bytes_or_null_deserialize(reader);
    }

    return record;
}

static void regina_event_sync_request_destroy(void *data)
{
    if (data) {
        regina_event_sync_request_t *record = (regina_event_sync_request_t *)data;

        regina_bytes_destroy(record->body);
        regina_data_destroy(record);
    }
}

static void regina_event_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_event_sync_request_t *record = (regina_event_sync_request_t *)data;

        regina_long_serialize(writer, &record->seq_num);
        regina_long_serialize(writer, &record->at);
        regina_enum_serialize(writer, &record->level);
        regina_bytes_serialize(writer, record->body);
    }
}

static size_t regina_event_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_event_sync_request_t *record = (regina_event_sync_request_t *)data;

        record_size += regina_long_get_size(&record->seq_num);
        record_size += regina_long_get_size(&record->at);
        record_size += regina_enum_get_size(&record->level);
        record_size += regina_bytes_get_size(record->body);

        return record_size;
    }

    return 0;
}

regina_event_sync_request_t *regina_event_sync_request_create()
{
    regina_event_sync_request_t *record = 
            (regina_event_sync_request_t *)regina_calloc(1, sizeof(regina_event_sync_request_t));

    if (record) {
        record->serialize = regina_event_sync_request_serialize;
        record->get_size = regina_event_sync_request_get_size;
        record->destroy = regina_event_sync_request_destroy;
    }

    return record;
}



regina_event_sync_response_t *regina_event_sync_response_deserialize(avro_reader_t reader)
{
    regina_event_sync_response_t *record = 
            (regina_event_sync_response_t *)regina_malloc(sizeof(regina_event_sync_response_t));

    if (record) {
        int64_t __regina_enum_value;

        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
    }

    return record;
}

static void regina_sensor_data_entry_destroy(void *data)
{
    if (data) {
        regina_sensor_data_entry_t *record = (regina_sensor_data_entry_t *)data;

        regina_bytes_destroy(record->data);
        regina_data_destroy(record);
    }
}

static void regina_sensor_data_entry_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_sensor_data_entry_t *record = (regina_sensor_data_entry_t *)data;

        regina_long_serialize(writer, &record->at);
        regina_bytes_serialize(writer, record->data);
    }
}

static size_t regina_sensor_data_entry_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_sensor_data_entry_t *record = (regina_sensor_data_entry_t *)data;

        record_size += regina_long_get_size(&record->at);
        record_size += regina_bytes_get_size(record->data);

        return record_size;
    }

    return 0;
}

regina_sensor_data_entry_t *regina_sensor_data_entry_create()
{
    regina_sensor_data_entry_t *record = 
            (regina_sensor_data_entry_t *)regina_calloc(1, sizeof(regina_sensor_data_entry_t));

    if (record) {
        record->serialize = regina_sensor_data_entry_serialize;
        record->get_size = regina_sensor_data_entry_get_size;
        record->destroy = regina_sensor_data_entry_destroy;
    }

    return record;
}

static void regina_sensor_data_sync_request_destroy(void *data)
{
    if (data) {
        regina_sensor_data_sync_request_t *record = (regina_sensor_data_sync_request_t *)data;

                    regina_list_destroy(record->sensor_data_entries, regina_sensor_data_entry_destroy);
                    regina_data_destroy(record);
    }
}

static void regina_sensor_data_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_sensor_data_sync_request_t *record = (regina_sensor_data_sync_request_t *)data;

        regina_long_serialize(writer, &record->seq_num);
                    regina_array_serialize(writer, record->sensor_data_entries, regina_sensor_data_entry_serialize);
                }
}

static size_t regina_sensor_data_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_sensor_data_sync_request_t *record = (regina_sensor_data_sync_request_t *)data;

        record_size += regina_long_get_size(&record->seq_num);
                    record_size += regina_array_get_size(record->sensor_data_entries, regina_sensor_data_entry_get_size);
            
        return record_size;
    }

    return 0;
}

regina_sensor_data_sync_request_t *regina_sensor_data_sync_request_create()
{
    regina_sensor_data_sync_request_t *record = 
            (regina_sensor_data_sync_request_t *)regina_calloc(1, sizeof(regina_sensor_data_sync_request_t));

    if (record) {
        record->serialize = regina_sensor_data_sync_request_serialize;
        record->get_size = regina_sensor_data_sync_request_get_size;
        record->destroy = regina_sensor_data_sync_request_destroy;
    }

    return record;
}

static void regina_sensor_data_sync_response_destroy(void *data)
{
    if (data) {
        regina_sensor_data_sync_response_t *record = (regina_sensor_data_sync_response_t *)data;

            regina_list_destroy(record->sensor_data_delivery_status, regina_data_destroy);
            regina_data_destroy(record);
    }
}


regina_sensor_data_sync_response_t *regina_sensor_data_sync_response_deserialize(avro_reader_t reader)
{
    regina_sensor_data_sync_response_t *record = 
            (regina_sensor_data_sync_response_t *)regina_malloc(sizeof(regina_sensor_data_sync_response_t));

    if (record) {

        record->destroy = regina_sensor_data_sync_response_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
            record->sensor_data_delivery_status = regina_array_deserialize_wo_ctx(reader, (deserialize_wo_ctx_fn)regina_enum_deserialize);
        }

    return record;
}

static void regina_config_sync_request_destroy(void *data)
{
    if (data) {
        regina_config_sync_request_t *record = (regina_config_sync_request_t *)data;

        regina_bytes_destroy(record->md5);
        regina_data_destroy(record);
    }
}

static void regina_config_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_config_sync_request_t *record = (regina_config_sync_request_t *)data;

        regina_long_serialize(writer, &record->seq_num);
        regina_bytes_serialize(writer, record->md5);
    }
}

static size_t regina_config_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_config_sync_request_t *record = (regina_config_sync_request_t *)data;

        record_size += regina_long_get_size(&record->seq_num);
        record_size += regina_bytes_get_size(record->md5);

        return record_size;
    }

    return 0;
}

regina_config_sync_request_t *regina_config_sync_request_create()
{
    regina_config_sync_request_t *record = 
            (regina_config_sync_request_t *)regina_calloc(1, sizeof(regina_config_sync_request_t));

    if (record) {
        record->serialize = regina_config_sync_request_serialize;
        record->get_size = regina_config_sync_request_get_size;
        record->destroy = regina_config_sync_request_destroy;
    }

    return record;
}

static void regina_config_sync_response_destroy(void *data)
{
    if (data) {
        regina_config_sync_response_t *record = (regina_config_sync_response_t *)data;

        if (record->config_body && record->config_body->destroy) {
            record->config_body->destroy(record->config_body);
        }
        regina_data_destroy(record);
    }
}


regina_config_sync_response_t *regina_config_sync_response_deserialize(avro_reader_t reader)
{
    regina_config_sync_response_t *record = 
            (regina_config_sync_response_t *)regina_malloc(sizeof(regina_config_sync_response_t));

    if (record) {

        record->destroy = regina_config_sync_response_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
        record->config_body = regina_union_bytes_or_null_deserialize(reader);
    }

    return record;
}


static void regina_soft_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_soft_sync_request_t *record = (regina_soft_sync_request_t *)data;

        regina_long_serialize(writer, &record->seq_num);
    }
}

static size_t regina_soft_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_soft_sync_request_t *record = (regina_soft_sync_request_t *)data;

        record_size += regina_long_get_size(&record->seq_num);

        return record_size;
    }

    return 0;
}

regina_soft_sync_request_t *regina_soft_sync_request_create()
{
    regina_soft_sync_request_t *record = 
            (regina_soft_sync_request_t *)regina_calloc(1, sizeof(regina_soft_sync_request_t));

    if (record) {
        record->serialize = regina_soft_sync_request_serialize;
        record->get_size = regina_soft_sync_request_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

static void regina_software_update_info_destroy(void *data)
{
    if (data) {
        regina_software_update_info_t *record = (regina_software_update_info_t *)data;

        regina_string_destroy(record->module_name);
        regina_string_destroy(record->url);
        regina_string_destroy(record->version);
        regina_bytes_destroy(record->md5);
        regina_data_destroy(record);
    }
}

static void regina_software_update_info_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_software_update_info_t *record = (regina_software_update_info_t *)data;

        regina_string_serialize(writer, record->module_name);
        regina_string_serialize(writer, record->url);
        regina_string_serialize(writer, record->version);
        regina_bytes_serialize(writer, record->md5);
    }
}

static size_t regina_software_update_info_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_software_update_info_t *record = (regina_software_update_info_t *)data;

        record_size += regina_string_get_size(record->module_name);
        record_size += regina_string_get_size(record->url);
        record_size += regina_string_get_size(record->version);
        record_size += regina_bytes_get_size(record->md5);

        return record_size;
    }

    return 0;
}

regina_software_update_info_t *regina_software_update_info_create()
{
    regina_software_update_info_t *record = 
            (regina_software_update_info_t *)regina_calloc(1, sizeof(regina_software_update_info_t));

    if (record) {
        record->serialize = regina_software_update_info_serialize;
        record->get_size = regina_software_update_info_get_size;
        record->destroy = regina_software_update_info_destroy;
    }

    return record;
}

regina_software_update_info_t *regina_software_update_info_deserialize(avro_reader_t reader)
{
    regina_software_update_info_t *record = 
            (regina_software_update_info_t *)regina_malloc(sizeof(regina_software_update_info_t));

    if (record) {

        record->serialize = regina_software_update_info_serialize;
        record->get_size = regina_software_update_info_get_size;
        record->destroy = regina_software_update_info_destroy;

        record->module_name = regina_string_deserialize(reader);
        record->url = regina_string_deserialize(reader);
        record->version = regina_string_deserialize(reader);
        record->md5 = regina_bytes_deserialize(reader);
    }

    return record;
}

static void regina_software_info_destroy(void *data)
{
    if (data) {
        regina_software_info_t *record = (regina_software_info_t *)data;

        regina_string_destroy(record->module_name);
        regina_string_destroy(record->version);
        regina_data_destroy(record);
    }
}

static void regina_software_info_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_software_info_t *record = (regina_software_info_t *)data;

        regina_string_serialize(writer, record->module_name);
        regina_string_serialize(writer, record->version);
    }
}

static size_t regina_software_info_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_software_info_t *record = (regina_software_info_t *)data;

        record_size += regina_string_get_size(record->module_name);
        record_size += regina_string_get_size(record->version);

        return record_size;
    }

    return 0;
}

regina_software_info_t *regina_software_info_create()
{
    regina_software_info_t *record = 
            (regina_software_info_t *)regina_calloc(1, sizeof(regina_software_info_t));

    if (record) {
        record->serialize = regina_software_info_serialize;
        record->get_size = regina_software_info_get_size;
        record->destroy = regina_software_info_destroy;
    }

    return record;
}

regina_software_info_t *regina_software_info_deserialize(avro_reader_t reader)
{
    regina_software_info_t *record = 
            (regina_software_info_t *)regina_malloc(sizeof(regina_software_info_t));

    if (record) {

        record->serialize = regina_software_info_serialize;
        record->get_size = regina_software_info_get_size;
        record->destroy = regina_software_info_destroy;

        record->module_name = regina_string_deserialize(reader);
        record->version = regina_string_deserialize(reader);
    }

    return record;
}

static void regina_soft_sync_response_destroy(void *data)
{
    if (data) {
        regina_soft_sync_response_t *record = (regina_soft_sync_response_t *)data;

                    regina_list_destroy(record->software_update_info, regina_software_update_info_destroy);
                    regina_data_destroy(record);
    }
}


regina_soft_sync_response_t *regina_soft_sync_response_deserialize(avro_reader_t reader)
{
    regina_soft_sync_response_t *record = 
            (regina_soft_sync_response_t *)regina_malloc(sizeof(regina_soft_sync_response_t));

    if (record) {

        record->destroy = regina_soft_sync_response_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
            record->software_update_info = regina_array_deserialize_wo_ctx(reader, (deserialize_wo_ctx_fn)regina_software_update_info_deserialize);
        }

    return record;
}

static void regina_cmd_confirm_destroy(void *data)
{
    if (data) {
        regina_cmd_confirm_t *record = (regina_cmd_confirm_t *)data;

        regina_string_destroy(record->uuid);
        regina_bytes_destroy(record->resp_nf_body);
        regina_data_destroy(record);
    }
}

static void regina_cmd_confirm_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_cmd_confirm_t *record = (regina_cmd_confirm_t *)data;

        regina_string_serialize(writer, record->uuid);
        regina_bytes_serialize(writer, record->resp_nf_body);
    }
}

static size_t regina_cmd_confirm_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_cmd_confirm_t *record = (regina_cmd_confirm_t *)data;

        record_size += regina_string_get_size(record->uuid);
        record_size += regina_bytes_get_size(record->resp_nf_body);

        return record_size;
    }

    return 0;
}

regina_cmd_confirm_t *regina_cmd_confirm_create()
{
    regina_cmd_confirm_t *record = 
            (regina_cmd_confirm_t *)regina_calloc(1, sizeof(regina_cmd_confirm_t));

    if (record) {
        record->serialize = regina_cmd_confirm_serialize;
        record->get_size = regina_cmd_confirm_get_size;
        record->destroy = regina_cmd_confirm_destroy;
    }

    return record;
}

static void regina_notify_confirm_destroy(void *data)
{
    if (data) {
        regina_notify_confirm_t *record = (regina_notify_confirm_t *)data;

        regina_string_destroy(record->uuid);
        regina_bytes_destroy(record->resp_nf_body);
        regina_data_destroy(record);
    }
}

static void regina_notify_confirm_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_notify_confirm_t *record = (regina_notify_confirm_t *)data;

        regina_string_serialize(writer, record->uuid);
        regina_bytes_serialize(writer, record->resp_nf_body);
    }
}

static size_t regina_notify_confirm_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_notify_confirm_t *record = (regina_notify_confirm_t *)data;

        record_size += regina_string_get_size(record->uuid);
        record_size += regina_bytes_get_size(record->resp_nf_body);

        return record_size;
    }

    return 0;
}

regina_notify_confirm_t *regina_notify_confirm_create()
{
    regina_notify_confirm_t *record = 
            (regina_notify_confirm_t *)regina_calloc(1, sizeof(regina_notify_confirm_t));

    if (record) {
        record->serialize = regina_notify_confirm_serialize;
        record->get_size = regina_notify_confirm_get_size;
        record->destroy = regina_notify_confirm_destroy;
    }

    return record;
}

static void regina_cmd_confirm_ack_destroy(void *data)
{
    if (data) {
        regina_cmd_confirm_ack_t *record = (regina_cmd_confirm_ack_t *)data;

        regina_string_destroy(record->cmd_confirm_ack);
        regina_data_destroy(record);
    }
}


regina_cmd_confirm_ack_t *regina_cmd_confirm_ack_deserialize(avro_reader_t reader)
{
    regina_cmd_confirm_ack_t *record = 
            (regina_cmd_confirm_ack_t *)regina_malloc(sizeof(regina_cmd_confirm_ack_t));

    if (record) {
        int64_t __regina_enum_value;

        record->destroy = regina_cmd_confirm_ack_destroy;

        record->cmd_confirm_ack = regina_string_deserialize(reader);
        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
    }

    return record;
}

static void regina_notify_confirm_ack_destroy(void *data)
{
    if (data) {
        regina_notify_confirm_ack_t *record = (regina_notify_confirm_ack_t *)data;

        regina_string_destroy(record->notify_confirm_ack);
        regina_data_destroy(record);
    }
}


regina_notify_confirm_ack_t *regina_notify_confirm_ack_deserialize(avro_reader_t reader)
{
    regina_notify_confirm_ack_t *record = 
            (regina_notify_confirm_ack_t *)regina_malloc(sizeof(regina_notify_confirm_ack_t));

    if (record) {
        int64_t __regina_enum_value;

        record->destroy = regina_notify_confirm_ack_destroy;

        record->notify_confirm_ack = regina_string_deserialize(reader);
        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
    }

    return record;
}

static void regina_register_sync_request_destroy(void *data)
{
    if (data) {
        regina_register_sync_request_t *record = (regina_register_sync_request_t *)data;

        regina_string_destroy(record->regiter_code);
        regina_string_destroy(record->sdk_uuid);
                    regina_list_destroy(record->software_info, regina_software_info_destroy);
                    regina_data_destroy(record);
    }
}

static void regina_register_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_register_sync_request_t *record = (regina_register_sync_request_t *)data;

        regina_string_serialize(writer, record->regiter_code);
        regina_string_serialize(writer, record->sdk_uuid);
                    regina_array_serialize(writer, record->software_info, regina_software_info_serialize);
                }
}

static size_t regina_register_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_register_sync_request_t *record = (regina_register_sync_request_t *)data;

        record_size += regina_string_get_size(record->regiter_code);
        record_size += regina_string_get_size(record->sdk_uuid);
                    record_size += regina_array_get_size(record->software_info, regina_software_info_get_size);
            
        return record_size;
    }

    return 0;
}

regina_register_sync_request_t *regina_register_sync_request_create()
{
    regina_register_sync_request_t *record = 
            (regina_register_sync_request_t *)regina_calloc(1, sizeof(regina_register_sync_request_t));

    if (record) {
        record->serialize = regina_register_sync_request_serialize;
        record->get_size = regina_register_sync_request_get_size;
        record->destroy = regina_register_sync_request_destroy;
    }

    return record;
}

regina_register_sync_request_t *regina_register_sync_request_deserialize(avro_reader_t reader)
{
    regina_register_sync_request_t *record = 
            (regina_register_sync_request_t *)regina_malloc(sizeof(regina_register_sync_request_t));

    if (record) {

        record->serialize = regina_register_sync_request_serialize;
        record->get_size = regina_register_sync_request_get_size;
        record->destroy = regina_register_sync_request_destroy;

        record->regiter_code = regina_string_deserialize(reader);
        record->sdk_uuid = regina_string_deserialize(reader);
            record->software_info = regina_array_deserialize_wo_ctx(reader, (deserialize_wo_ctx_fn)regina_software_info_deserialize);
        }

    return record;
}

static void regina_register_sync_response_destroy(void *data)
{
    if (data) {
        regina_register_sync_response_t *record = (regina_register_sync_response_t *)data;

        if (record->device_id && record->device_id->destroy) {
            record->device_id->destroy(record->device_id);
        }
        if (record->auth_code && record->auth_code->destroy) {
            record->auth_code->destroy(record->auth_code);
        }
        regina_data_destroy(record);
    }
}

static void regina_register_sync_response_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_register_sync_response_t *record = (regina_register_sync_response_t *)data;

        regina_enum_serialize(writer, &record->result);
        record->device_id->serialize(writer, record->device_id);
        record->auth_code->serialize(writer, record->auth_code);
    }
}

static size_t regina_register_sync_response_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_register_sync_response_t *record = (regina_register_sync_response_t *)data;

        record_size += regina_enum_get_size(&record->result);
        record_size += record->device_id->get_size(record->device_id);
        record_size += record->auth_code->get_size(record->auth_code);

        return record_size;
    }

    return 0;
}

regina_register_sync_response_t *regina_register_sync_response_create()
{
    regina_register_sync_response_t *record = 
            (regina_register_sync_response_t *)regina_calloc(1, sizeof(regina_register_sync_response_t));

    if (record) {
        record->serialize = regina_register_sync_response_serialize;
        record->get_size = regina_register_sync_response_get_size;
        record->destroy = regina_register_sync_response_destroy;
    }

    return record;
}

regina_register_sync_response_t *regina_register_sync_response_deserialize(avro_reader_t reader)
{
    regina_register_sync_response_t *record = 
            (regina_register_sync_response_t *)regina_malloc(sizeof(regina_register_sync_response_t));

    if (record) {
        int64_t __regina_enum_value;

        record->serialize = regina_register_sync_response_serialize;
        record->get_size = regina_register_sync_response_get_size;
        record->destroy = regina_register_sync_response_destroy;

        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
        record->device_id = regina_union_long_or_null_deserialize(reader);
        record->auth_code = regina_union_string_or_null_deserialize(reader);
    }

    return record;
}

static void regina_authen_sync_request_destroy(void *data)
{
    if (data) {
        regina_authen_sync_request_t *record = (regina_authen_sync_request_t *)data;

        regina_string_destroy(record->sdk_uuid);
        regina_string_destroy(record->auth_code);
                    regina_list_destroy(record->software_info, regina_software_info_destroy);
                    regina_data_destroy(record);
    }
}

static void regina_authen_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_authen_sync_request_t *record = (regina_authen_sync_request_t *)data;

        regina_long_serialize(writer, &record->device_id);
        regina_string_serialize(writer, record->sdk_uuid);
        regina_string_serialize(writer, record->auth_code);
                    regina_array_serialize(writer, record->software_info, regina_software_info_serialize);
                }
}

static size_t regina_authen_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_authen_sync_request_t *record = (regina_authen_sync_request_t *)data;

        record_size += regina_long_get_size(&record->device_id);
        record_size += regina_string_get_size(record->sdk_uuid);
        record_size += regina_string_get_size(record->auth_code);
                    record_size += regina_array_get_size(record->software_info, regina_software_info_get_size);
            
        return record_size;
    }

    return 0;
}

regina_authen_sync_request_t *regina_authen_sync_request_create()
{
    regina_authen_sync_request_t *record = 
            (regina_authen_sync_request_t *)regina_calloc(1, sizeof(regina_authen_sync_request_t));

    if (record) {
        record->serialize = regina_authen_sync_request_serialize;
        record->get_size = regina_authen_sync_request_get_size;
        record->destroy = regina_authen_sync_request_destroy;
    }

    return record;
}

regina_authen_sync_request_t *regina_authen_sync_request_deserialize(avro_reader_t reader)
{
    regina_authen_sync_request_t *record = 
            (regina_authen_sync_request_t *)regina_malloc(sizeof(regina_authen_sync_request_t));

    if (record) {

        record->serialize = regina_authen_sync_request_serialize;
        record->get_size = regina_authen_sync_request_get_size;
        record->destroy = regina_authen_sync_request_destroy;

        avro_binary_encoding.read_long(reader, &record->device_id);
        record->sdk_uuid = regina_string_deserialize(reader);
        record->auth_code = regina_string_deserialize(reader);
            record->software_info = regina_array_deserialize_wo_ctx(reader, (deserialize_wo_ctx_fn)regina_software_info_deserialize);
        }

    return record;
}


static void regina_authen_sync_response_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_authen_sync_response_t *record = (regina_authen_sync_response_t *)data;

        regina_enum_serialize(writer, &record->result);
    }
}

static size_t regina_authen_sync_response_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_authen_sync_response_t *record = (regina_authen_sync_response_t *)data;

        record_size += regina_enum_get_size(&record->result);

        return record_size;
    }

    return 0;
}

regina_authen_sync_response_t *regina_authen_sync_response_create()
{
    regina_authen_sync_response_t *record = 
            (regina_authen_sync_response_t *)regina_calloc(1, sizeof(regina_authen_sync_response_t));

    if (record) {
        record->serialize = regina_authen_sync_response_serialize;
        record->get_size = regina_authen_sync_response_get_size;
        record->destroy = regina_data_destroy;
    }

    return record;
}

regina_authen_sync_response_t *regina_authen_sync_response_deserialize(avro_reader_t reader)
{
    regina_authen_sync_response_t *record = 
            (regina_authen_sync_response_t *)regina_malloc(sizeof(regina_authen_sync_response_t));

    if (record) {
        int64_t __regina_enum_value;

        record->serialize = regina_authen_sync_response_serialize;
        record->get_size = regina_authen_sync_response_get_size;
        record->destroy = regina_data_destroy;

        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
    }

    return record;
}

static void regina_thirdparty_service_request_destroy(void *data)
{
    if (data) {
        regina_thirdparty_service_request_t *record = (regina_thirdparty_service_request_t *)data;

        regina_bytes_destroy(record->data);
        if (record->ext_data && record->ext_data->destroy) {
            record->ext_data->destroy(record->ext_data);
        }
        regina_data_destroy(record);
    }
}

static void regina_thirdparty_service_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_thirdparty_service_request_t *record = (regina_thirdparty_service_request_t *)data;

        regina_long_serialize(writer, &record->seq_num);
        regina_int_serialize(writer, &record->qos);
        regina_bytes_serialize(writer, record->data);
        record->ext_data->serialize(writer, record->ext_data);
    }
}

static size_t regina_thirdparty_service_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_thirdparty_service_request_t *record = (regina_thirdparty_service_request_t *)data;

        record_size += regina_long_get_size(&record->seq_num);
        record_size += regina_int_get_size(&record->qos);
        record_size += regina_bytes_get_size(record->data);
        record_size += record->ext_data->get_size(record->ext_data);

        return record_size;
    }

    return 0;
}

regina_thirdparty_service_request_t *regina_thirdparty_service_request_create()
{
    regina_thirdparty_service_request_t *record = 
            (regina_thirdparty_service_request_t *)regina_calloc(1, sizeof(regina_thirdparty_service_request_t));

    if (record) {
        record->serialize = regina_thirdparty_service_request_serialize;
        record->get_size = regina_thirdparty_service_request_get_size;
        record->destroy = regina_thirdparty_service_request_destroy;
    }

    return record;
}

regina_thirdparty_service_request_t *regina_thirdparty_service_request_deserialize(avro_reader_t reader)
{
    regina_thirdparty_service_request_t *record = 
            (regina_thirdparty_service_request_t *)regina_malloc(sizeof(regina_thirdparty_service_request_t));

    if (record) {

        record->serialize = regina_thirdparty_service_request_serialize;
        record->get_size = regina_thirdparty_service_request_get_size;
        record->destroy = regina_thirdparty_service_request_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
        avro_binary_encoding.read_int(reader, &record->qos);
        record->data = regina_bytes_deserialize(reader);
        record->ext_data = regina_union_bytes_or_null_deserialize(reader);
    }

    return record;
}

static void regina_thirdparty_service_response_destroy(void *data)
{
    if (data) {
        regina_thirdparty_service_response_t *record = (regina_thirdparty_service_response_t *)data;

        if (record->data && record->data->destroy) {
            record->data->destroy(record->data);
        }
        if (record->ext_data && record->ext_data->destroy) {
            record->ext_data->destroy(record->ext_data);
        }
        regina_data_destroy(record);
    }
}

static void regina_thirdparty_service_response_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_thirdparty_service_response_t *record = (regina_thirdparty_service_response_t *)data;

        regina_long_serialize(writer, &record->seq_num);
        regina_enum_serialize(writer, &record->result);
        record->data->serialize(writer, record->data);
        record->ext_data->serialize(writer, record->ext_data);
    }
}

static size_t regina_thirdparty_service_response_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_thirdparty_service_response_t *record = (regina_thirdparty_service_response_t *)data;

        record_size += regina_long_get_size(&record->seq_num);
        record_size += regina_enum_get_size(&record->result);
        record_size += record->data->get_size(record->data);
        record_size += record->ext_data->get_size(record->ext_data);

        return record_size;
    }

    return 0;
}

regina_thirdparty_service_response_t *regina_thirdparty_service_response_create()
{
    regina_thirdparty_service_response_t *record = 
            (regina_thirdparty_service_response_t *)regina_calloc(1, sizeof(regina_thirdparty_service_response_t));

    if (record) {
        record->serialize = regina_thirdparty_service_response_serialize;
        record->get_size = regina_thirdparty_service_response_get_size;
        record->destroy = regina_thirdparty_service_response_destroy;
    }

    return record;
}

regina_thirdparty_service_response_t *regina_thirdparty_service_response_deserialize(avro_reader_t reader)
{
    regina_thirdparty_service_response_t *record = 
            (regina_thirdparty_service_response_t *)regina_malloc(sizeof(regina_thirdparty_service_response_t));

    if (record) {
        int64_t __regina_enum_value;

        record->serialize = regina_thirdparty_service_response_serialize;
        record->get_size = regina_thirdparty_service_response_get_size;
        record->destroy = regina_thirdparty_service_response_destroy;

        avro_binary_encoding.read_long(reader, &record->seq_num);
        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->result = __regina_enum_value;
        record->data = regina_union_bytes_or_null_deserialize(reader);
        record->ext_data = regina_union_bytes_or_null_deserialize(reader);
    }

    return record;
}

# ifndef REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_C_
static void regina_union_register_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_register_sync_request_t *record = (regina_register_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_register_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_register_sync_request_t * record = (regina_register_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_register_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_register_sync_request_t * record = (regina_register_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_register_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_register_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_register_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_register_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_register_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_register_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_register_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_register_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_C_
static void regina_union_authen_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_authen_sync_request_t *record = (regina_authen_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_authen_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_authen_sync_request_t * record = (regina_authen_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_authen_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_authen_sync_request_t * record = (regina_authen_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_authen_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_authen_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_authen_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_authen_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_authen_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_authen_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_authen_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_authen_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_C_
static void regina_union_profile_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_profile_sync_request_t *record = (regina_profile_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_profile_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_profile_sync_request_t * record = (regina_profile_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_profile_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_profile_sync_request_t * record = (regina_profile_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_profile_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_profile_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_profile_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_profile_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_profile_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_profile_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_profile_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_profile_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_C_
static void regina_union_notify_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_sync_request_t *record = (regina_notify_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_notify_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_sync_request_t * record = (regina_notify_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_notify_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_sync_request_t * record = (regina_notify_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_notify_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_notify_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_notify_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_notify_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_notify_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_notify_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_notify_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_notify_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_C_
static void regina_union_event_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_event_sync_request_t *record = (regina_event_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_event_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_event_sync_request_t * record = (regina_event_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_event_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_event_sync_request_t * record = (regina_event_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_event_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_event_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_event_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_event_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_event_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_event_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_event_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_event_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_C_
static void regina_union_sensor_data_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sensor_data_sync_request_t *record = (regina_sensor_data_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_sensor_data_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sensor_data_sync_request_t * record = (regina_sensor_data_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_sensor_data_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sensor_data_sync_request_t * record = (regina_sensor_data_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_sensor_data_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_sensor_data_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_sensor_data_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_sensor_data_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_sensor_data_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_sensor_data_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_sensor_data_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_sensor_data_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_C_
static void regina_union_config_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_config_sync_request_t *record = (regina_config_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_config_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_config_sync_request_t * record = (regina_config_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_config_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_config_sync_request_t * record = (regina_config_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_config_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_config_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_config_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_config_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_config_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_config_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_config_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_config_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_C_
# define REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_C_
static void regina_union_soft_sync_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_soft_sync_request_t *record = (regina_soft_sync_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_soft_sync_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_soft_sync_request_t * record = (regina_soft_sync_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_soft_sync_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_soft_sync_request_t * record = (regina_soft_sync_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_soft_sync_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_soft_sync_request_or_null_serialize;
        regina_union->get_size = regina_union_soft_sync_request_or_null_get_size;
        regina_union->destroy = regina_union_soft_sync_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_soft_sync_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_soft_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_soft_sync_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_soft_sync_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_C_ */

# ifndef REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_C_
# define REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_C_
static void regina_union_sync_response_ack_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sync_response_ack_t *record = (regina_sync_response_ack_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_sync_response_ack_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sync_response_ack_t * record = (regina_sync_response_ack_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_sync_response_ack_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sync_response_ack_t * record = (regina_sync_response_ack_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_sync_response_ack_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_sync_response_ack_or_null_serialize;
        regina_union->get_size = regina_union_sync_response_ack_or_null_get_size;
        regina_union->destroy = regina_union_sync_response_ack_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_sync_response_ack_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_sync_response_ack_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_sync_response_ack_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_sync_response_ack_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_C_ */

# ifndef REGINA_UNION_CMD_CONFIRM_OR_NULL_C_
# define REGINA_UNION_CMD_CONFIRM_OR_NULL_C_
static void regina_union_cmd_confirm_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_cmd_confirm_t *record = (regina_cmd_confirm_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_cmd_confirm_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_cmd_confirm_t * record = (regina_cmd_confirm_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_cmd_confirm_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_cmd_confirm_t * record = (regina_cmd_confirm_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_cmd_confirm_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_cmd_confirm_or_null_serialize;
        regina_union->get_size = regina_union_cmd_confirm_or_null_get_size;
        regina_union->destroy = regina_union_cmd_confirm_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_cmd_confirm_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_cmd_confirm_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_cmd_confirm_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_cmd_confirm_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_CMD_CONFIRM_OR_NULL_C_ */

# ifndef REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_C_
# define REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_C_
static void regina_union_notify_confirm_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_confirm_t *record = (regina_notify_confirm_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_notify_confirm_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_confirm_t * record = (regina_notify_confirm_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_notify_confirm_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_confirm_t * record = (regina_notify_confirm_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_notify_confirm_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_notify_confirm_or_null_serialize;
        regina_union->get_size = regina_union_notify_confirm_or_null_get_size;
        regina_union->destroy = regina_union_notify_confirm_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_notify_confirm_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_notify_confirm_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_notify_confirm_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_notify_confirm_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_C_ */

# ifndef REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_C_
# define REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_C_
static void regina_union_thirdparty_service_request_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_thirdparty_service_request_t *record = (regina_thirdparty_service_request_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}

static size_t regina_union_thirdparty_service_request_or_null_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_thirdparty_service_request_t * record = (regina_thirdparty_service_request_t *)regina_union->data;
                union_size += record->get_size(record);
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

static void regina_union_thirdparty_service_request_or_null_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_thirdparty_service_request_t * record = (regina_thirdparty_service_request_t *)regina_union->data;
                record->serialize(writer, record);
            }
            break;
        }
        default:
            break;
        }
    }
}
static regina_union_t *regina_union_thirdparty_service_request_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_thirdparty_service_request_or_null_serialize;
        regina_union->get_size = regina_union_thirdparty_service_request_or_null_get_size;
        regina_union->destroy = regina_union_thirdparty_service_request_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_thirdparty_service_request_or_null_branch_0_create()
{
    regina_union_t *regina_union = regina_union_thirdparty_service_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_thirdparty_service_request_or_null_branch_1_create()
{
    regina_union_t *regina_union = regina_union_thirdparty_service_request_or_null_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_1;
    }
    return regina_union;
}
# endif /* REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_C_ */

static void regina_sync_request_destroy(void *data)
{
    if (data) {
        regina_sync_request_t *record = (regina_sync_request_t *)data;

        if (record->register_sync_request && record->register_sync_request->destroy) {
            record->register_sync_request->destroy(record->register_sync_request);
        }
        if (record->authen_sync_request && record->authen_sync_request->destroy) {
            record->authen_sync_request->destroy(record->authen_sync_request);
        }
        if (record->profile_sync_request && record->profile_sync_request->destroy) {
            record->profile_sync_request->destroy(record->profile_sync_request);
        }
        if (record->notify_sync_request && record->notify_sync_request->destroy) {
            record->notify_sync_request->destroy(record->notify_sync_request);
        }
        if (record->event_sync_request && record->event_sync_request->destroy) {
            record->event_sync_request->destroy(record->event_sync_request);
        }
        if (record->sensor_data_sync_request && record->sensor_data_sync_request->destroy) {
            record->sensor_data_sync_request->destroy(record->sensor_data_sync_request);
        }
        if (record->config_sync_request && record->config_sync_request->destroy) {
            record->config_sync_request->destroy(record->config_sync_request);
        }
        if (record->soft_sync_request && record->soft_sync_request->destroy) {
            record->soft_sync_request->destroy(record->soft_sync_request);
        }
        if (record->sync_response_ack && record->sync_response_ack->destroy) {
            record->sync_response_ack->destroy(record->sync_response_ack);
        }
        if (record->cmd_confirm && record->cmd_confirm->destroy) {
            record->cmd_confirm->destroy(record->cmd_confirm);
        }
        if (record->notify_confirm && record->notify_confirm->destroy) {
            record->notify_confirm->destroy(record->notify_confirm);
        }
        if (record->thirdparty_service_request && record->thirdparty_service_request->destroy) {
            record->thirdparty_service_request->destroy(record->thirdparty_service_request);
        }
        regina_data_destroy(record);
    }
}

static void regina_sync_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_sync_request_t *record = (regina_sync_request_t *)data;

        record->register_sync_request->serialize(writer, record->register_sync_request);
        record->authen_sync_request->serialize(writer, record->authen_sync_request);
        record->profile_sync_request->serialize(writer, record->profile_sync_request);
        record->notify_sync_request->serialize(writer, record->notify_sync_request);
        record->event_sync_request->serialize(writer, record->event_sync_request);
        record->sensor_data_sync_request->serialize(writer, record->sensor_data_sync_request);
        record->config_sync_request->serialize(writer, record->config_sync_request);
        record->soft_sync_request->serialize(writer, record->soft_sync_request);
        record->sync_response_ack->serialize(writer, record->sync_response_ack);
        record->cmd_confirm->serialize(writer, record->cmd_confirm);
        record->notify_confirm->serialize(writer, record->notify_confirm);
        record->thirdparty_service_request->serialize(writer, record->thirdparty_service_request);
    }
}

static size_t regina_sync_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_sync_request_t *record = (regina_sync_request_t *)data;

        record_size += record->register_sync_request->get_size(record->register_sync_request);
        record_size += record->authen_sync_request->get_size(record->authen_sync_request);
        record_size += record->profile_sync_request->get_size(record->profile_sync_request);
        record_size += record->notify_sync_request->get_size(record->notify_sync_request);
        record_size += record->event_sync_request->get_size(record->event_sync_request);
        record_size += record->sensor_data_sync_request->get_size(record->sensor_data_sync_request);
        record_size += record->config_sync_request->get_size(record->config_sync_request);
        record_size += record->soft_sync_request->get_size(record->soft_sync_request);
        record_size += record->sync_response_ack->get_size(record->sync_response_ack);
        record_size += record->cmd_confirm->get_size(record->cmd_confirm);
        record_size += record->notify_confirm->get_size(record->notify_confirm);
        record_size += record->thirdparty_service_request->get_size(record->thirdparty_service_request);

        return record_size;
    }

    return 0;
}

regina_sync_request_t *regina_sync_request_create()
{
    regina_sync_request_t *record = 
            (regina_sync_request_t *)regina_calloc(1, sizeof(regina_sync_request_t));

    if (record) {
        record->serialize = regina_sync_request_serialize;
        record->get_size = regina_sync_request_get_size;
        record->destroy = regina_sync_request_destroy;
    }

    return record;
}

# ifndef REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_register_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_register_sync_response_t *record = (regina_register_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_register_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_register_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_register_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_register_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_register_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_authen_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_authen_sync_response_t *record = (regina_authen_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_authen_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_authen_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_authen_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_authen_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_authen_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_profile_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_profile_sync_response_t *record = (regina_profile_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_profile_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_profile_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_profile_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_profile_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_profile_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_cmd_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_cmd_sync_response_t *record = (regina_cmd_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_cmd_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_cmd_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_cmd_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_cmd_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_cmd_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_notify_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_sync_response_t *record = (regina_notify_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_notify_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_notify_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_notify_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_notify_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_notify_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_event_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_event_sync_response_t *record = (regina_event_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_event_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_event_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_event_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_event_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_event_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_sensor_data_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_sensor_data_sync_response_t *record = (regina_sensor_data_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_sensor_data_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_sensor_data_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_sensor_data_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_sensor_data_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_sensor_data_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_config_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_config_sync_response_t *record = (regina_config_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_config_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_config_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_config_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_config_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_config_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_C_
# define REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_C_
static void regina_union_soft_sync_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_soft_sync_response_t *record = (regina_soft_sync_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_soft_sync_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_soft_sync_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_soft_sync_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_soft_sync_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_soft_sync_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_C_ */

# ifndef REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_C_
# define REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_C_
static void regina_union_cmd_confirm_ack_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_cmd_confirm_ack_t *record = (regina_cmd_confirm_ack_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_cmd_confirm_ack_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_cmd_confirm_ack_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_cmd_confirm_ack_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_cmd_confirm_ack_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_BRANCH_0: {
            regina_union->data = regina_cmd_confirm_ack_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_C_ */

# ifndef REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_C_
# define REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_C_
static void regina_union_notify_confirm_ack_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_notify_confirm_ack_t *record = (regina_notify_confirm_ack_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_notify_confirm_ack_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_notify_confirm_ack_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_notify_confirm_ack_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_notify_confirm_ack_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_BRANCH_0: {
            regina_union->data = regina_notify_confirm_ack_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_C_ */

# ifndef REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_C_
# define REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_C_
static void regina_union_thirdparty_service_response_or_null_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_BRANCH_0:
        {
            if (regina_union->data) {
                regina_thirdparty_service_response_t *record = (regina_thirdparty_service_response_t *)regina_union->data;
                record->destroy(record);
            }
            break;
        }
        default:
            break;
        }

        regina_data_destroy(regina_union);
    }
}
static regina_union_t *regina_union_thirdparty_service_response_or_null_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->destroy = regina_union_thirdparty_service_response_or_null_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_thirdparty_service_response_or_null_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_thirdparty_service_response_or_null_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_BRANCH_0: {
            regina_union->data = regina_thirdparty_service_response_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_C_ */

static void regina_sync_response_destroy(void *data)
{
    if (data) {
        regina_sync_response_t *record = (regina_sync_response_t *)data;

        if (record->register_sync_response && record->register_sync_response->destroy) {
            record->register_sync_response->destroy(record->register_sync_response);
        }
        if (record->authen_sync_response && record->authen_sync_response->destroy) {
            record->authen_sync_response->destroy(record->authen_sync_response);
        }
        if (record->profile_sync_response && record->profile_sync_response->destroy) {
            record->profile_sync_response->destroy(record->profile_sync_response);
        }
        if (record->cmd_sync_response && record->cmd_sync_response->destroy) {
            record->cmd_sync_response->destroy(record->cmd_sync_response);
        }
        if (record->notify_sync_response && record->notify_sync_response->destroy) {
            record->notify_sync_response->destroy(record->notify_sync_response);
        }
        if (record->event_sync_response && record->event_sync_response->destroy) {
            record->event_sync_response->destroy(record->event_sync_response);
        }
        if (record->sensor_data_sync_response && record->sensor_data_sync_response->destroy) {
            record->sensor_data_sync_response->destroy(record->sensor_data_sync_response);
        }
        if (record->config_sync_response && record->config_sync_response->destroy) {
            record->config_sync_response->destroy(record->config_sync_response);
        }
        if (record->soft_sync_response && record->soft_sync_response->destroy) {
            record->soft_sync_response->destroy(record->soft_sync_response);
        }
        if (record->cmd_confirm_ack && record->cmd_confirm_ack->destroy) {
            record->cmd_confirm_ack->destroy(record->cmd_confirm_ack);
        }
        if (record->notify_confirm_ack && record->notify_confirm_ack->destroy) {
            record->notify_confirm_ack->destroy(record->notify_confirm_ack);
        }
        if (record->thirdparty_service_response && record->thirdparty_service_response->destroy) {
            record->thirdparty_service_response->destroy(record->thirdparty_service_response);
        }
        regina_data_destroy(record);
    }
}


regina_sync_response_t *regina_sync_response_deserialize(avro_reader_t reader)
{
    regina_sync_response_t *record = 
            (regina_sync_response_t *)regina_malloc(sizeof(regina_sync_response_t));

    if (record) {

        record->destroy = regina_sync_response_destroy;

        record->register_sync_response = regina_union_register_sync_response_or_null_deserialize(reader);
        record->authen_sync_response = regina_union_authen_sync_response_or_null_deserialize(reader);
        record->profile_sync_response = regina_union_profile_sync_response_or_null_deserialize(reader);
        record->cmd_sync_response = regina_union_cmd_sync_response_or_null_deserialize(reader);
        record->notify_sync_response = regina_union_notify_sync_response_or_null_deserialize(reader);
        record->event_sync_response = regina_union_event_sync_response_or_null_deserialize(reader);
        record->sensor_data_sync_response = regina_union_sensor_data_sync_response_or_null_deserialize(reader);
        record->config_sync_response = regina_union_config_sync_response_or_null_deserialize(reader);
        record->soft_sync_response = regina_union_soft_sync_response_or_null_deserialize(reader);
        record->cmd_confirm_ack = regina_union_cmd_confirm_ack_or_null_deserialize(reader);
        record->notify_confirm_ack = regina_union_notify_confirm_ack_or_null_deserialize(reader);
        record->thirdparty_service_response = regina_union_thirdparty_service_response_or_null_deserialize(reader);
    }

    return record;
}

