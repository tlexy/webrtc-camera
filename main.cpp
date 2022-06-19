#include <iostream>
#include "core/video_capture.h"
#include "core/video_capture_factory.h"
#include "vcm_capturer.h"
#include "test/sdl_player.h"
#include "test/x264_encoder.h"
#include "rtp_h264/sock_utils.h"

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "ws2_32.lib")

#undef main

int main()
{
    sockets::Init();
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    if (!info) {
        std::cout << "info error" << std::endl;
    }
    int num_devices = info->NumberOfDevices();
    std::cout << "nums of device: " << num_devices << std::endl;

    auto player = std::make_shared<SdlPlayer>();

    int width = 640;
    int height = 480;
    auto capturer = webrtc::test::VcmCapturer::Create(width, height, 30, 0);

    auto x264_encoder = std::make_shared<X264Encoder>();
    x264_encoder->init(width, height, 30);

    capturer->AddSubscriber(player);
    capturer->AddSubscriber(x264_encoder);

    capturer->StartCapture();

    player->start(capturer->RealWidth(), capturer->RealHeight());
    x264_encoder->start();

    std::cin.get();
    player->stop();
    x264_encoder->stop();

    sockets::Destroy();

	return 0;
}