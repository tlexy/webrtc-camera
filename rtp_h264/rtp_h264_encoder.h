#ifndef RTP_H264_ENCODER_H
#define RTP_H264_ENCODER_H

#include <memory>
#include <list>
#include "rtp.h"
#include "rtp_h264_def.h"

//0 没有定义
//1 - 23 NAL 单元单个NAL 单元包
//24 STAP - A 单一时间的组合包
//25 STAP - B 单一时间的组合包
//26 MTAP16 多个时间的组合包
//27 MTAP24 多个时间的组合包
//28 FU - A 分片的单元（fragment unit ）
//29 FU - B 分片的单元

#define MAX_NALU_LEN 1360

class RtpH264Encoder
{
public:
	void init(rtp_parameter_t, rtp_session_t, int);
	//NALU不带start code
	//将PPS/SPS/ESI打包为一个rtp包
	bool encode(const char* Nalu, int len);
	/// <summary>
	/// 
	/// </summary>
	/// <returns>
	/// -1代表没有可返回的包
	/// 0表示当次调用有返回，但下一次没有返回了，除非重新调用encode
	/// >0表示仍然有包输出
	/// </returns>
	int get_packet(rtp_packet_t*& rtp);

	rtp_packet_t* pack_single(const char* Nalu, int len, bool time_inc = true);

private:
	//仅仅用于打包SPS和PPS及SEI，它们都没有增加时间
	void pack(rtp_packet_t*& rtp, int count);
	void pack(rtp_packet_t*& rtp);

	void pack_FuA(rtp_packet_t*&, FU_INDICATOR*, FU_HEADER*, 
		const char* Nalu, int len, bool time_inc);

private:
	std::list<NALU_UNIT*> _unpack_list;
	int _unpack_size{0};
	int _ts_step{ 3000 };

	std::list<rtp_packet_t*> _pack_rtp;

	rtp_parameter_t _param;
	rtp_session_t _sess;
};

#endif