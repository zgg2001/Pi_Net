# Pi_Net
&emsp;&emsp;物联网应用系统综合课设，环境指标监控系统

## 快速了解
&emsp;&emsp;本项目为河南工业大学物联网应用系统综合课设，具体应用为环境指标监控系统，用于环境内温度、湿度、光照、火焰预警等数据的检测与管理。本项目主要采用C/S架构，总体形式为单server端多client端。server端主要进行数据汇总与转发，client端用于数据上传与数据获取。<br>
&emsp;&emsp;client端细分为树莓派端与PC端。**树莓派端**用于接收传感器数据并上传至server端；**PC端**使用QT编写GUI，用于请求接收server端数据并显示。<br>
&emsp;&emsp;server端架设至云服务器，用于接收并保存**树莓派端**的数据并转发至**PC端**显示。server端采用多线程、`epoll`等技术提高整体并发与可用性。C/S之间整体采用自定义应用层协议(传输层协议使用TCP)进行数据传输，来保证整体数据的完整性。<br>
&emsp;&emsp;项目中环境数据的采集使用树莓派搭配若干传感器进行，主要使用的传感器/硬件有：树莓派4B、ssd1306显示屏、dht11温湿度传感器、LM2904分贝传感器。传感器数据主要使用`python`脚本进行获取，并通过`shell`脚本进行汇总与管理。

## 项目目录
* client4PC&emsp;&emsp;&ensp;&ensp;client端 for PC(windows)

* client4PI &emsp;&emsp;&ensp;&ensp;client端 for 树莓派

* hardware4PI&ensp;&emsp;树莓派端传感器数据获取脚本

* public &emsp;&emsp;&emsp;&emsp;公共源码库部分

* server &emsp;&emsp;&emsp;&emsp;server端 for linux


## 作者
zgg2001

## 版权说明
该项目签署了 Apache-2.0 授权许可，详情请参阅 [LICENSE](https://github.com/zgg2001/Pi_Net/blob/main/LICENSE)