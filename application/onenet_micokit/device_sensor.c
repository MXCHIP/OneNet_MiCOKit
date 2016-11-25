#include "device_sensor.h"

#include "onenet.h"
#include "mico_app_define.h"
#include "micokit_ext.h"
#include "sensor\DHT11\DHT11.h"

#define device_sensor_log(M, ...) custom_log("DS", M, ##__VA_ARGS__)

static bool dc_motor = 0;

void onenet_profile_callback(const char *ieme, char **profile, size_t *size)
{
    avro_writer_t writer;
    p72416_t0_profile_t *prof = p72416_t0_profile_create( );

    prof->mac = regina_string_move_create( ieme, NULL );

    *size = prof->get_size( prof );
    *profile = (char*) malloc( *size );

    writer = avro_writer_memory( *profile, *size );
    prof->serialize( writer, prof );

    prof->destroy( prof );
    avro_writer_free( writer );
}

int onenet_configuration_callback( const char *configuration, size_t size )
{
    avro_reader_t reader;
    p72416_t1_configuration_t *cf;
    app_context_t *app_context = app_context_get( );

    if (!configuration || !size)
        return 0;

    reader = avro_reader_memory(configuration, size);
    cf = p72416_t1_configuration_deserialize(reader);

    app_context->device_config->acquisition_frequency  = cf->acquisition_frequency ;

    app_para_storage( app_context );

    device_sensor_log("configuration: T %d", app_context->device_config->acquisition_frequency );

    cf->destroy(cf);
    avro_reader_free(reader);
    return true;
}

void onenet_notification_callback( void **notification_conf, void **notification, avro_reader_t reader,
                                          int *len )
{
    p72416_t3_notification_t *noti_conf = p72416_t3_notification_create();
    p72416_t3_notification_t *noti = p72416_t3_notification_deserialize(reader);

    float hues = 0;
    float saturation = 0;
    float brightness = 0;


    /*protocol conversion*/
    if( noti->hues->type == P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1 ){
        hues = *(float *)noti->hues->data;
        saturation = *(float *)noti->saturation->data;
        brightness = *(float *)noti->brightness->data;
        if( brightness > 1 ){
            hsb2rgb_led_open(hues, saturation, brightness);
        }else{
            hsb2rgb_led_close();
        }
        device_sensor_log("notification: H %.1f, S %.1f, H %.1f", hues, saturation, brightness);
    }

    if( noti->dc_motor->type == P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1 ){
        dc_motor = *(bool *)noti->dc_motor->data;
        dc_motor_set(dc_motor);
        device_sensor_log("notification: M %d", dc_motor);
    }

    noti_conf->hues = p72416_t3_union_null_or_boolean_branch_0_create( );
    noti_conf->saturation = p72416_t3_union_null_or_boolean_branch_0_create( );
    noti_conf->brightness = p72416_t3_union_null_or_boolean_branch_0_create( );
    noti_conf->dc_motor = p72416_t3_union_null_or_boolean_branch_0_create( );

    *notification_conf = noti_conf;
    *notification = noti;
    *len = noti_conf->get_size(noti_conf);
}

void onenet_notification_destroy( void *writer, void *notification_conf, void *notification )
{
    p72416_t3_notification_t *noti_conf = (p72416_t3_notification_t *)notification_conf;
    p72416_t3_notification_t *noti = (p72416_t3_notification_t *)notification;
    noti_conf->serialize(writer, noti_conf);
    noti_conf->destroy(noti_conf);
    noti->destroy(noti);
}

void onenet_event_callback(void **event, int *len, uint8_t *data, uint32_t datalen )
{
    event_t *evt = (event_t *)data;
    p72416_t2_event_t *ev = p72416_t2_event_create();

    /*protocol conversion*/
    ev->is_sensor = evt->is_sensor;

    *len = ev->get_size(ev);
    *event = ev;
    device_sensor_log("event: F %d", ev->is_sensor);
}

void onenet_event_destroy( avro_writer_t writer, void *event )
{
    if( event == NULL ) return;
    p72416_t2_event_t *ev = (p72416_t2_event_t *)event;
    ev->serialize(writer, ev);
    ev->destroy(ev);
}

void onenet_sensor_data_callback(void **sensor, int *len, uint8_t *data, uint32_t datalen )
{
    sensor_data_t *sensor_data = (sensor_data_t *)data;
    p72416_t4_sensordata_t *sd = p72416_t4_sensordata_create();

    /*protocol conversion*/
    sd->temperature = sensor_data->temperature;
    sd->humidity = sensor_data->humidity;
    sd->infrared = sensor_data->infrared;
    sd->light_sensor = sensor_data->light_sensor;
    sd->dc_motor = dc_motor;

    *len = sd->get_size(sd);
    *sensor = sd;
    device_sensor_log("sensordata: T %d, H %d, I %d, L %d, D %d",
                      sensor_data->temperature,
                      sensor_data->humidity,
                      sensor_data->infrared,
                      sensor_data->light_sensor,
                      dc_motor);
}

void onenet_seneor_data_destroy( avro_writer_t writer, void *sensor )
{
    if( sensor == NULL ) return;
    p72416_t4_sensordata_t *sd = (p72416_t4_sensordata_t *)sensor;
    sd->serialize(writer, sd);
    sd->destroy(sd);
}

void onenet_connect_server_state_callback( bool state )
{
#ifdef USE_MiCOKit_EXT
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};
    memset(oled_show_line, '\0', OLED_DISPLAY_MAX_CHAR_PER_ROW+1);
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", (char *)"OneNet State    ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, (char *)oled_show_line);

    if( state == 1){
        OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, (char *)"  Connected     ");
    }else{
        OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, (char *)"  Disconnected  ");
    }
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)"                ");
#endif
}

static void deviceNotify_WifiStatusHandler(WiFiEvent event, void *arg)
{
#ifdef USE_MiCOKit_EXT
    mico_Context_t *mico_context = mico_system_context_get( );
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};
    memset(oled_show_line, '\0', OLED_DISPLAY_MAX_CHAR_PER_ROW+1);
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", (char *)"WiFi State      ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, (char *)oled_show_line);

    switch( event ){
        case NOTIFY_STATION_UP:
            OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, (char *)"  Connected     ");
            break;
        case NOTIFY_STATION_DOWN:
            OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, (char *)"  Disconnected  ");
            break;
        default:
            break;
    }

    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%16s", mico_context->flashContentInRam.micoSystemConfig.ssid);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)oled_show_line);
#endif
}

int get_firmware_version( char *version )
{
    char wifi_model[13];
    char *pos;
    int len = 0;
    sprintf( wifi_model, "%s", MODEL );
    pos = wifi_model;
    pos += (strlen( pos ) - 4);
    sprintf( version, "ONENET_%s_%s_%s_%s_V%s", MANUFACTURER_NAME, pos, PRODUCT_TYPE, PRODUCT_MODEL,
             APP_VERSION );
    len = strlen( version );
    return len;
}

static void device_oled_init( void )
{
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};
    memset(oled_show_line, '\0', OLED_DISPLAY_MAX_CHAR_PER_ROW+1);

    OLED_Init();
    OLED_Clear();
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", MODEL);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_1, oled_show_line);
    memset(oled_show_line, '\0', OLED_DISPLAY_MAX_CHAR_PER_ROW+1);
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", "OneNet State    ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2,  oled_show_line);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3,  "  Connected     ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4,  "                ");
}

static OSStatus device_sensor_init( int *count )
{
    OSStatus err = kNoErr;

    //初始化外部传感器
    err = DHT11_Init();             //温湿度传感器初始化
    require_noerr_action( err, exit, device_sensor_log("ERROR: Unable to Init DHT11") );

    //红外初始化
    err = infrared_reflective_init();
    require_noerr_action( err, exit, device_sensor_log("ERROR: Unable to Init infrared refective") );

    /*init Light sensor*/
    err = light_sensor_init();
    require_noerr_action( err, exit, device_sensor_log("ERROR: Unable to Init light sensor") );

    //初始化电机
    err = dc_motor_init();
    require_noerr_action( err, exit, device_sensor_log("ERROR: Unable to Init DC motor") );
    dc_motor_set(0);

    //初始化rgb_led
    hsb2rgb_led_init();

    if( *count != 0 ){
        device_oled_init();
    }
    *count = 1;

    device_sensor_log("module init success");

exit:
    return err;
}

static OSStatus device_sensor_data_read( sensor_data_t *sensor_data )
{
    OSStatus err = kNoErr;

    //读取温湿度
    while( 1 ){
        err = DHT11_Read_Data(&(sensor_data->temperature), &(sensor_data->humidity));
        require_noerr_action( err, exit, device_sensor_log("ERROR: Can't DHT11 Read Data") );
        if( sensor_data->temperature != 0 ) break;
        mico_rtos_thread_sleep( 1 );
    }
//        device_sensor_log("DHT11  T: %3.1fC  H: %3.1f%%", (float)sensor_data->temperature, (float)sensor_data->humidity);

    err = infrared_reflective_read( &(sensor_data->infrared) );
    require_noerr_action( err, exit, device_sensor_log("ERROR: Can't infrared refectiver read data") );
//        device_sensor_log("infrared reflective date: %d", ensor_data->infrared);

    err = light_sensor_read( &(sensor_data->light_sensor) );
    require_noerr_action( err, exit, device_sensor_log("ERROR: Can't light sensor read data") );
//        device_sensor_log("light date: %d", upload_sensor_data->light_sensor);
exit:
    return err;
}

static void device_sensor_thread( uint32_t arg )
{
    OSStatus err = kNoErr;
    bool is_sensor = false;
    int count = 0;
    app_context_t *app_context = app_context_get( );
    onenet_context_t *onenet_context = onenet_server_context_get( );
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};

    sensor_data_t *sensor_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));
    require_action( sensor_data, exit, err = kNoMemoryErr );

    event_t *event = (event_t *)malloc(sizeof(event_t));
    require_action( event, exit, err = kNoMemoryErr );

    while(1){
        if( onenet_context->onenet_status.onenet_connect == false ){
            mico_rtos_thread_msleep( 500 );
            continue;
        }

        if( is_sensor == false ){
            if( device_sensor_init( &count ) == kNoErr ){
                is_sensor = true;
                mico_rtos_thread_sleep( 1 );
            }else{
                memset(event, 0x00, sizeof(event_t));
                event->is_sensor = false;
                onenet_message_send(ONENET_EVENT, (uint8_t *)event, sizeof(event_t), SEND_DATA_TIME_OUT);
                is_sensor = false;
                mico_rtos_thread_sleep(app_context->device_config->acquisition_frequency);
            }
        }else{
            memset(sensor_data, 0x00, sizeof(sensor_data_t));
            if( device_sensor_data_read( sensor_data ) == kNoErr){
                snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "T:%3.1fC  H:%3.1f%%", (float)sensor_data->temperature, (float)sensor_data->humidity);
                OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)oled_show_line);
                onenet_message_send(ONENET_SENSOR, (uint8_t *)sensor_data, sizeof(sensor_data_t), SEND_DATA_TIME_OUT);
                mico_rtos_thread_sleep(app_context->device_config->acquisition_frequency);
            }else{
                memset(event, 0x00, sizeof(event_t));
                event->is_sensor = false;
                onenet_message_send(ONENET_EVENT, (uint8_t *)event, sizeof(event_t), SEND_DATA_TIME_OUT);
                is_sensor = false;
            }
        }
    }

exit:
    device_sensor_log("delete device sensor thread Err: %d", err);
    if(sensor_data) free(sensor_data);
    mico_rtos_delete_thread(NULL);
}

void device_oled_update( void )
{
    mico_Context_t *mico_context = mico_system_context_get( );
    char oled_show_line[OLED_DISPLAY_MAX_CHAR_PER_ROW+1] = {'\0'};

    if( mico_context->flashContentInRam.micoSystemConfig.configured != allConfigured )
        return;

#ifdef USE_MiCOKit_EXT
    memset(oled_show_line, '\0', OLED_DISPLAY_MAX_CHAR_PER_ROW+1);
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%s", (char *)"WiFi State      ");
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_2, (char *)oled_show_line);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_3, (char *)"  Connect to ...");
    snprintf(oled_show_line, OLED_DISPLAY_MAX_CHAR_PER_ROW+1, "%16s", mico_context->flashContentInRam.micoSystemConfig.ssid);
    OLED_ShowString(OLED_DISPLAY_COLUMN_START, OLED_DISPLAY_ROW_4, (char *)oled_show_line);

#endif
}

void device_sensor_int( void )
{
    char version[30];

    get_firmware_version( version );
    device_sensor_log("firmware version %s", version);
    mico_system_notify_register(mico_notify_WIFI_STATUS_CHANGED, (void *)deviceNotify_WifiStatusHandler, NULL);
    mico_rtos_create_thread( NULL, MICO_APPLICATION_PRIORITY - 2, "sensor", device_sensor_thread,
                                        DEVICE_SENSOR_THREAD_STACK_SIZE, 0 );
}
