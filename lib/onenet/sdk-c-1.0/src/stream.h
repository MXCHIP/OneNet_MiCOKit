#ifndef __REGINA_STREAM_H__
#define __REGINA_STREAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "std_wrapper.h"

struct regina_extent_t;

typedef struct regina_stream_block_t {
    struct regina_stream_block_t *next;
    size_t size;
    char *block;
} regina_stream_block_t;

typedef struct regina_stream_t {
    regina_stream_block_t *first_block;
    regina_stream_block_t *last_block;
    size_t total_bytes;
    size_t available_bytes;
} regina_stream_t;

void regina_stream_init(regina_stream_t *stream);
int regina_stream_erase(regina_stream_t *stream, size_t bytes);
int regina_stream_write(regina_stream_t *stream, const char *data, size_t size);
int regina_stream_write_stream(regina_stream_t *stream, regina_stream_t *data);
int regina_stream_get_next(regina_stream_t *stream, char **buf, size_t *size);
int regina_stream_reserve_next(regina_stream_t *stream, char **buf, size_t size);
int regina_stream_revert(regina_stream_t *stream, size_t size);
int regina_stream_extract(regina_stream_t *stream, struct regina_extent_t *extents, size_t *count);
int regina_stream_peek(regina_stream_t *stream, char *c, size_t offset);
int regina_stream_read(regina_stream_t *stream, char *buf, size_t size);

#define regina_stream_destroy(stream) regina_stream_erase((stream), (stream)->total_bytes)
#define regina_stream_get_size(stream) ((stream)->total_bytes)
#define regina_stream_reset(stream) regina_stream_erase((stream), (stream)->total_bytes)

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_STREAM_H__ */
