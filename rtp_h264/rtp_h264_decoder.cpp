#include "rtp_h264_decoder.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

RtpH264Decoder::RtpH264Decoder()
	:_pos(0)
{
	_buff_size = 1024 * 1024 * 20;
	_buff = new uint8_t[_buff_size];
}

NALU* RtpH264Decoder::decode_rtp(const uint8_t* payload, int len)
{
	if (!rtp_unpack_test((void*)payload, len))
	{
		return nullptr;
	}
	int ptype = rtp_payload_type((void*)payload, len);
	if (ptype != rtp_base::eH264PayLoad)
	{
		return nullptr;
	}
	rtp_packet_t* rtp = rtp_unpack((void*)payload, len);
	NALU_HEADER* hdr = (NALU_HEADER*)rtp->arr;
	if (hdr->TYPE != 28)
	{
		_last_seqno = rtp->hdr.seq_number;
		_last_ts = rtp->hdr.timestamp;
		return decode_single(rtp->arr, rtp->payload_len);
	}
	else
	{
		return decode_fua(rtp);
	}
}

NALU* RtpH264Decoder::decode_fua(rtp_packet_t* rtp)
{
	/*如果和上个包的序号相差为1，正常接收；大于1，那么就要放弃整个帧
	* 将接收的包放到一个链表中，当接收到最后一个时，进行恢复
	*/
	if (is_first_fua(rtp))
	{
		_last_ts = rtp->hdr.timestamp;
		_fu_list.push_back(rtp);
		_last_seqno = rtp->hdr.seq_number;
	}
	else
	{
		//如果不是同一时间或者收到迟到的包
		if (_last_ts != rtp->hdr.timestamp
			|| rtp->hdr.seq_number < _last_seqno)
		{
			//丢弃所有收到的fua包
		}
		if (rtp->hdr.seq_number > _last_seqno && rtp->hdr.seq_number - _last_seqno == 1)
		{
			_fu_list.push_back(rtp);
		}
		if (is_last_fua(rtp))
		{
			//输出（重新整合）整个NALU包
		}
	}
	NALU_HEADER* hdr = (NALU_HEADER*)rtp->arr;
	
}

NALU* RtpH264Decoder::decode_single(const uint8_t* payload, int len)
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
		nalu->payload_len = _pos;
		nalu->payload = (uint8_t*)malloc(nalu->payload_len);
		memcpy(nalu->payload, payload, len);
		nalu->hdr = (NALU_HEADER*)nalu->payload;
		return nalu;
	}
	std::cerr << "decode nalu error." << std::endl;
	return nullptr;
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
		nalu->payload_len = _pos;
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
		//解析与验证是否是一个FU-A NALU？？？？？
		int off = 1;
		uint16_t* nalu_len = 0;
		while (_pos - off >= 2)
		{
			nalu_len = (uint16_t*)_buff[_pos];
			if (*nalu_len > _pos - off - 2)
			{
				return nullptr;
			}
			
		}
	}
	return nalu;
}