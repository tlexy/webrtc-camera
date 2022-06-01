#ifndef X264_ENCODER_H
#define X264_ENCODER_H

#include <stdio.h>
#include <SDL.h>
#include <memory>
#include <thread>
#include "common/threadqueue.hpp"
#include "video_frame/video_frame.h"
#include "video_frame_subscriber.h"

class X264Encoder : public webrtc::test::VideoFrameSubscriber
{
public:
	X264Encoder();
	virtual void OnFrame(const webrtc::VideoFrame&) override;

	void start();
	void stop();

	void init(int width, int height, int fps);

private:
	void encode_thread();

private:
	std::shared_ptr<std::thread> _th{nullptr};
	ThreadQueue<webrtc::VideoFrame> _qu;
	int _width;
	int _height;
	int _fps;
	bool _is_stop{true};
};

#endif
