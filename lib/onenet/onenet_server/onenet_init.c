#include "app_para_storage.h"
#include "onenet_queue.h"
#include "onenet.h"

para_handle_t onenet_conf;
onenet_context_t *onenet_context = NULL;

OSStatus onenet_context_init( int sensor_buf_size, int event_buf_size )
{
    OSStatus err = kNoErr;

    if( sensor_buf_size == 0 )
        return err = kGeneralErr;

    /* Create application context */
    onenet_context = (onenet_context_t *) calloc( 1, sizeof(onenet_context_t) );
    require_action( onenet_context, exit, err = kNoMemoryErr );

    onenet_context->onenet_status.msg.sensor_buf = calloc( 1, sensor_buf_size);
    require_action( onenet_context->onenet_status.msg.sensor_buf, exit, err = kNoMemoryErr );
    onenet_context->onenet_status.msg.sensor_data_len = sensor_buf_size;

    if( event_buf_size > 0 ){
        onenet_context->onenet_status.msg.event_buf = calloc( 1, event_buf_size);
        require_action( onenet_context->onenet_status.msg.event_buf, exit, err = kNoMemoryErr );
        onenet_context->onenet_status.msg.event_data_len = event_buf_size;
    }else{
        onenet_context->onenet_status.msg.event_buf = NULL;
        onenet_context->onenet_status.msg.event_data_len = 0;
    }

    err = app_para_register(&onenet_conf, sizeof(onenet_config_t));
    require_noerr( err, exit );

    err = onenet_queue_init();
    require_noerr( err, exit );
exit:
    return err;
}

onenet_context_t *onenet_server_context_get( void )
{
    onenet_context->onenet_config = app_para_data_read(&onenet_conf);
    return onenet_context;
}

void onenet_server_para_storage( void )
{
    app_para_data_write(&onenet_conf);
}

void onenet_server_para_default( void )
{
    memset( onenet_context->onenet_config, 0x00, sizeof(onenet_config_t) );
}
