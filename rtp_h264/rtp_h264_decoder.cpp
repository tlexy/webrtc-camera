#include "rtp_h264_decoder.h"
#include <string.h>
#include <stdlib.h>

RtpH264Decoder::RtpH264Decoder()
	:_pos(0)
{
	_buff_size = 1024 * 1024 * 20;
	_buff = new uint8_t[_buff_size];
}

NALU* RtpH264Decoder::decode(const uint8_t* payload, int len)
{
	if (len + _pos > _buff_size)
	{
		return nullptr;
	}
	memcpy(_buff + _pos, payload, len);
	_pos += len;
	return do_decode();
}

NALU* RtpH264Decoder::do_decode()
{
	NALU_HEADER* hdr = (NALU_HEADER*)_buff;
	NALU* nalu = new NALU;
	nalu->start_code[0] = 0x0;
	nalu->start_code[1] = 0x0;
	nalu->start_code[2] = 0x01;
	nalu->start_code[3] = 0x01;
	nalu->_start_code_len = 3;
	if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_SPS
		|| (hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_PPS)
	{
		nalu->start_code[2] = 0x00;
		nalu->_start_code_len = 4;
	}

	//假设只有拆分包，没有组合包
	if (hdr->TYPE != 28)
	{
		nalu->payload_len = _pos + 1;
		nalu->payload = (uint8_t*)malloc(nalu->payload_len);
		nalu->hdr = (NALU_HEADER*)nalu->payload;
	}
	else
	{
		if (_pos < 4)
		{
			delete nalu;
			return nullptr;
		}
		//解析与验证是否是一个STAP-A NALU
	}

}