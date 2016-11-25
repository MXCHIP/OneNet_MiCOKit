#	中移物联OneNET C语言SDK

		
### 目录
 * 编译选项
     - CMAKE_BUILD_TYPE
	 - CMAKE_INSTALL_PREFIX
	 - CMAKE_C_COMPILER
     - CMAKE_C_FLAGS
 * 目录结构
 
 
###  编译选项

#### CMAKE_BUILD_TYPE
在执行CMake的时候，定义CMAKE_BUILD_TYPE变量可以设置SDK的编译模式，可取的值为:
Debug和Release，默认值为Release。
如： CMake -DCMAKE_BUILD_TYPE=Debug [sdk-c] 将生成调试版的SDK


#### CMAKE_INSTALL_PREFIX
在执行CMake的时候，定义CMAKE_INSTALL_PREFIX变量可以改变SDK-C的安装目录。
在Linux下的默认路径为 /usr/local。
如： CMake -DCMAKE_INSTALL_PREFIX=/home/xxx/sdk [sdk-c]， SDK将安装到
     /home/xxx/sdk目录下。
	 
	 
#### CMAKE_C_COMPILER
在做交叉编译的时候，可通过设置该选项来指定使用哪个编译器。
如：CMake -DCMAKE_C_COMPILER="mipsel-linux-gcc" [sdk-c]
将用MIPS平台下的gcc编译器编译SDK。
如果该参数未指定，则使用运行CMake的系统中的默认编译器。


#### CMAKE_C_FLAGS
添加编译器所需参数。如： cmake -DCMAKE_C_FLAGS=-Wl,-rpath=./ [sdk-c]

#### MBEDTLS_PLATFORM_MEMORY
编译时定义MBEDTLS_PLATFORM_MEMORY，则SDK使用的mbedtls加密库将使用SDK内建的
regina_calloc和regina_free来分配、释放内存

#### MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
如果平台不支持mbedtls提供的各种 entropy 生成方法，可以通过定义该宏，使用SDK
内建的entropy生成方法

### 目录结构
```
sdk-c
 ├ documents                    SDK相关文档
 ├ include  
 │  └ regina                    API相关头文件  
 ├ platforms                    平台相关的接口定义和实现  
 │  ├ include                   平台相关的接口定义  
 │  └ src                       平台相关的接口实现  
 │     ├ posix                  支持Posix系统的平台相关接口实现  
 │     ├ win                    Windows系统的平台相关接口实现  
 │     ├ FreeRTOS               FreeRTOS系统相关接口实现
 │     ├ spreadtrum             展讯系统相关接口实现
 │     └ ucossii                ucossii平台相关接口实现，参看documents/build_keil_ucosii.md
 ├ src                          sdk实现  
 ├ sample                       示例代码存目录
 └ thirdparty                   sdk依赖的第三方库 
    └ mbedtls-2.2.1             sdk使用的加密库 
```
