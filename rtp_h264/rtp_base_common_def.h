#ifndef RTP_BASE_COMMON_DEF_H
#define RTP_BASE_COMMON_DEF_H

#include <stdint.h>

namespace rtp_base
{
#pragma pack(push, 4)
	//扩展的rtp header
	struct rtc_ext_header
	{
		uint16_t ext_seq;
		uint8_t group_size;
		uint8_t red_size;//冗余包大小
		uint8_t group_seq;//for 0 to group_size-1
	};
#pragma pack(pop)

	//RTP扩展头类型
	enum rtp_ext_profile
	{
		eGroupExt = 1,
		eEndExt
	};

	enum rtp_payload_type
	{
		eH264PayLoad = 96,
		eAacLcPayLoad = 97,
		eFecPayLoad = 98,
		eEndPayLoad
	};
}

#endif