#include <iostream>
//#include <windows.h>
#include "core/video_capture.h"
#include "core/video_capture_factory.h"
#include "vcm_capturer.h"
#include "test/sdl_player.h"
#include "common/sock_utils.h"

//#include <QApplication>
//#include <QtWidgets/QWidget>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Iphlpapi.lib")
#pragma comment (lib, "Psapi.lib")
#pragma comment (lib, "Userenv.lib")


#pragma execution_character_set("utf-8")
#undef main

int main(int argc, char* argv[])
{
    sockets::Init();
    // 为当前线程初始化COM库并设置并发模式 
    HRESULT hr_retrun = CoInitialize(NULL);// CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr_retrun))
    {
        printf("无法在线程中初始化 COM：: %x\n", hr_retrun);
        return hr_retrun;
    }

    /* 中间处理逻辑... */

    /*std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    if (!info) {
        std::cout << "info error" << std::endl;
    }
    int num_devices = info->NumberOfDevices();
    std::cout << "nums of device: " << num_devices << std::endl;

    for (int i = 0; i < num_devices; ++i)
    {
        char device_name[256];
        char unique_name[256];
        memset(device_name, 0x0, sizeof(device_name));
        memset(unique_name, 0x0, sizeof(unique_name));
        int ret = info->GetDeviceName(i, device_name, sizeof(device_name), unique_name, sizeof(unique_name));
        if (ret == 0)
        {
            std::cout << device_name << " --------- " << unique_name << std::endl;
        }
        else 
        {
            std::cout << "get device name error, idx=" << i << std::endl;
        }
    }*/

    auto player = std::make_shared<SdlPlayer>();

    int width = 1080;
    int height = 720;
    auto capturer = webrtc::test::VcmCapturer::Create(width, height, 30, 0);

    capturer->AddSubscriber(player);

    capturer->StartCapture();

    player->start(capturer->RealWidth(), capturer->RealHeight());

    std::cin.get();
    player->stop();
    // 反初始化
    CoUninitialize();
    sockets::Destroy();
	return 0;
}