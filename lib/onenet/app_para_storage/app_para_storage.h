#ifndef __APP_PARA_STORAGE_H
#define __APP_PARA_STORAGE_H

#include "mico.h"

typedef void *para_handle_t;

/*注册应用程序需要保存的参数
 * 返回0成功， 返回-1失败*/
OSStatus app_para_register( para_handle_t *para, uint32_t para_data_size );

/*获取应用程序
 * 返回参数的大写*/
int app_para_data_size_get( void );

/*获取应用程序保存的参数*/
void *app_para_data_read( para_handle_t *para );

/*保存应用程序参数*/
OSStatus app_para_data_write( para_handle_t *para );

#endif
