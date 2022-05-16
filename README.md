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