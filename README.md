## windows摄像头采集（webrtc windows camera capture)
### 这个是什么项目
* 从webrtc windows源代码中抠出来的摄像头采集模块
* 只做了少量的修改（绝大多数的修改是为减少webrtc底层库的依赖）

### 使用说明
* 推荐使用Visual Studio 2019 community
* Demo只使用x64配置和编译，x86需要自行配置
* 具体使用见main.cpp
* 持续更新中，目前默认使用第0号摄像头进行采集

### 编译libjpeg-turbo以支持部分设备输出的jpeg格式数据
1. 解压libjpeg-turbo-2.1.3.zip到当前文件夹
2. 进入目录libjpeg-turbo-2.1.3并新建build文件夹
3. 进入build文件夹，打开powershell或者cmd，执行cmake -G "Visual Studio 16 2019" ..
4. 打开生成的libjpeg-turbo.sln，生成整个解决方案
5. 根据自身的路径对windows_capture_test进行配置，当前的配置是使用turbojpeg-static.lib

### Todo List
* 获取设备的名字

### 直播建议
```
高清流建议码率：2000kbps，分辨率：1080p。
标清流建议码率：1000kbps，分辨率：720p。 

压缩比例来说，baseline< main < high， 直播可以考虑用main

码率到底选ABR CBR VBR没有特别定论： 带宽好的可以选择CBR， 带宽一般的选择VBR（可以设置最大 最小波动）， 折中ABR。 同画质而言VBR最省带宽。
```

## 编码部分
1. 添加了libx264进行编码
2. 保存h264 NALU流到本地文件中
3. 将h264 NALU通过rtp（by udp）发送到本地端口12500，使用show.sdp文件进行播放
4. 添加rtp h264解码，接收rtp包后进行解码，将解码后的h264 NALU保存到本地文件中
5. 添加了一个h264码流分析工具（H264BSAnalyzer.exe），以对比本地保存的H264文件与rtp解码后的h264文件的差异

