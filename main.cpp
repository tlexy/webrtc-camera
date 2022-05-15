#include <iostream>
#include "core/video_capture.h"
#include "core/video_capture_factory.h"

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "Winmm.lib")

int main()
{
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    if (!info) {
        std::cout << "info error" << std::endl;
    }
    int num_devices = info->NumberOfDevices();
    std::cout << "nums of device: " << num_devices << std::endl;
	return 0;
}