#include "app_para_storage.h"

#define para_log(format, ...)  custom_log("para", format, ##__VA_ARGS__)

typedef struct _app_para_offset_t
{
    uint32_t offset;
    uint32_t size;
} app_para_offset_t;

static int para_count_size = 0;

OSStatus app_para_register( para_handle_t *para, uint32_t para_data_size )
{
    OSStatus err = kNoErr;
    app_para_offset_t *para_offset = (app_para_offset_t *)malloc(sizeof(app_para_offset_t));
    require_action( para_offset, exit, err = kNoMemoryErr );

    para_offset->offset = para_count_size;
    para_offset->size = para_data_size;

    para_count_size += para_data_size;

    *para = para_offset;

exit:
    return err;
}

int app_para_data_size_get( void )
{
    return para_count_size;
}

void *app_para_data_read( para_handle_t *para )
{
    void *addr_sec = NULL;
    app_para_offset_t *para_offset = *para;
    if( para_offset == NULL ){
        return NULL;
    }
    addr_sec = mico_system_context_get_user_data( mico_system_context_get() ) + para_offset->offset;
    mico_system_para_read((void **)&addr_sec, PARA_APP_DATA_SECTION, para_offset->offset, para_offset->size);
    return addr_sec;
}

OSStatus app_para_data_write( para_handle_t *para )
{
    void *addr_sec;
    app_para_offset_t *para_offset = *para;
    if( para_offset == NULL ){
        return kGeneralErr;
     }
    addr_sec = mico_system_context_get_user_data( mico_system_context_get() );
    return mico_system_para_write(addr_sec + para_offset->offset, PARA_APP_DATA_SECTION, para_offset->offset, para_offset->size);
}

