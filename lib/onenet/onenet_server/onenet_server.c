#include "regina/device.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include "regina/error.h"
#include "std_wrapper.h"
#include "conf.h"

#include <stdarg.h>

#include "mico.h"
#include "StringUtils.h"
#include "onenet.h"
#include "onenet_queue.h"
#include "onenet_conf.h"
#include "onenet_bonjour.h"

#define onenet_server_log(M, ...) custom_log("ONENET", M, ##__VA_ARGS__)

#define MAX_SENSOR_DATA_COUNT 1
static bool _wifiConnected = false;
static mico_semaphore_t _wifiConnected_sem = NULL;
static onenet_context_t *onenet_context = NULL;

WEAK void onenet_connect_server_state_callback( bool state )
{

}

WEAK void onenet_ota_server_callback( char *version, char *md5, char *url )
{

}

WEAK void onenet_profile_callback(const char *ieme, char **profile, size_t *size)
{

}

WEAK int onenet_configuration_callback( const char *configuration, size_t size )
{
    return 1;
}

WEAK void onenet_notification_callback( void **notification_conf, void **notification, avro_reader_t reader, int *len )
{

}

WEAK void onenet_notification_destroy( void *writer, void *notification_conf, void *notification )
{

}

WEAK void onenet_event_callback( void **event, int *len, uint8_t *data, uint32_t datalen )
{

}

WEAK void onenet_event_destroy( avro_writer_t writer, void *event )
{

}

WEAK void onenet_sensor_data_callback( void **sensor, int *len, uint8_t *data, uint32_t datalen )
{

}

WEAK void onenet_seneor_data_destroy( avro_writer_t writer, void *sensor )
{

}

void onenetNotify_WifiStatusHandler( int event, void* arg )
{
    (void) arg;
    switch ( event )
    {
        case NOTIFY_STATION_UP:
            _wifiConnected = true;
            mico_rtos_set_semaphore( &_wifiConnected_sem );
            break;
        case NOTIFY_STATION_DOWN:
            _wifiConnected = false;
            break;
        default:
            break;
    }
    return;
}

typedef struct _regina_context_t
{
    regina_sensor_data_t sensor_data_buf[MAX_SENSOR_DATA_COUNT];
    regina_enum_ack_t sensor_data_ack_buf[MAX_SENSOR_DATA_COUNT];

    regina_uplink_data_t data;
    regina_uplink_data_ack_t ack;
} __attribute__ ((aligned (8))) regina_context_t;


static void regina_context_init( regina_context_t *ctx )
{
    memset( ctx, 0, sizeof(regina_context_t) );
    ctx->ack.sensor_data_ack = ctx->sensor_data_ack_buf;
}

static void onenet_retreive_profile( regina_context_t *ctx, const char *ieme, char **profile,
                                     size_t *size )
{
    onenet_profile_callback(ieme, profile, size);
}

static int onenet_handle_configuration( void *ctx, const char *configuration, size_t size )
{
    return onenet_configuration_callback( configuration, size );
}

static void onenet_handle_notification( void *ctx, const regina_notification_id_t *id,
                                        const char *notification,
                                        size_t size )
{
    regina_context_t *context = (regina_context_t*) ctx;
    void *noti_conf = NULL;
    void *noti = NULL;
    int len = 0;
    avro_reader_t reader = avro_reader_memory( notification, size );
    avro_writer_t writer;

    onenet_server_log("The content of the notification(%s)",id->uuid);

    onenet_notification_callback(&noti_conf, &noti, reader, &len);

    free( context->data.ntf_id.uuid );
    context->data.ntf_id.uuid = strdup( id->uuid );
    context->data.ntf_id.internal = id->internal;
    context->data.cf_len = len;

    free( context->data.ntf_confirmation );
    context->data.ntf_confirmation = (char*) malloc( context->data.cf_len );

    writer = avro_writer_memory( context->data.ntf_confirmation, context->data.cf_len );

    onenet_notification_destroy( writer, noti_conf, noti );
    size = len;

    avro_writer_free( writer );
    avro_reader_free( reader );
}

static void onenet_add_event( regina_context_t *ctx, uint8_t *data, uint32_t datalen )
{
    avro_writer_t writer;
    void *ev = NULL;
    int len = 0;

    onenet_event_callback(&ev, &len, data, datalen);

    ctx->data.event.len = len;
    ctx->data.event.event = (char*)malloc(ctx->data.event.len);
    ctx->data.event.level = REGINA_EVENT_LEVEL_WARNING;

    writer = avro_writer_memory(ctx->data.event.event, ctx->data.event.len);

    onenet_event_destroy(writer, ev);
    avro_writer_free(writer);

}

static void onenet_add_sensor_data( regina_context_t *ctx, uint8_t *data, uint32_t datalen )
{
    avro_writer_t writer;
    int idx = 0 ;
    void *sd = NULL;
    int len = 0;

    if ( ctx->data.sd_count >= MAX_SENSOR_DATA_COUNT )
    {
        REGINA_LOG0( ERROR, "failed to add the sensor data for buffer is full." );
        return;
    }

    onenet_sensor_data_callback(&sd, &len, data, datalen );

    idx = ctx->data.sd_count;
    ctx->sensor_data_buf[idx].len = len;
    ctx->sensor_data_buf[idx].data = (char*) malloc( ctx->sensor_data_buf[idx].len );
    writer = avro_writer_memory( ctx->sensor_data_buf[idx].data,
                                 ctx->sensor_data_buf[idx].len );

    onenet_seneor_data_destroy( writer, sd );
    avro_writer_free( writer );

    ctx->data.sensor_data = ctx->sensor_data_buf;
    ++ctx->data.sd_count;
}

static void onenet_handle_software_update( void *ctx, const regina_module_update_info_t *modules,
                                           int count )
{
    int i= 0;
    int j;
    char md5[100];
    char *cursor = md5;

    for ( j = 0; j < 16; ++j )
    {
        unsigned int c = ((unsigned char*) modules[i].md5)[j];

        sprintf( cursor, "%02X", c );
        cursor += 2;
    }

    onenet_ota_server_callback(modules->version, md5, modules->url);
}

static void onenet_clear_uplink_data( regina_uplink_data_t *data,
                                      const regina_uplink_data_ack_t *ack )
{
    size_t i;

    for ( i = 0; i < data->sd_count; ++i )
    {
        if ( REGINA_ACK_SUCCESS != ack->sensor_data_ack[i] )
        {
            onenet_context->onenet_status.upload_state = false;
            onenet_server_log("failed to add sensor data");
        } else
        {
            onenet_context->onenet_status.upload_state = true;
        }

        free( data->sensor_data[i].data );
        data->sensor_data[i].data = NULL;
        data->sensor_data[i].len = 0;
    }
    data->sd_count = 0;

    if ( REGINA_ACK_SUCCESS != ack->event_ack )
    {
        onenet_server_log("failed to send event, ack=%d", ack->event_ack);
    }
    free( data->event.event );
    data->event.event = NULL;
    
    if ( REGINA_ACK_SUCCESS != ack->nt_confirmation_ack )
    {
        onenet_server_log("failed to send the confirmation of the notification(%s), ack=%d",
            data->ntf_id.uuid, ack->nt_confirmation_ack);
    }
    free( data->ntf_confirmation );
    free( data->ntf_id.uuid );
    data->ntf_confirmation = NULL;
    data->ntf_id.uuid = NULL;
}

extern int64_t device_status_get_device_id( );

static void onenet_thread( uint32_t arg )
{
    int err = 0;
    int milliseconds = 0;
    int send_data_len = 0;
    regina_context_t ctx;
    regina_uplink_data_ack_t *ack;
    char *profile = NULL;
    size_t prof_len = 0;
    LinkStatusTypeDef wifi_link;
    mico_Context_t *mico_context = mico_system_context_get( );
    onenet_context = onenet_server_context_get( );

    const regina_module_info_t modules[] = {
        {
            "MXCHIP",
            "1.0.0"
        }
    };

    mico_rtos_init_semaphore( &_wifiConnected_sem, 1 );

    /* Regisist notifications */
    err = mico_system_notify_register( mico_notify_WIFI_STATUS_CHANGED,
                                       (void *) onenetNotify_WifiStatusHandler, NULL );
    require_noerr( err, exit );

    err = micoWlanGetLinkStatus( &wifi_link );
    require_noerr( err, exit );

    if ( wifi_link.is_connected == true )
        _wifiConnected = true;

    regina_context_init( &ctx );
    ack = &ctx.ack;
    onenet_retreive_profile( &ctx, mico_context->micoStatus.mac, &profile, &prof_len );

    while ( 1 )
    {
        if ( onenet_context->onenet_status.onenet_connect == false )
        {
            if ( _wifiConnected == false )
            {
                mico_rtos_get_semaphore( &_wifiConnected_sem, MICO_WAIT_FOREVER );
            }

            err = regina_device_open( regina_conf_get( ), REGINA_CONF_SIZE,
                                      PRODUCT_REGISTER_CODE,
                                      profile, prof_len,
                                      modules,
                                      sizeof(modules) / sizeof(modules[0]),
                                      &ctx.data,
                                      ack );
            onenet_clear_uplink_data( &ctx.data, ack );

            if ( REGINA_ERR_NONE != err )
            {
                onenet_server_log("Failed to connect to the server, error= %d.", err);
                goto ReConnet;
            }

            regina_device_get_device_id( (char *) onenet_context->onenet_config->device_id,
                                         (char *) onenet_context->onenet_config->access_token );
            memset( onenet_context->onenet_config->device_id, 0x00, 21 );
            Int2Str( onenet_context->onenet_config->device_id,
                     (int) device_status_get_device_id( ) );
            onenet_server_para_storage( );

            onenet_server_log("Device id is [%s]", onenet_context->onenet_config->device_id);
            onenet_server_log("Access token is [%s]", onenet_context->onenet_config->access_token);

#if ONENET_USE_BONJOUR
            onenet_bonjour_server_update();
#endif

            onenet_context->onenet_status.onenet_connect = true;
            onenet_connect_server_state_callback( 1 );

            milliseconds = 0;
        } else
        {
            err = regina_device_check_and_keepalive( );
            if ( REGINA_ERR_NONE != err )
            {
                onenet_server_log("Connection is closed, error=%d.", err);
                goto ReConnet;
            }

            err = regina_device_handle_downlink_data( &ctx,
                                                      onenet_handle_notification,
                                                      onenet_handle_configuration,
                                                      onenet_handle_software_update );
            if ( REGINA_ERR_NONE != err )
            {
                onenet_server_log("Failed to handle downlink data, error=%d.", err);
                goto ReConnet;
            }

            if( (send_data_len = onenet_queue_sensor_pop(onenet_context->onenet_status.msg.sensor_buf)) > 0 ) //have data
            {
                onenet_add_sensor_data( &ctx, onenet_context->onenet_status.msg.sensor_buf, send_data_len );
            }

            if( (send_data_len = onenet_queue_event_pop(onenet_context->onenet_status.msg.event_buf)) > 0 ) //have data
            {
                onenet_add_event( &ctx, onenet_context->onenet_status.msg.event_buf, send_data_len );
            }

            err = regina_device_send( &ctx.data, ack );
            if ( REGINA_ERR_NONE != err )
            {
                goto ReConnet;
            }
            onenet_clear_uplink_data( &ctx.data, ack );
            onenet_message_send_sem_set( );

            if ( 0 == (milliseconds % 10) )
            {
                char *resp;
                size_t size;
                regina_enum_ack_t ack;

                err = regina_device_request_thirdparty_service( REGINA_THIRDPARTY_SERVICE_QOS1,
                                                                "req",
                                                                3, &ack, &resp, &size );

                if ( REGINA_ERR_NONE != err )
                {
                    onenet_server_log("Failed to request thirdparty service, error=%d.", err);
                    goto ReConnet;
                }

                regina_free( resp );
            }

            milliseconds++;
        }
        
        continue;
        
        ReConnet:
        if( onenet_context->onenet_status.onenet_connect == true ){
            onenet_context->onenet_status.onenet_connect = false;
            onenet_connect_server_state_callback( 0 );
        }
        regina_device_close( );
        mico_thread_sleep( 2 );
    }

    exit:
    regina_device_close( );
    onenet_server_log("Device stopped.");
    mico_rtos_delete_thread( NULL );
}

OSStatus onenet_server_start( void )
{

#if ONENET_USE_BONJOUR
    onenet_bonjour_server_start();
#endif

    onenet_server_log("OneNet SDK version: %d.%d.%d",
                      ONENET_VER_MAJOR, ONENET_VER_MINOR, ONENET_VER_REVISION);

    return mico_rtos_create_thread( NULL, MICO_APPLICATION_PRIORITY, "ONENET", onenet_thread,
                                    ONENET_THREAD_STACK_SIZE, 0 );
}

