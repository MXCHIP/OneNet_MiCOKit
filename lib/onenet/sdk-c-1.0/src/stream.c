#include "stream.h"
#include "regina/allocator.h"
#include "extent.h"
#include "common.h"
#include "std_wrapper.h"

#define BLOCK_THRESHOLD (1 * 1024)  /* bytes */

void regina_stream_init(regina_stream_t *stream)
{
    regina_memset(stream, 0, sizeof(regina_stream_t));
}

int regina_stream_reserve_next(regina_stream_t *stream, char **buf, size_t size)
{
    size_t used_bytes;

    if ((size > stream->available_bytes) || (0 == size)) {
        size_t block_size = size > BLOCK_THRESHOLD ? size : BLOCK_THRESHOLD;

        regina_stream_block_t *block = regina_malloc(sizeof(regina_stream_block_t) + block_size);
        REGINA_RETURN_IF_NIL(block, REGINA_ERR_NOMEM);

        block->next = NULL;
        block->size = block_size;
        block->block = (char*)block + sizeof(regina_stream_block_t);

        if (stream->last_block) {
            stream->last_block->size -= stream->available_bytes;
            stream->last_block->next = block;
            stream->last_block = block;
        }
        else {
            stream->first_block = block;
            stream->last_block = block;
        }

        stream->available_bytes = block_size;
    }

    used_bytes = stream->last_block->size - stream->available_bytes;
    *buf = stream->last_block->block + used_bytes;
    stream->available_bytes -= size;
    stream->total_bytes += size;
    return REGINA_ERR_NONE;
}

int regina_stream_get_next(regina_stream_t *stream, char **buf, size_t *size)
{
    if (stream->available_bytes) {
        size_t bytes_used = stream->last_block->size - stream->available_bytes;
        *buf = stream->last_block->block + bytes_used;
        *size = stream->available_bytes;
        stream->total_bytes += stream->available_bytes;
        stream->available_bytes = 0;
        return REGINA_ERR_NONE;
    }

    *size = BLOCK_THRESHOLD;
    return regina_stream_reserve_next(stream, buf, *size);
}

int regina_stream_write(regina_stream_t *stream, const char *data, size_t size)
{
    char *buf;
    int err = regina_stream_reserve_next(stream, &buf, size);

    if (REGINA_ERR_NONE != err)
        return err;

    regina_memcpy(buf, data, size);
    return REGINA_ERR_NONE;
}

int regina_stream_write_stream(regina_stream_t *stream, regina_stream_t *data)
{
    while (1) {
        regina_extent_t ext[1];
        size_t count = 1;
        int err;

        regina_stream_extract(data, ext, &count);
        if (0 == count)
            break;

        err = regina_stream_write(stream, ext->base, ext->len);
        REGINA_RETURN_IF_ERR(err);

        regina_stream_erase(data, ext->len);
    }

#if 0 /* it is faster but waste some memory. */
    if (data->total_bytes) {
        if (!stream->first_block) {
            *stream = *data;
        }
        else {
            stream->last_block->next = data->first_block;
            stream->last_block->size -= stream->available_bytes;
            stream->last_block = data->last_block;
            stream->available_bytes = data->available_bytes;
            stream->total_bytes += data->total_bytes;
        }

        regina_stream_init(data);
    }
#endif

    return REGINA_ERR_NONE;
}

static int regina_stream_interal_read(regina_stream_t *stream, char *buf, size_t bytes)
{
    if (stream->total_bytes < bytes)
        return REGINA_ERR_BADPARAM;

    while (stream->first_block) {
        if (bytes > stream->first_block->size) {
            regina_stream_block_t *block = stream->first_block;
            /* assert(stream->first_block != stream->last_block); */

            if (buf) {
                regina_memcpy(buf, block->block, block->size);
                buf += block->size;
            }

            stream->first_block = block->next;
            stream->total_bytes -= block->size;
            bytes -= block->size;
            regina_free(block);
        }
        else {
            if (buf)
                regina_memcpy(buf, stream->first_block->block, bytes);

            stream->total_bytes -= bytes;
            stream->first_block->block += bytes;
            stream->first_block->size -= bytes;

            if (stream->first_block == stream->last_block) {
                /* assert (stream->first_block->size >= stream->available_bytes); */

                if (stream->first_block->size == stream->available_bytes) {
                    /* must free for the pointer of the block could move */
                    regina_free(stream->first_block);
                    stream->first_block = NULL;
                    stream->last_block = NULL;
                    stream->available_bytes = 0;
                    stream->total_bytes = 0;
                    break;
                }
            }

            if (0 == stream->first_block->size) {
                regina_stream_block_t *tmp = stream->first_block;
                stream->first_block = stream->first_block->next;
                regina_free(tmp);
            }

            break;
        }
    }

    return REGINA_ERR_NONE;
}

int regina_stream_erase(regina_stream_t *stream, size_t bytes)
{
    return regina_stream_interal_read(stream, NULL, bytes);
}

int regina_stream_read(regina_stream_t *stream, char *buf, size_t size)
{
    return regina_stream_interal_read(stream, buf, size);
}

int regina_stream_revert(regina_stream_t *stream, size_t size)
{
    size_t used_bytes;

    if (!stream->last_block)
        return REGINA_ERR_BADPARAM;

    used_bytes = stream->last_block->size - stream->available_bytes;
    if (size > used_bytes)
        return REGINA_ERR_BADPARAM;

    stream->available_bytes += size;
    stream->total_bytes -= size;
    return REGINA_ERR_NONE;
}

int regina_stream_extract(regina_stream_t *stream, struct regina_extent_t *extents, size_t *count)
{
    size_t i;
    regina_stream_block_t *cursor;

    cursor = stream->first_block;
    i = 0;
    while (cursor && (i < *count)) {
        extents[i].base = cursor->block;
        extents[i].len = cursor->size;

        ++i;
        cursor = cursor->next;
    }

    *count = i;

    if (!cursor) /* last block need to exclude unused data. */
        extents[i - 1].len -= stream->available_bytes;

    return REGINA_ERR_NONE;
}

int regina_stream_peek(regina_stream_t *stream, char *c, size_t offset)
{
    regina_stream_block_t *cursor;

    if (stream->total_bytes < 1 + offset)
        return REGINA_ERR_READ_FAILED;

    cursor = stream->first_block;
    while (offset > cursor->size) {
        offset -= cursor->size;
        cursor = cursor->next;
    }

    *c = cursor->block[offset];
    return REGINA_ERR_NONE;
}

