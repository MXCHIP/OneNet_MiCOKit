#ifndef __REGINA_COMPRESSION_H__
#define __REGINA_COMPRESSION_H__

#include "std_wrapper.h"

typedef struct compression_codec_t compression_codec_t;
struct regina_stream_t;

int compression_codec_encode(compression_codec_t *codec, const char *data,
                             size_t size, struct regina_stream_t *output);
int compression_codec_decode(compression_codec_t *codec, const char *data,
                             size_t size, struct regina_stream_t *output);

#endif /* __REGINA_COMPRESSION_H__ */
