#include "rtp_h264_encoder.h"
#include <stdlib.h>

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
		rtp = pack_single(nalu->payload, nalu->len, false);
		_unpack_list.pop_front();
		return _unpack_list.size();
	}
	else if ((hdr->TYPE & NALU_TYPE_MASK) == NALU_TYPE_PPS)
	{
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
	int count = nalu->len / MAX_NALU_LEN;
	if (count * MAX_NALU_LEN < nalu->len)
	{
		count += 1;
	}
	if (count == 1)
	{
		rtp = pack_single(nalu->payload, nalu->len, time_inc);
	}
	else
	{
		FU_INDICATOR fuidc;
		FU_HEADER fuhdr;
		//拆分为多个rtp包
		for (int i = 1; i <= count; ++i)
		{
			if (i == 1)
			{
				fuidc.F = 0;
				fuidc.NRI = 0;
				fuidc.TYPE = 28;

				fuhdr.E = 0;
				fuhdr.R = 0;
				fuhdr.S = 1;
				fuhdr.TYPE = hdr->TYPE;
			}
			else if (i > 1 && i < count)
			{
				fuidc.F = 0;
				fuidc.NRI = 0;
				fuidc.TYPE = 28;

				fuhdr.E = 0;
				fuhdr.R = 0;
				fuhdr.S = 0;
				fuhdr.TYPE = hdr->TYPE;
			}
			else
			{
				fuidc.F = 0;
				fuidc.NRI = 0;
				fuidc.TYPE = 28;

				fuhdr.E = 1;
				fuhdr.R = 0;
				fuhdr.S = 0;
				fuhdr.TYPE = hdr->TYPE;
			}
			int fua_len = nalu->len - (i - 1) * MAX_NALU_LEN;
			if (fua_len > MAX_NALU_LEN)
			{
				fua_len = MAX_NALU_LEN;
			}
			rtp_packet_t* rtp_temp;
			pack_FuA(rtp_temp, &fuidc, &fuhdr, nalu->payload + (i - 1) * MAX_NALU_LEN, fua_len, time_inc);
			_pack_rtp.push_back(rtp_temp);
		}
		rtp = _pack_rtp.front();
		_pack_rtp.pop_front();
	}
}

void RtpH264Encoder::pack_FuA(rtp_packet_t*& rtp, FU_INDICATOR* idc, FU_HEADER* hdr, 
	const char* Nalu, int len, bool time_inc)
{
	rtp = rtp_alloc(2 + len);//sizeof(FU_INDICATOR) + sizeof(FU_HEADER) + len;
	if (time_inc)
	{
		_sess.timestamp += _ts_step;
	}
	_sess.seq_number += 1;
	rtp_pack(rtp, &_param, &_sess, Nalu, len, 2);
	FU_INDICATOR* pidc = (FU_INDICATOR*)rtp->arr;
	pidc->F = idc->F;
	pidc->NRI = idc->NRI;
	pidc->TYPE = idc->TYPE;

	FU_HEADER* phdr = (FU_HEADER*)(rtp->arr + 1);
	phdr->E = hdr->E;
	phdr->S = hdr->S;
	phdr->R = hdr->R;
	phdr->TYPE = hdr->TYPE;
}

rtp_packet_t* RtpH264Encoder::pack_single(const char* Nalu, int len, bool time_inc)
{
	rtp_packet_t* rtp = rtp_alloc(len);
	if (time_inc)
	{
		_sess.timestamp += _ts_step;
	}
	_sess.seq_number += 1;
	rtp_pack(rtp, &_param, &_sess, Nalu, len);
	return rtp;
}