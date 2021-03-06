<h1>基于GXCT嵌入式STM32开发板的BadApple个人项目</h1>

![IMG](./img.png)

# 介绍

一年前(2019.4)参加完蓝桥杯后因个人兴趣做的一个通过UART从PC传输BadApple视频至STM32开发板并显示在LCD上的小项目。

项目由三部分构成：

- > [接收数据并显示在LCD的STM32程序（即本Repo）](https://github.com/ChnTamap/BadAppleUART_STM32)
- [PC端进行串行通讯发送数据并与STM32进行同步的上位机Win32控制台程序](https://github.com/ChnTamap/BadApple_UART_Serial)
- [对图像数据进行编码及压缩的Win32控制台程序](https://github.com/ChnTamap/BadApple_UART_BuildData)

# 性能

由于STM32及串行通讯的性能问题，简单的对项目使用了一部分压缩算法，视频帧率大部分时间都是25FPS。同时设计了时间同步协议，当某帧渲染时间过长，会减少后续帧的空闲时间，直到与实际视频的时间同步，整个播放过程中原视频与LCD上的视频达到完美同步。

# 使用

首先项目基于国信长天的CT117E嵌入式开发板（比赛用的XD），IDE使用的是Keil MDK uvision5（本来是GXCT的板子没法用Keil5的，弄了点小操作），链接好Libraries库函数文件夹即可进行编译烧写。
