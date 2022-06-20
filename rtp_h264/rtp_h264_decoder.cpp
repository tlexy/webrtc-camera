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
		//如果不是第一个包，那么链表中至少有一个包存在
		if (_last_ts != rtp->hdr.timestamp
			|| rtp->hdr.seq_number < _last_seqno
			|| _fu_list.size() == 0)
		{
			rtp_free(rtp);
			//丢弃所有收到的fua包
			for (auto it = _fu_list.begin(); it != _fu_list.end(); ++it)
			{
				rtp_free(*it);
			}
			_fu_list.clear();
			return nullptr;
		}
		if (rtp->hdr.seq_number > _last_seqno && rtp->hdr.seq_number - _last_seqno == 1)
		{
			_fu_list.push_back(rtp);
		}
		if (is_last_fua(rtp))
		{
			//输出（重新整合）整个NALU包
			auto nalu = assembly_nalu(_fu_list);
			for (auto it = _fu_list.begin(); it != _fu_list.end(); ++it)
			{
				rtp_free(*it);
			}
			_fu_list.clear();
			return nalu;
		}
	}
	return nullptr;
}

bool RtpH264Decoder::is_first_fua(rtp_packet_t* rtp)
{
	if (rtp_len(rtp) <= 2)
	{
		return false;
	}
	FU_INDICATOR* idc = (FU_INDICATOR*)rtp->arr;
	FU_HEADER* hdr = (FU_HEADER*)(rtp->arr + 1);
	if (hdr->S == 1 && hdr->R == 0 && hdr->E == 0)
	{
		return true;
	}
	return false;
}

bool RtpH264Decoder::is_last_fua(rtp_packet_t* rtp)
{
	if (rtp_len(rtp) <= 2)
	{
		return false;
	}
	FU_HEADER* hdr = (FU_HEADER*)(rtp->arr + 1);
	if (hdr->S == 0 && hdr->R == 0 && hdr->E == 1)
	{
		return true;
	}
	return false;
}

NALU* RtpH264Decoder::assembly_nalu(const std::list<rtp_packet_t*>&)
{
	if (_fu_list.size() < 2)
	{
		_fu_list.clear();
		return nullptr;
	}
	int total_len = 1;
	for (auto it = _fu_list.begin(); it != _fu_list.end(); ++it)
	{
		total_len = total_len + (*it)->payload_len;
	}
	NALU* nalu = new NALU;
	if (!nalu)
	{
		return nullptr;
	}
	nalu->payload_len = total_len;
	nalu->payload = (uint8_t*)malloc(nalu->payload_len);
	nalu->start_code[0] = 0x0;
	nalu->start_code[1] = 0x0;
	nalu->start_code[2] = 0x01;
	nalu->start_code[3] = 0x01;
	nalu->_start_code_len = 3;
	nalu->hdr = (NALU_HEADER*)nalu->payload;

	FU_INDICATOR* idc = (FU_INDICATOR*)(nalu->payload + 1);
	FU_HEADER* hdr = (FU_HEADER*)(nalu->payload + 2);
	nalu->hdr->F = 0;
	nalu->hdr->NRI = idc->NRI;
	nalu->hdr->TYPE = hdr->TYPE;
	int off = 1;
	for (auto it = _fu_list.begin(); it != _fu_list.end(); ++it)
	{
		memcpy(nalu->payload + off, (*it)->arr, (*it)->payload_len);
		off += ((*it)->payload_len);
	}
	return nalu;
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