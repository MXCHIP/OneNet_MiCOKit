#include "configuration_service.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "extent.h"
#include "device_status.h"
#include "security.h"
#include "safe_ls_file.h"
#include "std_wrapper.h"


const char* path = "regina_config";

struct configuration_service_t {
    int need_sync;
    int64_t seq_num;
    int64_t ack;
    char *file;
    int config_handled;
    regina_extent_t config;
};

static void configuration_service_clear_config(configuration_service_t *svc)
{
    regina_free(svc->config.base);
    svc->config.base = NULL;
    svc->config.len = 0;
    svc->config_handled = 1;
}

static int configuration_service_do_save(void *context, regina_file_t *file)
{
    regina_extent_t *conf = (regina_extent_t*)context;

    int nsize = (int)conf->len;
    return regina_file_write((char*)&nsize, sizeof(int), 1, file)
        &&
        regina_file_write(conf->base, conf->len, 1, file);
}


static int configuration_service_do_load(void *context, regina_file_t *file)
{
    regina_bytes_t *conf = (regina_bytes_t*)context;
    size_t ntotal = 0;
    int nsize = 0;

    regina_file_seek(file, 0, REGINA_FILE_SEEK_END);
    ntotal = (size_t)regina_file_tell(file);
    regina_file_seek(file, 0, REGINA_FILE_SEEK_SET);
    if (ntotal < sizeof(int)) {
        REGINA_LOG0(WARNING, "The configuration file is corruption, ignore it.");
        return -2;
    }

    if (1 != regina_file_read((char*)&nsize, sizeof(int), 1, file)) {
        return -1;
    }
    if (ntotal - sizeof(int) != nsize) {
        REGINA_LOG0(WARNING, "The configuration file is corruption, ignore it.");
        return -2;
    }
    conf->size = nsize;
    conf->buffer = (uint8_t*)regina_malloc(conf->size);
    if (!conf->buffer) {
        REGINA_LOG0(WARNING, "No memory to store the configuration, ignore it.");
        return -1;
    }

    if (1 != regina_file_read(conf->buffer, conf->size, 1, file)) {
        regina_free(conf->buffer);
        return -1;
    }

    return 1;
}

int configuration_service_create(configuration_service_t **svc)
{
    REGINA_RETURN_IF_NIL(svc, REGINA_ERR_BADPARAM);

    *svc = (configuration_service_t*)regina_malloc(sizeof(configuration_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);

    regina_memset(*svc, 0, sizeof(configuration_service_t));
    (*svc)->need_sync = 1;
    (*svc)->ack = -1;

    if (device_status_get_mode() == DEVICE_MODE_NORMAL) {
        (*svc)->file = regina_strdup(path);
    }
    else {
        const char *reg_code = device_status_get_reg_code();
        size_t len = regina_strlen(path) + regina_strlen(reg_code) + 2;

        (*svc)->file = regina_malloc(len);
        if ((*svc)->file)
            regina_snprintf((*svc)->file, len, "%s-%s", path, reg_code);
    }

    REGINA_RETURN_IF_NIL((*svc)->file, REGINA_ERR_NOMEM);
    return REGINA_ERR_NONE;
}

void configuration_service_destroy(configuration_service_t *svc)
{
    if (svc) {
        configuration_service_clear_config(svc);
        regina_free(svc->file);
        regina_free(svc);
    }
}

int configuration_service_extract_req(configuration_service_t *svc,
                                      regina_union_t **req)
{
    if (!svc->need_sync) {
        *req = regina_union_config_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }
    else {
        regina_config_sync_request_t *conf_req;

        REGINA_LOG_DEBUG0("[CONF SVC] Extracting SYNC request.");

        *req = regina_union_config_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

        conf_req = regina_config_sync_request_create();
        REGINA_RETURN_IF_NIL(conf_req, REGINA_ERR_NOMEM);

        (*req)->data = conf_req;
        conf_req->seq_num = ++ svc->seq_num;
        conf_req->md5 = regina_bytes_move_create(
          (uint8_t*)device_status_get_conf_md5(), MD5_LENGTH, NULL);
        REGINA_RETURN_IF_NIL(conf_req->md5, REGINA_ERR_NOMEM);

        svc->need_sync = 0;
        REGINA_LOG_DEBUG0("[CONF SVC] Complete to extract SYNC request.");
    }

    return REGINA_ERR_NONE;
}

int configuration_service_get_config(configuration_service_t *svc,
                                     char **conf, size_t *size)
{
    regina_bytes_t *context = (regina_bytes_t*)regina_malloc(sizeof(regina_bytes_t));
    regina_memset(context, 0, sizeof(regina_bytes_t));

    if (safe_load_file(svc->file, configuration_service_do_load, context) <= 0) {

        regina_free(context);
        return REGINA_ERR_READ_FAILED;
    }

    *conf = (char*)context->buffer;
    *size = context->size;

    regina_free(context);
    return REGINA_ERR_NONE;
}

int configuration_service_extract_ack(configuration_service_t *svc,
                                      regina_union_t **ack)
{

    if ((svc->ack >= 0) && svc->config_handled) {
        int64_t *id;

        REGINA_LOG_DEBUG1("[CONF SVC] Extracting the configuration ack: %d", svc->ack);

        *ack = regina_union_long_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);

        id = (int64_t*)regina_malloc(sizeof(int64_t));
        REGINA_RETURN_IF_NIL(id, REGINA_ERR_NOMEM);

        (*ack)->data = id;
        *id = svc->ack;
        svc->ack = -1;

        REGINA_LOG_DEBUG0("[CONF SVC] Complete to extract the ack.");
    }
    else {
        *ack = regina_union_long_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);
    }

    return REGINA_ERR_NONE;
}

int configuration_service_handle_sync_resp(configuration_service_t *svc,
                                           regina_union_t *resp)
{
    if ((REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_BRANCH_0 == resp->type) &&
        resp->data) {
        regina_config_sync_response_t *cf_resp;
        cf_resp = (regina_config_sync_response_t*)resp->data;

        REGINA_LOG_DEBUG2("[CONF SVC] Receive configuration sync response, "
                          "local sn=%d, remote sn=%d.",
                          svc->seq_num, cf_resp->seq_num);

        ++svc->seq_num;

        if ((REGINA_UNION_BYTES_OR_NULL_BRANCH_1 ==  cf_resp->config_body->type) ||
            !cf_resp->config_body->data)
            return REGINA_ERR_NONE;

        if (svc->seq_num == cf_resp->seq_num) {
            regina_bytes_t *conf;

            conf = (regina_bytes_t*)cf_resp->config_body->data;
            regina_free(svc->config.base);
            svc->config.base = (char*)regina_malloc(conf->size);
            REGINA_RETURN_IF_NIL(svc->config.base, REGINA_ERR_NOMEM);
            regina_memcpy(svc->config.base, conf->buffer, conf->size);
            svc->config.len = conf->size;

            svc->ack = cf_resp->seq_num;
            svc->config_handled = 0;
        }
    }

    return REGINA_ERR_NONE;
}

int configuration_service_handle_config(configuration_service_t *svc, void *ctx,
                                        configuration_callback cf_callback)
{
    if (svc->config.base && svc->config.len) {
        char md5[MD5_LENGTH];
        int ignore;

        REGINA_LOG_DEBUG0("[CONF SVC] handle configuration by callback handler.");
        ignore = cf_callback(ctx, svc->config.base, svc->config.len);
        if (!ignore)
            safe_save_file(svc->file, configuration_service_do_save, &svc->config);

        security_md5(svc->config.base, svc->config.len, md5);
        device_status_set_conf_md5(md5);

        configuration_service_clear_config(svc);
        return 1;
    }
    return 0;
}

void configuration_service_handle_close(configuration_service_t *svc)
{
    REGINA_LOG_DEBUG0("[CONF SVC] Handle close.");
    svc->need_sync = 1;
    configuration_service_clear_config(svc);
}
