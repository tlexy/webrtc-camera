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

    auto player = std::make_shared<SdlPlayer>();

    int width = 1080;
    int height = 720;
    auto capturer = webrtc::test::VcmCapturer::Create(width, height, 30, 0);

    capturer->AddSubscriber(player);

    capturer->StartCapture();

    player->start(capturer->RealWidth(), capturer->RealHeight());

    std::cin.get();
    player->stop();
	return 0;
}