
###软件使用的软硬件配置

*硬件配置：展讯SC6500
*软件系统: Threadx

#编译说明

*文件部分采用NV存储
*内存管理由用户实现
*tcp_util部分使用平台兼容的socket api

添加宏定义：

*REGINA_PLATFORM_SPREADTRUM 必选
*REGINA_DEBUG 可选，建议调试时打开，发布的时候关闭

