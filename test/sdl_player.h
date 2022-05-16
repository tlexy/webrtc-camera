#include <stdio.h>
#include <SDL.h>
#include <memory>
#include <thread>
#include "common/threadqueue.hpp"
#include "video_frame/video_frame.h"
#include "video_frame_subscriber.h"

class SdlPlayer : public webrtc::test::VideoFrameSubscriber
{
public:
	void start(int width, int height);
	void stop();

	virtual void OnFrame(const webrtc::VideoFrame&) override;

private:
	void player_thread();

private:
	std::shared_ptr<std::thread> _th{nullptr};
	ThreadQueue<webrtc::VideoFrame> _qu;
	bool _is_stop{ true };
	int _width;
	int _height;
};
