#include "regina/regina_common_schema.h"
#include "regina/allocator.h"
#include "regina/encoding.h"
#include "regina/avro_io.h"
#include "common.h"
#include "std_wrapper.h"

size_t avro_long_get_size(int64_t l)
{
    int64_t len = 0;
    uint64_t n = (l << 1) ^ (l >> 63);
    while (n & ~0x7F) {
        len++;
        n >>= 7;
    }
    len++;
    return len;
}

void regina_string_serialize(avro_writer_t writer, void *data)
{
    regina_string_t *str = NULL;

    if (!writer || !data)
        return;

    str = (regina_string_t *)data;
    if (str->data) {
        avro_binary_encoding.write_string(writer, str->data);
    }
}

regina_string_t *regina_string_move_create(const char *data, destroy_fn destroy)
{
    regina_string_t *str = NULL;

    REGINA_RETURN_IF_NIL(data, NULL);

    str = (regina_string_t *)regina_malloc(sizeof(regina_string_t));
    REGINA_RETURN_IF_NIL(str, NULL);

    str->data = (char *)data;
    str->destroy = destroy;

    return str;
}

regina_string_t *regina_string_copy_create(const char *data)
{
    size_t len = 0;
    regina_string_t *str = NULL;

    REGINA_RETURN_IF_NIL(data, NULL);

    str = (regina_string_t *)regina_malloc(sizeof(regina_string_t));
    REGINA_RETURN_IF_NIL(str, NULL);

    len = regina_strlen(data) + 1;
    str->data = (char *)regina_malloc(len * sizeof(char));
    if (!str->data) {
        regina_free(str);
        return NULL;
    }

    regina_memcpy(str->data, data, len);
    str->destroy = &regina_data_destroy;

    return str;
}

void regina_string_destroy(void *data)
{
    regina_string_t *str = NULL;

    if (!data)
        return;

    str = (regina_string_t *)data;
    if (str->data && str->destroy) {
        str->destroy(str->data);
    }
    regina_free(str);
}

regina_string_t *regina_string_deserialize(avro_reader_t reader)
{
    regina_string_t *str =NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    str = (regina_string_t *)regina_malloc(sizeof(regina_string_t));
    REGINA_RETURN_IF_NIL(str, NULL);

    avro_binary_encoding.read_string(reader, &str->data, NULL);
    str->destroy = regina_data_destroy;

    return str;
}

size_t regina_string_get_size(void *data)
{
    regina_string_t *str = NULL;

    REGINA_RETURN_IF_NIL(data, 0);

    str = (regina_string_t *)data;
    if (str->data) {
        size_t len = regina_strlen(str->data);
        return avro_long_get_size(len) + len;
    }
    return 0;
}

regina_bytes_t *regina_bytes_move_create(const uint8_t *data, size_t data_len, destroy_fn destroy)
{
    regina_bytes_t *bytes_array = NULL;

    REGINA_RETURN_IF_NIL2(data, data_len, NULL);

    bytes_array = (regina_bytes_t *)regina_malloc(sizeof(regina_bytes_t));
    REGINA_RETURN_IF_NIL(bytes_array, NULL);

    bytes_array->buffer = (uint8_t *)data;
    bytes_array->size = data_len;
    bytes_array->destroy = destroy;

    return bytes_array;
}

regina_bytes_t *regina_bytes_copy_create(const uint8_t *data, size_t data_len)
{
    regina_bytes_t *bytes_array = NULL;

    REGINA_RETURN_IF_NIL2(data, data_len, NULL);

    bytes_array = (regina_bytes_t *)regina_malloc(sizeof(regina_bytes_t));
    REGINA_RETURN_IF_NIL(bytes_array, NULL);

    bytes_array->buffer = (uint8_t *)regina_malloc(sizeof(uint8_t) * data_len);
    if (!bytes_array->buffer) {
        regina_free(bytes_array);
        return NULL;
    }

    regina_memcpy(bytes_array->buffer, data, data_len);
    bytes_array->size = data_len;
    bytes_array->destroy = &regina_data_destroy;

    return bytes_array;
}

void regina_bytes_destroy(void *data)
{
    regina_bytes_t *bytes =NULL;

    if (!data)
        return;

    bytes = (regina_bytes_t *)data;
    if (bytes->buffer && bytes->destroy) {
        bytes->destroy(bytes->buffer);
    }
    regina_free(bytes);
}

regina_bytes_t *regina_bytes_deserialize(avro_reader_t reader)
{
    int64_t size;
    regina_bytes_t *bytes = NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    bytes = (regina_bytes_t *)regina_malloc(sizeof(regina_bytes_t));
    REGINA_RETURN_IF_NIL(bytes, NULL);

    avro_binary_encoding.read_bytes(reader, (char **)&bytes->buffer, &size);
    bytes->size = size;
    bytes->destroy = regina_data_destroy;

    return bytes;
}

void regina_bytes_serialize(avro_writer_t writer, void *data)
{
    regina_bytes_t *bytes = (regina_bytes_t *)data;
    if (!writer || !bytes || !bytes->buffer || !bytes->size)
        return;

    avro_binary_encoding.write_bytes(writer, (char *)bytes->buffer, bytes->size);
}

size_t regina_bytes_get_size(void *data)
{
    regina_bytes_t *bytes = (regina_bytes_t *)data;
    REGINA_RETURN_IF_NIL3(bytes, bytes->buffer, bytes->size, 0);
    return avro_long_get_size(bytes->size) + bytes->size;

}

regina_bytes_t *regina_fixed_move_create(const uint8_t *data, size_t data_len, destroy_fn destroy)
{
    return regina_bytes_move_create(data, data_len, destroy);
}

regina_bytes_t *regina_fixed_copy_create(const uint8_t *data, size_t data_len)
{
    return regina_bytes_copy_create(data, data_len);
}

void regina_fixed_destroy(void *data)
{
    regina_bytes_destroy(data);
}

void regina_fixed_serialize(avro_writer_t writer, void *data)
{
    regina_bytes_t *bytes = (regina_bytes_t *)data;
    if (!writer || !bytes || !bytes->buffer || !bytes->size)
        return;

    avro_write(writer, (char *)bytes->buffer, bytes->size);
}

regina_bytes_t *regina_fixed_deserialize(avro_reader_t reader, void *context)
{
    regina_bytes_t *bytes = NULL;

    REGINA_RETURN_IF_NIL2(reader, context, NULL);

    bytes = (regina_bytes_t *)regina_malloc(sizeof(regina_bytes_t));
    REGINA_RETURN_IF_NIL(bytes, NULL);
    bytes->buffer = (uint8_t*)regina_malloc((*(size_t *)context) * sizeof(uint8_t));
    REGINA_RETURN_IF_NIL(bytes->buffer, NULL);

    avro_read(reader, (void *)bytes->buffer, (*(size_t *)context));
    bytes->size = (*(size_t *)context);
    bytes->destroy = regina_data_destroy;

    return bytes;
}

size_t regina_fixed_get_size(void *data)
{
    regina_bytes_t *bytes = (regina_bytes_t *)data;
    REGINA_RETURN_IF_NIL3(bytes, bytes->buffer, bytes->size, 0);
    return bytes->size;
}

void regina_boolean_serialize(avro_writer_t writer, void *data)
{
    if (!writer || !data)
        return;
    avro_binary_encoding.write_boolean(writer, *((int8_t *)data));
}

int8_t* regina_boolean_deserialize(avro_reader_t reader)
{
    int8_t* data = NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    data = (int8_t*)regina_malloc(sizeof(int8_t));
    REGINA_RETURN_IF_NIL(data, NULL);
    avro_binary_encoding.read_boolean(reader, data);
    return data;
}

size_t regina_boolean_get_size(void *data)
{
    REGINA_RETURN_IF_NIL(data, 0);
    return avro_long_get_size(*((int8_t *)data));
}

void regina_int_serialize(avro_writer_t writer, void *data)
{
    if (!writer || !data)
        return;
    avro_binary_encoding.write_int(writer, *((int32_t *)data));
}

int32_t *regina_int_deserialize(avro_reader_t reader)
{
    int32_t *data = NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    data = (int32_t *)regina_malloc(sizeof(int32_t));
    REGINA_RETURN_IF_NIL(data, NULL);
    avro_binary_encoding.read_int(reader, data);
    return data;
}

size_t regina_int_get_size(void *data)
{
    REGINA_RETURN_IF_NIL(data, 0);
    return avro_long_get_size(*((int32_t *)data));
}

void regina_long_serialize(avro_writer_t writer, void *data)
{
    if (!writer || !data)
        return;
    avro_binary_encoding.write_long(writer, *((int64_t *)data));
}

int64_t *regina_long_deserialize(avro_reader_t reader)
{
    int64_t *data = NULL;
    REGINA_RETURN_IF_NIL(reader, NULL);

    data = (int64_t *)regina_malloc(sizeof(int64_t));
    REGINA_RETURN_IF_NIL(data, NULL);
    avro_binary_encoding.read_long(reader, data);
    return data;
}

size_t regina_long_get_size(void *data)
{
    REGINA_RETURN_IF_NIL(data, 0);
    return avro_long_get_size(*((int64_t *)data));
}

void regina_enum_serialize(avro_writer_t writer, void *data)
{
    if (!writer || !data)
        return;
    avro_binary_encoding.write_long(writer, *((int *)data));
}

int *regina_enum_deserialize(avro_reader_t reader)
{
    int64_t value;
    int *data = NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    data = (int *)regina_malloc(sizeof(int));
    REGINA_RETURN_IF_NIL(data, NULL);

    avro_binary_encoding.read_long(reader, &value);
    *data = value;

    return data;
}

size_t regina_enum_get_size(void *data)
{
    REGINA_RETURN_IF_NIL(data, 0);
    return avro_long_get_size(*((int *)data));
}

void regina_float_serialize(avro_writer_t writer, void *data)
{
    if (!writer || !data)
        return;
    avro_binary_encoding.write_float(writer, *((float *)data));
}

float *regina_float_deserialize(avro_reader_t reader)
{
    float *data = NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    data = (float *)regina_malloc(sizeof(float));
    REGINA_RETURN_IF_NIL(data, NULL);
    avro_binary_encoding.read_float(reader, data);

    return data;
}

size_t regina_float_get_size(void *data)
{
    REGINA_RETURN_IF_NIL(data, 0);
    return AVRO_FLOAT_SIZE;
}

void regina_double_serialize(avro_writer_t writer, void *data)
{
    if (!writer || !data)
        return;
    avro_binary_encoding.write_double(writer, *((double *)data));
}

double *regina_double_deserialize(avro_reader_t reader)
{
    double* data = NULL;

    REGINA_RETURN_IF_NIL(reader, NULL);

    data = (double *)regina_malloc(sizeof(double));
    REGINA_RETURN_IF_NIL(data, NULL);
    avro_binary_encoding.read_double(reader, data);

    return data;
}

size_t regina_double_get_size(void *data)
{
    REGINA_RETURN_IF_NIL(data, 0);
    return AVRO_DOUBLE_SIZE;
}

void regina_array_serialize(avro_writer_t writer, regina_list_t *array, serialize_fn serialize)
{
    size_t element_count = 0;

    if (!writer)
        return;

    element_count = regina_list_get_size(array);

    if (element_count > 0) {
        avro_binary_encoding.write_long(writer, element_count);
        if (serialize) {
            regina_list_node_t *it = regina_list_begin(array);
            while (it) {
                serialize(writer, regina_list_get_data(it));
                it = regina_list_next(it);
            }
        }
    }

    avro_binary_encoding.write_long(writer, 0);
}

static void *do_deserialize(avro_reader_t reader, deserialize_fn deserialize, void *context)
{
    if (context) {
        return ((deserialize_w_ctx_fn)deserialize)(reader, context);
    }

    return ((deserialize_wo_ctx_fn)deserialize)(reader);
}

static regina_list_t *regina_array_deserialize(avro_reader_t reader, deserialize_fn deserialize, void *context)
{
    int64_t element_count;
    regina_list_t *array = NULL;

    REGINA_RETURN_IF_NIL2(reader, deserialize, NULL);

    array = regina_list_create();
    REGINA_RETURN_IF_NIL(array, NULL);

    avro_binary_encoding.read_long(reader, &element_count);

    while (element_count != 0) {
        if (element_count < 0) {
            int64_t temp;
            element_count *= (-1);
            avro_binary_encoding.read_long(reader, &temp);
        }

        while (element_count-- > 0) {
            regina_list_push_back(array, do_deserialize(reader, deserialize, context));
        }

        avro_binary_encoding.read_long(reader, &element_count);
    }

    return array;
}

regina_list_t *regina_array_deserialize_wo_ctx(avro_reader_t reader, deserialize_wo_ctx_fn deserialize)
{
    REGINA_RETURN_IF_NIL2(reader, deserialize, NULL);
    return regina_array_deserialize(reader, (deserialize_fn)deserialize, NULL);
}

regina_list_t *regina_array_deserialize_w_ctx(avro_reader_t reader, deserialize_w_ctx_fn deserialize, void *context)
{
    REGINA_RETURN_IF_NIL3(reader, deserialize, context, NULL);
    return regina_array_deserialize(reader, (deserialize_fn)deserialize, context);
}

size_t regina_array_get_size(regina_list_t *array, get_size_fn get_size)
{
    size_t array_size = 0;

    REGINA_RETURN_IF_NIL(get_size, 0);

    if (array) {
        regina_list_node_t *it = regina_list_begin(array);
        while (it) {
            array_size += get_size(regina_list_get_data(it));
            it = regina_list_next(it);
        }

        array_size += avro_long_get_size(regina_list_get_size(array));
    }

    array_size += avro_long_get_size(0);

    return array_size;
}

void regina_null_serialize(avro_writer_t writer, void *data)
{

}

void *regina_null_deserialize(avro_reader_t reader)
{
    return NULL;
}

void regina_null_destroy(void *data)
{
}

size_t regina_null_get_size( void )
{
    return AVRO_NULL_SIZE;
}

void regina_data_destroy(void *data)
{
    if (!data)
        return;
    regina_free(data);
}

