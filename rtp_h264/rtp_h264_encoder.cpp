#include "rtp_h264_encoder.h"
#include <stdlib.h>
#include "byte_order.hpp"
#include <iostream>

void RtpH264Encoder::init(rtp_parameter_t param, rtp_session_t sess, int ts_step)
{
	_param = param;
	_sess = sess;
	_ts_step = ts_step;
}

bool RtpH264Encoder::encode(const char* Nalu, int len)
{
	NALU_UNIT* nalu = new NALU_UNIT;
	if (!nalu)
	{
		return false;
	}
	nalu->payload = (char*)malloc(len);
	if (!nalu->payload)
	{
		return false;
	}
	memcpy(nalu->payload, Nalu, len);
	nalu->len = len;
	_unpack_list.push_back(nalu);
	_unpack_size += len;
	return true;
}

int RtpH264Encoder::get_packet(rtp_packet_t*& rtp)
{
	if (!_pack_rtp.empty())
	{
		rtp = _pack_rtp.front();
		_pack_rtp.pop_front();
		return _unpack_list.size() + _pack_rtp.size();
	}
	if (_unpack_list.empty())
	{
		return -1;
	}
	auto nalu = _unpack_list.front();
	NALU_HEADER* hdr = (NALU_HEADER*)nalu->payload;
	if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_SPS)
	{
		/*if (_unpack_list.size() < 2)
		{
			return -1;
		}
		else
		{
			pack(rtp, 2);
			return _unpack_list.size();
		}*/
		std::cout << "SPS PACK" << std::endl;
		rtp = pack_single(nalu->payload, nalu->len, false);
		_unpack_list.pop_front();
		return _unpack_list.size();
	}
	else if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_PPS)
	{
		std::cout << "PPS PACK" << std::endl;
		rtp = pack_single(nalu->payload, nalu->len, false);
		_unpack_list.pop_front();
		return _unpack_list.size();
	}
	else
	{
		pack(rtp);
		return (_unpack_list.size() + _pack_rtp.size());
	}
}

void RtpH264Encoder::pack(rtp_packet_t*& rtp, int count)
{
	//将SPP/PPS打包为一个RTP包
	if (_unpack_list.size() < count)
	{
		return;
	}
	int packet_size = 0;
	int num = 0;
	std::list<NALU_UNIT*> temp_list;
	for (auto it = _unpack_list.begin(); it != _unpack_list.end() && num < count; ++it)
	{
		packet_size += (*it)->len;
		++num;
		temp_list.push_back(*it);
	}
	
	char* buff = (char*)malloc(1 + 2 * count + packet_size);
	NALU_HEADER* stap_hdr = (NALU_HEADER*)buff;
	stap_hdr->TYPE = 24;
	stap_hdr->F = 0;
	stap_hdr->NRI = 0;

	int off = 1;
	num = 0;
	for (auto it = temp_list.begin(); it != temp_list.end(); ++it)
	{
		int* size = (int*)(buff + off);
		*size = sockets::hostToNetwork16((*it)->len);
		off += 2;
		memcpy(buff + off, (*it)->payload, (*it)->len);
		off += (*it)->len;

		_unpack_list.pop_front();
	}
	rtp = pack_single(buff, 1 + 2 * count + packet_size, false);
}

void RtpH264Encoder::pack(rtp_packet_t*& rtp)
{
	auto nalu = _unpack_list.front();
	_unpack_list.pop_front();
	NALU_HEADER* hdr = (NALU_HEADER*)nalu->payload;
	bool time_inc = true;
	if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_SEI)
	{
		time_inc = false;
	}
	/*else if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_IDR)
	{
		time_inc = false;
	}*/
	int count = nalu->len / MAX_NALU_LEN;
	int rem = nalu->len % MAX_NALU_LEN;
	if (count * MAX_NALU_LEN < nalu->len)
	{
		count += 1;
	}
	if (count == 1 || (count == 1 && rem == 1))
	{
		rtp = pack_single(nalu->payload, nalu->len, time_inc);
	}
	else
	{
		std::cout << "PACK MUTLI" << std::endl;
		FU_INDICATOR fuidc;
		fuidc.F = 0;
		fuidc.NRI = hdr->NRI;
		fuidc.TYPE = 28;

		FU_HEADER fuhdr;
		fuhdr.TYPE = hdr->TYPE;
		fuhdr.R = 0;
		//拆分为多个rtp包
		int header_off = 1;
		for (int i = 1; i <= count; ++i)
		{
			if (i == 1)
			{
				fuhdr.E = 0;
				fuhdr.S = 1;
			}
			else if (i > 1 && i < count)
			{
				fuhdr.E = 0;
				fuhdr.S = 0;
			}
			else
			{
				fuhdr.E = 1;
				fuhdr.S = 0;
			}
			if (nalu->len == 1361)
			{
				int a = 1;
			}
			std::cout << "nalu len: " << nalu->len << std::endl;
			int fua_len = nalu->len - (i - 1) * MAX_NALU_LEN - header_off;
			if (fua_len > MAX_NALU_LEN)
			{
				fua_len = MAX_NALU_LEN;
			}
			rtp_packet_t* rtp_temp = nullptr;
			pack_FuA(rtp_temp, &fuidc, &fuhdr, nalu->payload + (i - 1) * MAX_NALU_LEN + header_off, fua_len, time_inc);
			if (rtp_temp)
			{
				_pack_rtp.push_back(rtp_temp);
			}
			if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_IDR)
			{
				time_inc = false;
			}
		}
		rtp = _pack_rtp.front();
		_pack_rtp.pop_front();
	}
	
}

void RtpH264Encoder::pack_FuA(rtp_packet_t*& rtp, FU_INDICATOR* idc, FU_HEADER* hdr, 
	const char* Nalu, int len, bool time_inc)
{
	if (len <= 0)
	{
		return;
	}
	int payload_len = 2 + len;
	if (payload_len > 1362)
	{
		int a = 1;
	}
	rtp = rtp_alloc(payload_len);//sizeof(FU_INDICATOR) + sizeof(FU_HEADER) + len;
	if (time_inc)
	{
		_sess.timestamp += _ts_step;
	}
	_sess.seq_number += 1;
	rtp_pack(rtp, &_param, &_sess, Nalu, payload_len, 2);
	if (rtp->payload_len > 1362 || rtp->ext_len > 0)
	{
		int a = 1;
	}
	FU_INDICATOR* pidc = (FU_INDICATOR*)rtp->arr;
	pidc->F = idc->F;
	pidc->NRI = idc->NRI;
	pidc->TYPE = idc->TYPE;

	FU_HEADER* phdr = (FU_HEADER*)(rtp->arr + 1);
	phdr->E = hdr->E;
	phdr->S = hdr->S;
	phdr->R = hdr->R;
	phdr->TYPE = hdr->TYPE;
	if (rtp->payload_len > 1362 || rtp->ext_len > 0)
	{
		int a = 1;
	}
}

rtp_packet_t* RtpH264Encoder::pack_single(const char* Nalu, int len, bool time_inc)
{
	std::cout << "PACK SINGLE" << std::endl;
	rtp_packet_t* rtp = rtp_alloc(len);
	if (time_inc)
	{
		_sess.timestamp += _ts_step;
	}
	_sess.seq_number += 1;
	rtp_pack(rtp, &_param, &_sess, Nalu, len);
	return rtp;
}