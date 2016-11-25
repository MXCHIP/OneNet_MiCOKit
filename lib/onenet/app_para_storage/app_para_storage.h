#ifndef __APP_PARA_STORAGE_H
#define __APP_PARA_STORAGE_H

#include "mico.h"

typedef void *para_handle_t;

/*ע��Ӧ�ó�����Ҫ����Ĳ���
 * ����0�ɹ��� ����-1ʧ��*/
OSStatus app_para_register( para_handle_t *para, uint32_t para_data_size );

/*��ȡӦ�ó���
 * ���ز����Ĵ�д*/
int app_para_data_size_get( void );

/*��ȡӦ�ó��򱣴�Ĳ���*/
void *app_para_data_read( para_handle_t *para );

/*����Ӧ�ó������*/
OSStatus app_para_data_write( para_handle_t *para );

#endif
