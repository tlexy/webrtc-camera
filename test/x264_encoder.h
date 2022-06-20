#ifndef X264_ENCODER_H
#define X264_ENCODER_H

#include <stdio.h>
#include <SDL.h>
#include <memory>
#include <thread>
#include "common/threadqueue.hpp"
#include "video_frame/video_frame.h"
#include "video_frame_subscriber.h"
#include "../rtp_h264/rtp_h264_encoder.h"
#include "../rtp_h264/rtp_h264_decoder.h"
extern "C"
{
#include "x264.h"
#include "x264_config.h"
}

class FileSaver;

#define SAVEF

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

	void send_rtp(rtp_packet_t*, int fd, const char* ipstr, int port);

	void save_nalu(NALU*);

private:
	std::shared_ptr<std::thread> _th{nullptr};
	ThreadQueue<webrtc::VideoFrame> _qu;
	int _width;
	int _height;
	int _fps;
	bool _is_stop{true};
	x264_t* _handle;
	x264_param_t* _param;

	RtpH264Decoder* _rhd;

#ifdef SAVEF
	FileSaver* _h264_file{nullptr};
#endif
	FileSaver* _rtp_save{nullptr};
};

#endif
