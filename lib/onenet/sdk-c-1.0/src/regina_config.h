#ifndef __REGINA_CONFIG_H__
#define __REGINA_CONFIG_H__

#include "std_wrapper.h"
#include "sdk_conf_gen.h"

int regina_config_init(const char *conf, size_t size);
void regina_config_destroy( void );

const char *regina_config_get_sdk_uuid();
int32_t regina_config_get_sensor_data_max_size();
int32_t regina_config_get_sensor_data_min_interval();
int32_t regina_config_get_event_max_count_per_day();
int32_t regina_config_get_event_max_size();

void regina_config_goto_first_bootstrap();

const regina_bootstrap_access_point_t *
regina_config_get_next_bootstrap_server(int32_t protocol);

#endif /* __REGINA_CONFIG_H__ */
