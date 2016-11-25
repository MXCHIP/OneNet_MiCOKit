#include "compression.h"
#include "regina/error.h"

int compression_codec_encode(compression_codec_t *codec, const char *data,
                             size_t size, struct regina_stream_t *output)
{
    (void)codec;
    (void)data;
    (void)size;
    (void)output;
    return REGINA_ERR_UNSUPPORTED;
}

int compression_codec_decode(compression_codec_t *codec, const char *data,
                             size_t size, struct regina_stream_t *output)
{
	(void)codec;
	(void)data;
	(void)size;
	(void)output;
    return REGINA_ERR_UNSUPPORTED;
}
