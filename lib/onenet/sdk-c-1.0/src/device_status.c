#include "device_status.h"
#include "regina/allocator.h"
#include "regina/avro_io.h"
#include "extent.h"
#include "util.h"
#include "stream.h"
#include "security.h"
#include "regina_config.h"
#include "safe_ls_file.h"
#include "device_status_gen.h"

typedef struct device_status_t {
    char *path;
    crypto_codec_t *codec;

    int new;
    int dirty;

    regina_device_status_t *status_disk;

} device_status_t;

static const char MD5_GUARD[16] = {0};

static device_status_t *sgl_status = NULL;

/**
 * @return non-zero if succeeds.
 */
static int device_status_load_from_memory(device_status_t *status,
                                          const char *data, size_t size)
{
    avro_reader_t reader = avro_reader_memory(data, (int64_t)size);

    if (!reader)
        return 0;

    status->status_disk = regina_device_status_deserialize(reader);
    avro_reader_free(reader);

    REGINA_LOG_DEBUG3("DEVICE STATUS on disk:\n"
                      "                          AuthCode: %s\n"
                      "                          DeviceID: %lu\n"
                      "                          Notification SN: %ld\n",
                      status->status_disk->auth_code->data,
                      status->status_disk->device_id,
                      status->status_disk->notification_sn);

    REGINA_RETURN_IF_NIL(status->status_disk, 0);
    return 1;
}

static int device_status_do_load(void *context, regina_file_t *file)
{
    size_t size;
    int err;
    char *buf;
    regina_stream_t stream[1];

    device_status_t *status = (device_status_t*)context;

    regina_file_seek(file, 0, REGINA_FILE_SEEK_END);
    size = (size_t)regina_file_tell(file);
    regina_file_seek(file, 0, REGINA_FILE_SEEK_SET);

    buf = (char*)regina_malloc(size);
    if (!buf)
        return REGINA_ERR_NOMEM;

    if (1 != regina_file_read(buf, size, 1, file)) {
        regina_free(buf);
        return REGINA_ERR_READ_FAILED;
    }

    regina_stream_init(stream);
    err = security_crypto_codec_decode(status->codec, buf, size, stream);
    if (REGINA_ERR_NONE == err)
        err = security_crypto_codec_flush_decoder(status->codec, stream);

    regina_free(buf);

    if (REGINA_ERR_DATA_CORRUPTION == err)
        return 0;

    if (REGINA_ERR_NONE != err)
        return err;

    size = regina_stream_get_size(stream);

    buf = (char*)regina_malloc(size);
    if (!buf) {
        regina_stream_destroy(stream);
        return REGINA_ERR_NOMEM;
    }

    regina_stream_read(stream, buf, size);
    if (device_status_load_from_memory(status, buf, size))
        err = 1;
    else
        err = REGINA_ERR_DATA_CORRUPTION;

    regina_stream_destroy(stream);
    regina_free(buf);

    return err;
}

static int device_status_create_epoch_status(device_status_t *status,
                                             device_mode_t mode,
                                             const char *reg_code)
{
    char md5_guard[MD5_LENGTH] = {0};

    status->status_disk = regina_device_status_create();
    REGINA_RETURN_IF_NIL(status->status_disk, REGINA_ERR_NOMEM);

    status->new = 1;
    status->status_disk->mode = mode;
    status->status_disk->device_id = ~0LU;

    status->status_disk->reg_code = regina_string_copy_create(reg_code);
    status->status_disk->auth_code = regina_string_copy_create("");
    status->status_disk->profile_md5 = regina_bytes_copy_create(
        (uint8_t*)md5_guard, MD5_LENGTH);
    status->status_disk->conf_md5 = regina_bytes_copy_create(
        (uint8_t*)md5_guard, MD5_LENGTH);
    status->status_disk->notification_sn = 0;

    REGINA_RETURN_IF_NIL4(status->status_disk->reg_code,
                          status->status_disk->auth_code,
                          status->status_disk->profile_md5,
                          status->status_disk->conf_md5,
                          REGINA_ERR_NOMEM);

    return REGINA_ERR_NONE;
}

static int device_status_do_init(device_status_t *status, const char *path,
                                 device_mode_t mode, const char *reg_code)
{
    size_t path_len = regina_strlen(path);
    char key[MD5_LENGTH];
    const char *sdk_uuid = NULL;
    int err;

    if (!reg_code)
        reg_code = "";

    if (DEVICE_MODE_DEBUG == mode) {
        REGINA_RETURN_IF_NIL(reg_code, REGINA_ERR_BADPARAM);
        path_len += regina_strlen(reg_code) + 1; /* 1 byte for '-' */

        status->path = regina_malloc(path_len + 1);
        REGINA_RETURN_IF_NIL(status->path, REGINA_ERR_NOMEM);
        regina_snprintf(status->path, path_len + 1, "%s-%s", path, reg_code);
    }
    else {
        status->path = regina_strdup(path);
        REGINA_RETURN_IF_NIL(status->path, REGINA_ERR_NOMEM);
    }

    sdk_uuid = regina_config_get_sdk_uuid();
    security_md5(sdk_uuid, regina_strlen(sdk_uuid), key);

    security_crypto_aes_codec_create_ex(&status->codec, key, 16);
    REGINA_RETURN_IF_NIL(status->codec, REGINA_ERR_NOMEM);

    err = safe_load_file(status->path, device_status_do_load, status);
    if (err > 0) { /* success */
        regina_device_status_t *dstatus;

        err = REGINA_ERR_NONE;

        if ((regina_strcmp(reg_code, device_status_get_reg_code()) != 0) ||
            (mode != device_status_get_mode())) {
            err = REGINA_ERR_DATA_CORRUPTION;
        }

        dstatus = status->status_disk;
        if (!dstatus->profile_md5 || (MD5_LENGTH != dstatus->profile_md5->size) ||
            !dstatus->conf_md5 || (MD5_LENGTH != dstatus->conf_md5->size)) {
            REGINA_LOG0(FATAL, "The schema of the device status is error.");
            err = REGINA_ERR_DATA_CORRUPTION;
        }
    }
    else if (REGINA_ERR_NOT_FOUND == err) {  /* new device */
        if (status->status_disk) {
            status->status_disk->destroy(status->status_disk);
            status->status_disk = NULL;
        }

        err = REGINA_ERR_NONE;
        err = device_status_create_epoch_status(status, mode, reg_code);
        if (REGINA_ERR_NONE != err) {
            device_status_destroy();
        }
    }
    else if (0 == err) { /* file damaged */
        err = REGINA_ERR_DATA_CORRUPTION;
    }

    if (REGINA_ERR_DATA_CORRUPTION == err) {
        REGINA_LOG0(ERROR, "the device status file may be corruption or not "
                    "belong to current device.");
    }
    return err;
}

int device_status_load(const char *path, device_mode_t mode,
                       const char *reg_code)
{
    int err;

    if (sgl_status)
        return REGINA_ERR_NONE;

    sgl_status = (device_status_t*)regina_malloc(sizeof(device_status_t));
    if (!sgl_status)
        return REGINA_ERR_NOMEM;
    regina_memset(sgl_status, 0, sizeof(device_status_t));

    err = device_status_do_init(sgl_status, path, mode, reg_code);
    if (REGINA_ERR_NONE != err) {
        device_status_destroy();
        return err;
    }

    return REGINA_ERR_NONE;
}

static int device_status_do_save(void *context, regina_file_t *file)
{
    char *buf;
    size_t size;
    avro_writer_t writer;
    int err;
    regina_stream_t stream[1];
    device_status_t *status = (device_status_t*)context;

    size = status->status_disk->get_size(status->status_disk);
    buf = (char*)regina_malloc(size);
    if (!buf)
        return -1;

    writer = avro_writer_memory(buf, size);
    if (!writer) {
        regina_free(buf);
        return -1;
    }

    status->status_disk->serialize(writer, status->status_disk);
    avro_writer_free(writer);

    regina_stream_init(stream);
    err = security_crypto_codec_encode(status->codec, buf, size, stream);
    if (REGINA_ERR_NONE == err)
        err = security_crypto_codec_flush_encoder(status->codec, stream);

    regina_free(buf);

    if (REGINA_ERR_NONE == err) {
        while (1) {
            regina_extent_t extent[1];
            size_t count = 1;
            regina_stream_extract(stream, extent, &count);
            if (!count) {
                err = 1;
                break;
            }

            if (1 != regina_file_write(extent->base, extent->len, 1, file)) {
                err = -1;
                break;
            }
            regina_stream_erase(stream, extent->len);
        }
    }

    regina_stream_destroy(stream);
    return err;
}

int device_status_save()
{
    int err;
    /* assert(sgl_status); */

    if (!sgl_status->dirty)
        return REGINA_ERR_NONE;

    err = safe_save_file(sgl_status->path, device_status_do_save, sgl_status);
    if (err > 0) {
        sgl_status->dirty = 0;
        sgl_status->new = 0;
        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_WRITE_FAILED;
}

int device_status_set_auth_code(const char *auth_code)
{
    regina_string_t *tmp;
    regina_string_t *str_auth_code;

    /* assert(sgl_status); */
    tmp = regina_string_copy_create(auth_code);
    REGINA_RETURN_IF_NIL(tmp, REGINA_ERR_NOMEM);

    str_auth_code = sgl_status->status_disk->auth_code;
    if (str_auth_code->destroy)
        str_auth_code->destroy(str_auth_code);

    sgl_status->status_disk->auth_code = tmp;
    sgl_status->dirty = 1;

    return REGINA_ERR_NONE;
}

const char *device_status_get_auth_code()
{
    /* assert(sgl_status); */
    return sgl_status->status_disk->auth_code->data;
}

int device_status_set_device_id(int64_t id)
{
    sgl_status->status_disk->device_id = id;
    sgl_status->dirty = 1;

    return REGINA_ERR_NONE;
}

void device_status_set_notification_sn(int64_t sn)
{
    /* assert(sgl_status); */
    sgl_status->status_disk->notification_sn = sn;
    sgl_status->dirty = 1;
}

int64_t device_status_get_device_id()
{
    /* assert(sgl_status); */
    return sgl_status->status_disk->device_id;
}

int64_t device_status_get_notification_sn()
{
    /* assert(sgl_status); */
    return sgl_status->status_disk->notification_sn;
}

const char *device_status_get_profile_md5(device_status_t *status)
{
    /* assert(sgl_status); */
    /* assert(MD5_LENGTH == sgl_status->status_disk->profile_md5->size); */

    return (char*)sgl_status->status_disk->profile_md5->buffer;
}

void device_status_set_profile_md5(const char *md5)
{
    /* assert(sgl_status); */
    /* assert(MD5_LENGTH == sgl_status->status_disk->profile_md5->size); */

    regina_memcpy(sgl_status->status_disk->profile_md5->buffer, md5, MD5_LENGTH);
    sgl_status->dirty = 1;
}

void device_status_destroy()
{
    if (sgl_status) {
        if (sgl_status->status_disk)
            sgl_status->status_disk->destroy(sgl_status->status_disk);

        regina_free(sgl_status->path);
        security_crypto_codec_destroy(sgl_status->codec);

        regina_free(sgl_status);
        sgl_status = NULL;
    }
}

void device_status_set_conf_md5(const char *md5)
{
    /* assert(sgl_status); */
    /* assert(MD5_LENGTH == sgl_status->status_disk->conf_md5->size); */

    regina_memcpy(sgl_status->status_disk->conf_md5->buffer, md5, MD5_LENGTH);
    sgl_status->dirty = 1;
}

const char *device_status_get_conf_md5()
{
    /* assert(sgl_status); */
    /* assert(MD5_LENGTH == sgl_status->status_disk->conf_md5->size); */

    return (char*)sgl_status->status_disk->conf_md5->buffer;
}


device_mode_t device_status_get_mode()
{
    /* assert(sgl_status); */

    return (device_mode_t)sgl_status->status_disk->mode;
}

int device_status_is_new()
{
    return sgl_status->new;
}

const char *device_status_get_reg_code()
{
    /* assert(sgl_status); */

    return sgl_status->status_disk->reg_code->data;
}
