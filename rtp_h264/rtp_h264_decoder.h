#ifndef RTP_H264_DECODER_H
#define RTP_H264_DECODER_H

#include "rtp_h264_def.h"
#include "rtp_base_common_def.h"
#include "rtp.h"
#include <list>

class RtpH264Decoder
{
public:
	RtpH264Decoder();
	
	//假设rtp包是排序到达的，只是可能存在丢包
	NALU* decode_rtp(const uint8_t* payload, int len);

private:
	NALU* do_decode();
	NALU* decode_single(const uint8_t* payload, int len);
	NALU* decode_fua(rtp_packet_t*);

private:
	uint8_t* _buff;
	int _buff_size;
	int _pos;//指向第一个可写的位置，相当于可用buff长度
	uint16_t _last_seqno{0};
	uint32_t _last_ts{ 0 };
	std::list<rtp_packet_t*> _fu_list;
	int32_t _drop_ts{ 0 };
};


#endif