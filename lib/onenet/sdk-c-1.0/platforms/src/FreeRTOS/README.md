#  ʹ��KEIL����STM32_KYLIN�汾SDK

### ����ʹ�õ���Ӳ������
* Ӳ�����ã������������壺STM32F103RE+ESP8266����ģ��
* ���ϵͳ�����,��ΪSDK����10KB���ϵ�RAM, 256KB���ϵ�ROM

### ��ֲ˵����
* tcp_util����ʹ�ô��ڲ���ESP8266����� dep/network
* �ļ�������ʱʹ���ڴ�洢������û�ʹ��flash���оƬ���������û�ʵ��
* �ڴ����ʹ��KEIL�ṩ��microlib

### ����˵��
��keil��������Ϊ�����������º궨�壺 
* REGINA_PLATFORM_STM32_KYLIN ��ѡ��������������
* REGINA_DEBUG ��ѡ���������ʱ�򿪣�������ʱ��ر�
* REGINA_PLATFORM_BIG_ENDIAN ��ѡ�����ƽ̨��С��ģʽ������򿪸ú꣬����Ǵ��
ģʽ����򿪸ú�
* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES ��ѡ��MBEDTLS��Կ�����������ɷ�������
����رոú꣬��ʹ��mbedTLS�ṩ�ķ�����mbedTLS����Ӳ�����ԣ���
����򿪸ú꣬��ʹ��SDK�ṩ�ķ�����Ҳ���������޸�SDKʹ�õ���Կ��ص��������
�ɷ������ο�security_mbedtls.c�е�security_crypto_entropy_source��
* MBEDTLS_PLATFORM_MEMORY ��ѡ��ʹ��SDK�ṩ���ڴ����api������ʹ��ϵͳ�ṩ��
�ڴ����API

keil�궨��ο���REGINA_PLATFORM_STM32_KYLIN,REGINA_DEBUG,MBEDTLS_PLATFORM_MEMORY,MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES