#ifndef __REGINA_SECURITY_H__
#define __REGINA_SECURITY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "std_wrapper.h"

#define MD5_LENGTH 16


typedef struct crypto_codec_t crypto_codec_t;
struct regina_stream_t;

int security_crypto_init();
void security_crypto_deinit();

int security_crypto_rsa_codec_create(crypto_codec_t **codec,
                                     const char *pub_key, size_t pbk_len,
                                     const char *priv_key, size_t pvk_len);
int security_crypto_aes_codec_create(crypto_codec_t **codec, size_t bytes);
int security_crypto_aes_codec_create_ex(crypto_codec_t **codec,
                                        const char *key, size_t bytes);

void security_crypto_codec_destroy(crypto_codec_t *codec);
int security_crypto_codec_encode(crypto_codec_t *codec, const char *input,
                                 size_t size, struct regina_stream_t *output);
int security_crypto_codec_decode(crypto_codec_t *codec, const char *input,
                                 size_t size, struct regina_stream_t *output);
int security_crypto_codec_flush_encoder(crypto_codec_t *codec,
                                        struct regina_stream_t *output);
int security_crypto_codec_flush_decoder(crypto_codec_t *codec,
                                        struct regina_stream_t *output);

size_t security_crypto_codec_get_key(crypto_codec_t *codec, char *key);


void security_md5(const char *input, size_t size, char md[MD5_LENGTH]);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __REGINA_SECURITY_H__ */
