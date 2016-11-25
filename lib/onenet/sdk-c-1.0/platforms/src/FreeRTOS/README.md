#  使用KEIL编译STM32_KYLIN版本SDK

### 代码使用的软硬件配置
* 硬件配置：麒麟座开发板：STM32F103RE+ESP8266串口模块
* 软件系统：裸机,并为SDK保留10KB以上的RAM, 256KB以上的ROM

### 移植说明：
* tcp_util部分使用串口操作ESP8266，详见 dep/network
* 文件部分暂时使用内存存储，如果用户使用flash相关芯片，驱动由用户实现
* 内存管理使用KEIL提供的microlib

### 编译说明
以keil开发环境为例，增加以下宏定义： 
* REGINA_PLATFORM_STM32_KYLIN 必选，麒麟座开发板
* REGINA_DEBUG 可选，建议调试时打开，发布的时候关闭
* REGINA_PLATFORM_BIG_ENDIAN 可选，如果平台是小端模式则无需打开该宏，如果是大端
模式必须打开该宏
* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES 可选，MBEDTLS密钥相关随机数生成方法开关
如果关闭该宏，则使用mbedTLS提供的方法（mbedTLS有软硬依赖性）。
如果打开该宏，则使用SDK提供的方法，也可以自行修改SDK使用的密钥相关的随机数生
成方法，参考security_mbedtls.c中的security_crypto_entropy_source。
* MBEDTLS_PLATFORM_MEMORY 可选，使用SDK提供的内存管理api，否则使用系统提供的
内存管理API

keil宏定义参考：REGINA_PLATFORM_STM32_KYLIN,REGINA_DEBUG,MBEDTLS_PLATFORM_MEMORY,MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES