#ifndef __ONENET_QUEUE__H
#define __ONENET_QUEUE__H

#include "mico.h"

OSStatus onenet_queue_init( void );

OSStatus onenet_queue_sensor_push(uint8_t *buf, uint32_t len);

uint32_t onenet_queue_sensor_pop(uint8_t *buf);

OSStatus onenet_queue_event_push(uint8_t *buf, uint32_t len);

uint32_t onenet_queue_event_pop(uint8_t *buf);

void onenet_message_send_sem_set( void );

#endif
