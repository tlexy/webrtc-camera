#include <iostream>
#include "core/video_capture.h"
#include "core/video_capture_factory.h"
#include "vcm_capturer.h"
#include "test/sdl_player.h"

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Winmm.lib")

#undef main

int main()
{
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
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
    }

    /*auto player = std::make_shared<SdlPlayer>();

    int width = 1080;
    int height = 720;
    auto capturer = webrtc::test::VcmCapturer::Create(width, height, 30, 2);

    capturer->AddSubscriber(player);

    capturer->StartCapture();

    player->start(capturer->RealWidth(), capturer->RealHeight());*/

    std::cin.get();
    //player->stop();
	return 0;
}