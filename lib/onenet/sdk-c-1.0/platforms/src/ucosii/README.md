#  ʹ��KEIL����UCOSII�汾SDK

### ����ʹ�õ���Ӳ������
* Ӳ�����ã�STM32F407ZG+E2PROM+��̫����
* ���ϵͳ��ucosii+lwip,��ΪSDK����10KB���ϵ�RAM, 256KB���ϵ�ROM

### ��ֲ˵����
* tcp_util����ʹ��lwip�ṩ�ļ���socket api
* �ļ�����ʹ��e2prom����NV�洢��e2prom�Լ���������������û�ʵ��
* �ڴ�������û�ʵ��

### ����˵��
��keil��������Ϊ�����������º궨�壺 
* REGINA_PLATFORM_UCOSII ��ѡ��ѡ��UCOSIIϵͳ
* REGINA_DEBUG ��ѡ���������ʱ�򿪣�������ʱ��ر�
* REGINA_PLATFORM_BIG_ENDIAN ��ѡ�����ƽ̨��С��ģʽ������򿪸ú꣬����Ǵ��
ģʽ����򿪸ú�
* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES ��ѡ��MBEDTLS��Կ�����������ɷ�������
����رոú꣬��ʹ��mbedTLS�ṩ�ķ�����mbedTLS����Ӳ�����ԣ���
����򿪸ú꣬��ʹ��SDK�ṩ�ķ�����Ҳ���������޸�SDKʹ�õ���Կ��ص��������
�ɷ������ο�security_mbedtls.c�е�security_crypto_entropy_source��
* MBEDTLS_PLATFORM_MEMORY ��ѡ��ʹ��SDK�ṩ���ڴ����api������ʹ��ϵͳ�ṩ��
�ڴ����API
