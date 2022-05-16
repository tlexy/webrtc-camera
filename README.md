## windows摄像头采集（webrtc windows camera capture)
### 这个是什么项目
* 从webrtc windows源代码中抠出来的摄像头采集模块
* 只做了少量的修改（绝大多数的修改是为减少webrtc底层库的依赖）

### 使用说明
* 推荐使用Visual Studio 2019 community
* Demo只使用x64配置和编译，x86需要自行配置
* 具体使用见main.cpp
* 持续更新中，目前默认使用第0号摄像头进行采集

### Todo List
* 高分辨率下部分摄像头会有问题
* 摄像头输出kMJPEG格式时会有问题