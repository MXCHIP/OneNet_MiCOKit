#
#  UNPUBLISHED PROPRIETARY SOURCE CODE
#  Copyright (c) 2016 MXCHIP Inc.
#
#  The contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of MXCHIP Corporation.
#

NAME := Lib_OneNet

GLOBAL_DEFINES += REGINA_PLATFORM_MICO
GLOBAL_DEFINES += MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES

$(NAME)_COMPONENTS += protocols.onenet.app_para_storage

GLOBAL_INCLUDES := . \
				   schema_src \
				   sdk-c-1.0/include
				   
$(NAME)_INCLUDES := onenet_server \
					regina_conf \
				    sdk-c-1.0/include \
				    sdk-c-1.0/platforms/include \
				    sdk-c-1.0/platforms/src/mico/dep \
				    sdk-c-1.0/src \
				    sdk-c-1.0/thirdparty/mbedtls-2.2.1/include

$(NAME)_SOURCES := onenet_server/onenet_bonjour.c \
				   onenet_server/onenet_init.c \
				   onenet_server/onenet_server.c \
				   onenet_server/onenet_mico_platform.c \
				   onenet_server/onenet_queue.c \
				   regina_conf/conf.c \
				   schema_src/regina_configuration.c \
				   schema_src/regina_event.c \
				   schema_src/regina_notification.c \
				   schema_src/regina_profile.c \
				   schema_src/regina_sensor_data.c \
				   sdk-c-1.0/platforms/src/mico/dep/nv_dep.c \
				   sdk-c-1.0/platforms/src/mico/allocator.c \
				   sdk-c-1.0/platforms/src/mico/environment.c \
				   sdk-c-1.0/platforms/src/mico/std_wrapper.c \
				   sdk-c-1.0/platforms/src/mico/tcp_util.c \
				   sdk-c-1.0/platforms/src/mico/thread.c \
				   sdk-c-1.0/src/acc_dev_gen.c \
				   sdk-c-1.0/src/acceptor.c \
				   sdk-c-1.0/src/auth_service.c \
				   sdk-c-1.0/src/bootstrap.c \
				   sdk-c-1.0/src/bs_dev_gen.c \
				   sdk-c-1.0/src/channel.c \
				   sdk-c-1.0/src/compression.c \
				   sdk-c-1.0/src/configuration_service.c \
				   sdk-c-1.0/src/device_status_gen.c \
				   sdk-c-1.0/src/device_status.c \
				   sdk-c-1.0/src/device.c \
				   sdk-c-1.0/src/encoding_binary.c \
				   sdk-c-1.0/src/event_service.c \
				   sdk-c-1.0/src/io.c \
				   sdk-c-1.0/src/log.c \
				   sdk-c-1.0/src/notification_service.c \
				   sdk-c-1.0/src/profile_service.c \
				   sdk-c-1.0/src/regina_common_schema.c \
				   sdk-c-1.0/src/regina_config.c \
				   sdk-c-1.0/src/regina_list.c \
				   sdk-c-1.0/src/regina_tcp_protocol.c \
				   sdk-c-1.0/src/safe_ls_file.c \
				   sdk-c-1.0/src/sdk_conf_gen.c \
				   sdk-c-1.0/src/security_mbedtls.c \
				   sdk-c-1.0/src/sensor_data_service.c \
				   sdk-c-1.0/src/soft_service.c \
				   sdk-c-1.0/src/stream.c \
				   sdk-c-1.0/src/thirdparty_service.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/aes.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/aesni.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/arc4.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/asn1parse.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/asn1write.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/base64.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/bignum.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/blowfish.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/camellia.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ccm.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/certs.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/cipher_wrap.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/cipher.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ctr_drbg.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/des.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/dhm.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ecdh.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ecdsa.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ecjpake.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ecp_curves.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ecp.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/entropy_poll.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/entropy.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/error.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/gcm.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/havege.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/hmac_drbg.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/debug.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/platform.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/rsa.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/sha1.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/md_wrap.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/md.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/md2.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/md4.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/md5.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/memory_buffer_alloc.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/oid.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/padlock.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pem.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pk_wrap.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pk.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pkcs11.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pkcs12.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pkcs5.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pkparse.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/pkwrite.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ripemd160.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/sha256.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/sha512.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_cache.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_ciphersuites.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_cli.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_cookie.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_srv.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_ticket.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/ssl_tls.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/threading.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/version_features.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/version.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509_create.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509_crl.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509_crt.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509_csr.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509write_crt.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/x509write_csr.c \
				   sdk-c-1.0/thirdparty/mbedtls-2.2.1/library/xtea.c
				   

				   