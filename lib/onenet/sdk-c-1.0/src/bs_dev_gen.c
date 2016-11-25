# include "bs_dev_gen.h"
# include "regina/avro_io.h"
# include "regina/encoding.h"
# include "regina/allocator.h"

/*
 * AUTO-GENERATED CODE
 */

# ifndef REGINA_UNION_NULL_OR_BYTES_C_
# define REGINA_UNION_NULL_OR_BYTES_C_
static void regina_union_null_or_bytes_destroy(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_BYTES_BRANCH_1:
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

static size_t regina_union_null_or_bytes_get_size(void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        size_t union_size = avro_long_get_size(regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_BYTES_BRANCH_1:
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

static void regina_union_null_or_bytes_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_union_t *regina_union = (regina_union_t *)data;
        avro_binary_encoding.write_long(writer, regina_union->type);

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_BYTES_BRANCH_1:
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
static regina_union_t *regina_union_null_or_bytes_create()
{
    regina_union_t *regina_union = regina_calloc(1, sizeof(regina_union_t));

    if (regina_union) {
        regina_union->serialize = regina_union_null_or_bytes_serialize;
        regina_union->get_size = regina_union_null_or_bytes_get_size;
        regina_union->destroy = regina_union_null_or_bytes_destroy;
    }

    return regina_union;
}

regina_union_t *regina_union_null_or_bytes_branch_0_create()
{
    regina_union_t *regina_union = regina_union_null_or_bytes_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NULL_OR_BYTES_BRANCH_0;
    }
    return regina_union;
}

regina_union_t *regina_union_null_or_bytes_branch_1_create()
{
    regina_union_t *regina_union = regina_union_null_or_bytes_create();
    if (regina_union) {
        regina_union->type = REGINA_UNION_NULL_OR_BYTES_BRANCH_1;
    }
    return regina_union;
}

regina_union_t *regina_union_null_or_bytes_deserialize(avro_reader_t reader)
{
    regina_union_t *regina_union = regina_union_null_or_bytes_create();

    if (regina_union) {
        int64_t branch;
        avro_binary_encoding.read_long(reader, &branch);
        regina_union->type = branch;

        switch (regina_union->type) {
        case REGINA_UNION_NULL_OR_BYTES_BRANCH_1: {
            regina_union->data = regina_bytes_deserialize(reader);
            break;
        }
        default:
            break;
        }
    }

    return regina_union;
}
# endif /* REGINA_UNION_NULL_OR_BYTES_C_ */

static void regina_acceptor_node_destroy(void *data)
{
    if (data) {
        regina_acceptor_node_t *record = (regina_acceptor_node_t *)data;

        if (record->key && record->key->destroy) {
            record->key->destroy(record->key);
        }
        regina_bytes_destroy(record->token);
        regina_bytes_destroy(record->ip);
        regina_data_destroy(record);
    }
}

static void regina_acceptor_node_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_acceptor_node_t *record = (regina_acceptor_node_t *)data;

        record->key->serialize(writer, record->key);
        regina_bytes_serialize(writer, record->token);
        regina_int_serialize(writer, &record->token_live_time);
        regina_bytes_serialize(writer, record->ip);
        regina_int_serialize(writer, &record->port);
    }
}

static size_t regina_acceptor_node_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_acceptor_node_t *record = (regina_acceptor_node_t *)data;

        record_size += record->key->get_size(record->key);
        record_size += regina_bytes_get_size(record->token);
        record_size += regina_int_get_size(&record->token_live_time);
        record_size += regina_bytes_get_size(record->ip);
        record_size += regina_int_get_size(&record->port);

        return record_size;
    }

    return 0;
}

regina_acceptor_node_t *regina_acceptor_node_create()
{
    regina_acceptor_node_t *record = 
            (regina_acceptor_node_t *)regina_calloc(1, sizeof(regina_acceptor_node_t));

    if (record) {
        record->serialize = regina_acceptor_node_serialize;
        record->get_size = regina_acceptor_node_get_size;
        record->destroy = regina_acceptor_node_destroy;
    }

    return record;
}

regina_acceptor_node_t *regina_acceptor_node_deserialize(avro_reader_t reader)
{
    regina_acceptor_node_t *record = 
            (regina_acceptor_node_t *)regina_malloc(sizeof(regina_acceptor_node_t));

    if (record) {

        record->serialize = regina_acceptor_node_serialize;
        record->get_size = regina_acceptor_node_get_size;
        record->destroy = regina_acceptor_node_destroy;

        record->key = regina_union_null_or_bytes_deserialize(reader);
        record->token = regina_bytes_deserialize(reader);
        avro_binary_encoding.read_int(reader, &record->token_live_time);
        record->ip = regina_bytes_deserialize(reader);
        avro_binary_encoding.read_int(reader, &record->port);
    }

    return record;
}

static void regina_access_request_destroy(void *data)
{
    if (data) {
        regina_access_request_t *record = (regina_access_request_t *)data;

        regina_string_destroy(record->sdk_uuid);
        if (record->session_key && record->session_key->destroy) {
            record->session_key->destroy(record->session_key);
        }
        regina_data_destroy(record);
    }
}

static void regina_access_request_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_access_request_t *record = (regina_access_request_t *)data;

        regina_string_serialize(writer, record->sdk_uuid);
        regina_enum_serialize(writer, &record->mode);
        record->session_key->serialize(writer, record->session_key);
        regina_enum_serialize(writer, &record->acc_session_key_type);
    }
}

static size_t regina_access_request_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_access_request_t *record = (regina_access_request_t *)data;

        record_size += regina_string_get_size(record->sdk_uuid);
        record_size += regina_enum_get_size(&record->mode);
        record_size += record->session_key->get_size(record->session_key);
        record_size += regina_enum_get_size(&record->acc_session_key_type);

        return record_size;
    }

    return 0;
}

regina_access_request_t *regina_access_request_create()
{
    regina_access_request_t *record = 
            (regina_access_request_t *)regina_calloc(1, sizeof(regina_access_request_t));

    if (record) {
        record->serialize = regina_access_request_serialize;
        record->get_size = regina_access_request_get_size;
        record->destroy = regina_access_request_destroy;
    }

    return record;
}

regina_access_request_t *regina_access_request_deserialize(avro_reader_t reader)
{
    regina_access_request_t *record = 
            (regina_access_request_t *)regina_malloc(sizeof(regina_access_request_t));

    if (record) {
        int64_t __regina_enum_value;

        record->serialize = regina_access_request_serialize;
        record->get_size = regina_access_request_get_size;
        record->destroy = regina_access_request_destroy;

        record->sdk_uuid = regina_string_deserialize(reader);
        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->mode = __regina_enum_value;
        record->session_key = regina_union_null_or_bytes_deserialize(reader);
        avro_binary_encoding.read_long(reader, &__regina_enum_value);
        record->acc_session_key_type = __regina_enum_value;
    }

    return record;
}

static void regina_access_response_destroy(void *data)
{
    if (data) {
        regina_access_response_t *record = (regina_access_response_t *)data;

                    regina_list_destroy(record->acc_list, regina_acceptor_node_destroy);
                    regina_data_destroy(record);
    }
}

static void regina_access_response_serialize(avro_writer_t writer, void *data)
{
    if (data) {
        regina_access_response_t *record = (regina_access_response_t *)data;

                    regina_array_serialize(writer, record->acc_list, regina_acceptor_node_serialize);
                }
}

static size_t regina_access_response_get_size(void *data)
{
    if (data) {
        size_t record_size = 0;
        regina_access_response_t *record = (regina_access_response_t *)data;

                    record_size += regina_array_get_size(record->acc_list, regina_acceptor_node_get_size);
            
        return record_size;
    }

    return 0;
}

regina_access_response_t *regina_access_response_create()
{
    regina_access_response_t *record = 
            (regina_access_response_t *)regina_calloc(1, sizeof(regina_access_response_t));

    if (record) {
        record->serialize = regina_access_response_serialize;
        record->get_size = regina_access_response_get_size;
        record->destroy = regina_access_response_destroy;
    }

    return record;
}

regina_access_response_t *regina_access_response_deserialize(avro_reader_t reader)
{
    regina_access_response_t *record = 
            (regina_access_response_t *)regina_malloc(sizeof(regina_access_response_t));

    if (record) {

        record->serialize = regina_access_response_serialize;
        record->get_size = regina_access_response_get_size;
        record->destroy = regina_access_response_destroy;

            record->acc_list = regina_array_deserialize_wo_ctx(reader, (deserialize_wo_ctx_fn)regina_acceptor_node_deserialize);
        }

    return record;
}

