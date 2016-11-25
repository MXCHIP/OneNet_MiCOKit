#include "app_para_storage.h"
#include "mico_app_define.h"

para_handle_t device_conf;
app_context_t *app_context = NULL;

OSStatus app_context_init( void )
{
    OSStatus err = kNoErr;
    /* Create application context */
    app_context = (app_context_t *) calloc( 1, sizeof(app_context_t) );
    require_action( app_context, exit, err = kNoMemoryErr );

    err = app_para_register(&device_conf, sizeof(device_config_t));
    require_noerr( err, exit );
exit:
    return err;
}

app_context_t *app_context_get( void )
{
    app_context->device_config = app_para_data_read(&device_conf);
    return app_context;
}

void app_para_storage( app_context_t *context )
{
    app_para_data_write(&device_conf);
}
