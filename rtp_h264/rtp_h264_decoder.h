#ifndef RTP_H264_DECODER_H
#define RTP_H264_DECODER_H

#include "rtp_h264_def.h"

class RtpH264Decoder
{
public:
	RtpH264Decoder();
	
	NALU* decode(const uint8_t* payload, int len);

private:
	NALU* do_decode();

private:
	uint8_t* _buff;
	int _buff_size;
	int _pos;
};


#endif