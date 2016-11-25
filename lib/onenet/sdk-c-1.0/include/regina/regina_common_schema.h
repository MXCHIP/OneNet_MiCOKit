#ifndef REGINA_COMMON_SCHEMA_H_
#define REGINA_COMMON_SCHEMA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "avro_io.h"
#include "regina_list.h"



/*
 * Do not change neither name or value of these constants.
 */
#define AVRO_NULL_SIZE      0
#define AVRO_FLOAT_SIZE     4
#define AVRO_DOUBLE_SIZE    8



typedef void (*serialize_fn)(avro_writer_t writer, void *data);
typedef void *(*deserialize_fn)(void);
typedef void *(*deserialize_wo_ctx_fn)(avro_reader_t reader);
typedef void *(*deserialize_w_ctx_fn)(avro_reader_t reader, void *context);
typedef size_t (*get_size_fn)(void *data);
typedef void (*destroy_fn)(void *data);



typedef struct {
    uint8_t* buffer;
    int32_t  size;

    destroy_fn  destroy;
} regina_bytes_t;

typedef struct {
    char* data;

    destroy_fn  destroy;
} regina_string_t;

typedef struct {
    uint8_t type;
    void   *data;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_union_t;

/**
 * @brief Create a regina_string_t, ONLY copy the pointer of data
 * @return A regina_string_t or NULL if failed.
 */
REGINA_API regina_string_t *regina_string_move_create(const char *data, destroy_fn destroy);

/**
 * @brief Create a regina_string_t and copy data
 * @return A regina_string_t or NULL if failed.
 */
REGINA_API regina_string_t *regina_string_copy_create(const char *data);

/**
 * @brief Destroy a regina_string_t
 */
REGINA_API void regina_string_destroy(void *data);

/**
 * @brief Serialize a regina_string_t
 */
REGINA_API void regina_string_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a regina_string_t
 * @return A regina_string_t or NULL if failed.
 */
REGINA_API regina_string_t *regina_string_deserialize(avro_reader_t reader);

/**
 * @brief Return size of a regina_string_t
 */
REGINA_API size_t regina_string_get_size(void *data);

/**
 * @brief Create a regina_bytes_t object, ONLY copy the pointer of data
 * @return A regina_bytes_t object or NULL if failed.
 */
REGINA_API regina_bytes_t *regina_bytes_move_create(const uint8_t *data, size_t data_len, destroy_fn destroy);

/**
 * @brief Create a regina_bytes_t object and copy data
 * @return A regina_bytes_t object or NULL if failed.
 */
REGINA_API regina_bytes_t *regina_bytes_copy_create(const uint8_t *data, size_t data_len);

/**
 * @brief Destroy a regina_bytes_t object
 */
REGINA_API void regina_bytes_destroy(void *data);

/**
 * @brief Serialize a regina_bytes_t object
 */
REGINA_API void regina_bytes_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a regina_bytes_t object
 * @return A regina_bytes_t object or NULL if failed.
 */
REGINA_API regina_bytes_t *regina_bytes_deserialize(avro_reader_t reader);

/**
 * @brief Return size of a regina_bytes_t object
 */
REGINA_API size_t regina_bytes_get_size(void *data);

/**
 * @brief Create a regina_bytes_t object, ONLY copy the pointer of data
 * @return A regina_bytes_t object or NULL if failed.
 */
REGINA_API regina_bytes_t *regina_fixed_move_create(const uint8_t *data, size_t data_len, destroy_fn destroy);

/**
 * @brief Create a regina_bytes_t object and copy data
 * @return A regina_bytes_t object or NULL if failed.
 */
REGINA_API regina_bytes_t *regina_fixed_copy_create(const uint8_t *data, size_t data_len);

/**
 * @brief Destroy a regina_bytes_t object
 */
REGINA_API void regina_fixed_destroy(void *data);

/**
 * @brief Serialize a regina_bytes_t object
 */
REGINA_API void regina_fixed_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a regina_bytes_t object
 * @return A regina_bytes_t object or NULL if failed.
 */
REGINA_API regina_bytes_t *regina_fixed_deserialize(avro_reader_t reader, void *context);

/**
 * @brief Return size of a regina_bytes_t object
 */
REGINA_API size_t regina_fixed_get_size(void *data);

/**
 * @brief Serialize a boolean
 */
REGINA_API void regina_boolean_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a boolean
 * @return A int8_t pointer or NULL if failed.
 */
REGINA_API int8_t *regina_boolean_deserialize(avro_reader_t reader);

/**
 * @brief Return size of a boolean
 */
REGINA_API size_t regina_boolean_get_size(void *data);

/**
 * @brief Serialize an int
 */
REGINA_API void regina_int_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize an int
 * @return A int32_t pointer or NULL if failed.
 */
REGINA_API int32_t *regina_int_deserialize(avro_reader_t reader);

/**
 * @brief Return size of an int
 */
REGINA_API size_t regina_int_get_size(void *data);

/**
 * @brief Serialize a long
 */
REGINA_API void regina_long_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a long
 * @return A int64_t pointer or NULL if failed.
 */
REGINA_API int64_t *regina_long_deserialize(avro_reader_t reader);

/**
 * @brief Return size of a long
 */
REGINA_API size_t regina_long_get_size(void *data);

/**
 * @brief Serialize an enum
 */
REGINA_API void regina_enum_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize an enum
 * @return A int pointer or NULL if failed.
 */
REGINA_API int *regina_enum_deserialize(avro_reader_t reader);

/**
 * @brief Return size of an enum
 */
REGINA_API size_t regina_enum_get_size(void *data);

/**
 * @brief Serialize a float
 */
REGINA_API void regina_float_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a float
 * @return A float pointer or NULL if failed.
 */
REGINA_API float *regina_float_deserialize(avro_reader_t reader);

/**
 * @brief Return size of a float
 */
REGINA_API size_t regina_float_get_size(void *data);

/**
 * @brief Serialize a double
 */
REGINA_API void regina_double_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a double
 * @return A double pointer or NULL if failed.
 */
REGINA_API double *regina_double_deserialize(avro_reader_t reader);

/**
 * @brief Return size of a double
 */
REGINA_API size_t regina_double_get_size(void *data);

/**
 * @brief Serialize a regina_list_t array and each element
 */
REGINA_API void regina_array_serialize(avro_writer_t writer, regina_list_t *array, serialize_fn serialize);

/**
 * @brief Deserialize a regina_list_t array and each element
 * @return A regina_list_t pointer or NULL if failed.
 */
REGINA_API regina_list_t *regina_array_deserialize_wo_ctx(avro_reader_t reader, deserialize_wo_ctx_fn deserialize);

/**
 * @brief Deserialize a regina_list_t array and each element, use deserialize_context context
 * @return A regina_list_t pointer or NULL if failed.
 */
REGINA_API regina_list_t *regina_array_deserialize_w_ctx(avro_reader_t reader, deserialize_w_ctx_fn deserialize, void *deserialize_context);

/**
 * @brief Return size of a regina_list_t array
 */
REGINA_API size_t regina_array_get_size(regina_list_t *array, get_size_fn get_size);

/**
 * @brief Serialize a null, do nothing
 */
REGINA_API void regina_null_serialize(avro_writer_t writer, void *data);

/**
 * @brief Deserialize a null, do nothing
 * @return NULL.
 */
REGINA_API void *regina_null_deserialize(avro_reader_t reader);

/**
 * @brief Do nothing
 */
REGINA_API void regina_null_destroy(void *data);

/**
 * @brief Return size of a null
 * @return 0.
 */
REGINA_API size_t regina_null_get_size( void );

/**
 * @brief Free memeory of data
 */
REGINA_API void regina_data_destroy(void *data);

/**
 * @brief Return the sizeof avro long
 */
REGINA_API size_t avro_long_get_size(int64_t l);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* REGINA_COMMON_SCHEMA_H_ */
