#include "onenet_conf.h"
#include "mico.h"
#include "onenet.h"

static mico_queue_t sensor_queue;
static mico_queue_t event_queue;
static mico_semaphore_t send_sem;

typedef struct _msg
{
    int datelen;
    uint8_t *data_p;
} msg_t;

OSStatus onenet_queue_init( void )
{
    OSStatus err = kNoErr;

    err = mico_rtos_init_queue( &sensor_queue, "sensor queue", sizeof(msg_t), SENSOR_MESSAGES_NUM );
    require_noerr( err, exit );

    err = mico_rtos_init_queue( &event_queue, "event queue", sizeof(msg_t), EVENT_MESSAGES_NUM );
    require_noerr( err, exit );

    err = mico_rtos_init_semaphore(&send_sem, 1);
    require_noerr( err, exit );

exit:
    return err;
}

OSStatus onenet_queue_sensor_push(uint8_t *buf, uint32_t len)
{
    OSStatus err = kNoErr;
    msg_t msg;
    if( mico_rtos_is_queue_full(&sensor_queue) ){
        return ONENET_QUEUE_FULL;
    }

    msg.datelen = len;
    msg.data_p = malloc(msg.datelen + 1);
    require_action(msg.data_p, exit, err = kNoMemoryErr);
    memcpy(msg.data_p, buf, len);

    mico_rtos_push_to_queue(&sensor_queue, &msg, MICO_NO_WAIT);

exit:
    return err;
}

uint32_t onenet_queue_sensor_pop(uint8_t *buf)
{
    OSStatus err = kNoErr;
    msg_t msg;
    uint32_t len = 0;

    err = mico_rtos_pop_from_queue( &sensor_queue, &msg, MICO_NO_WAIT );
    if( err != kNoErr ){
        return len;
    }

    memcpy(buf, msg.data_p, msg.datelen);
    len = msg.datelen;

    if( msg.data_p != NULL ){
        free(msg.data_p);
        msg.data_p = NULL;
    }

    return len;
}

OSStatus onenet_queue_event_push(uint8_t *buf, uint32_t len)
{
    OSStatus err = kNoErr;
    msg_t msg;
    if( mico_rtos_is_queue_full(&event_queue) ){
        return ONENET_QUEUE_FULL;
    }

    msg.datelen = len;
    msg.data_p = malloc(msg.datelen + 1);
    require_action(msg.data_p, exit, err = kNoMemoryErr);
    memcpy(msg.data_p, buf, len);

    mico_rtos_push_to_queue(&event_queue, &msg, MICO_NO_WAIT);

exit:
    return err;
}

uint32_t onenet_queue_event_pop(uint8_t *buf)
{
    OSStatus err = kNoErr;
    msg_t msg;
    uint32_t len = 0;

    if( buf == NULL )
        return len;

    err = mico_rtos_pop_from_queue( &event_queue, &msg, MICO_NO_WAIT );
    if( err != kNoErr ){
        return len;
    }

    memcpy(buf, msg.data_p, msg.datelen);
    len = msg.datelen;

    if( msg.data_p != NULL ){
        free(msg.data_p);
        msg.data_p = NULL;
    }

    return len;
}

void onenet_message_send_sem_set( void )
{
    mico_rtos_set_semaphore(&send_sem);
}

OSStatus onenet_message_send(onenet_msg_type_e msg_type, uint8_t *buf, uint32_t len, uint32_t time_out)
{
    OSStatus err = kNoErr;
    onenet_context_t *onenet_context = onenet_server_context_get();

    if( msg_type == ONENET_SENSOR ){
        if( onenet_context->onenet_status.msg.sensor_data_len < len ) err= kGeneralErr;
        require_noerr( err, exit );
        err = onenet_queue_sensor_push(buf, len);
        require_noerr( err, exit );
    }else if( msg_type == ONENET_EVENT ){
        if( onenet_context->onenet_status.msg.event_data_len < len ) err= kGeneralErr;
        require_noerr( err, exit );
        err = onenet_queue_event_push(buf, len);
        require_noerr( err, exit );
    }

    err = mico_rtos_get_semaphore(&send_sem, time_out);
    require_noerr( err, exit );

    if( onenet_context->onenet_status.upload_state == true ){
        err = kNoErr;
    }else{
        err = kGeneralErr;
    }

exit:
    return err;
}
