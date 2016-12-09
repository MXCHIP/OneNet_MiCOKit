# OneNet 设备端SDK使用说明

标签（空格分隔）： 设备端SDK MICO中间件

---

> OneNet 设备端SDK是一个依托于MiCO SDK的中间件，完成和服务器后台的交互，屏蔽底层交互细节，为应用层提供简单好用的API接口，为设备端的快速开发提供保障。

---

## 1. 依赖

基于MiCO SDK: 3.1.0及以后版本 [MiCO SDK下载链接][1]

## 2. 目录结构

目录 | 内容
--- | ---
lib/onenet | 与OneNet后台的交互逻辑 
application/onenet_micokit | 为庆科的MiCOKit开发板定制的应用程序，支持硬件`MiCOKit-3165` `MiCOKit-3166` `MiCOKit-3031`

## 3. MiCOKit示例使用步骤

### 3.1 将OneNet SDK加入MiCoder

 1. 下载、安装MiCoder
 2. 将onenet文件夹复制到MiCO SDK工作目录 `MiCO SDK\libraries\protocols\` 中
 3. 将onenet_micokit文件夹复制到MiCO SDK工作目录 `MiCO SDK\demos\application` 中

### 3.2 在MiCoder编译下载

 1. 编译onenet_micosdk 以MiCOKit-3165为例， 命令 `application.onenet_micokit@MK3165`
 2. 下载运行onenet_micosdk 
 3. 若使用jlink仿真器， 命令 `application.onenet_micokit@MK3165 download run`
 4. 若使用stlink-v2仿真器， 命令 `application.onenet_micokit@MK3165 JTAG=stlink-v2 download run`

### 3.3 操作步骤及效果

请查看文档

## 4. OneNet SDK中间件API
请查看**onenet.h**，在目录`lib/onenet`中

## 5. 基于MiCOKit示例修改

**注意：onenet SDK和数据点是一一对应的，不能混淆**

请查看文档


----------

如有任何问题请与我们联系

上海庆科联系方式：
E-Mail: sales@mxchip.com
联系电话：021-52655026

中移物联联系方式：
E-Mail: zhangxiaobo@iot.chinamobile.com
电话：13883136817
  [1]: http://mico.io/wiki/download
  [2]: http://open.iot.10086.cn
  [3]: http://open.iot.10086.cn/doc/art273.html#70