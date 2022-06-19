#ifndef HI_RTP_H
#define HI_RTP_H

#include <stdint.h>

typedef struct rtp_header
{
#ifdef HIRTP_BIGENDIAN
	uint16_t version : 2;
	uint16_t padbit : 1;
	uint16_t extbit : 1;
	uint16_t cc : 4;
	uint16_t markbit : 1;
	uint16_t paytype : 7;
#else
	uint16_t cc : 4;
	uint16_t extbit : 1;
	uint16_t padbit : 1;
	uint16_t version : 2;
	uint16_t paytype : 7;
	uint16_t markbit : 1;
#endif
	uint16_t seq_number;
	uint32_t timestamp;
	uint32_t ssrc;
	//uint32_t csrc[16];//maybe never use
} rtp_hdr_t;

typedef struct rtp_parameter
{
	uint8_t version;
	uint8_t padding;
	uint8_t ext;
	uint8_t cc;		//always zero
	uint8_t marker;
	uint8_t pt;
}rtp_parameter_t;

typedef struct rtp_session
{
	uint16_t seq_number;
	uint32_t timestamp;
	uint32_t ssrc;
}rtp_session_t;

typedef struct {
	uint16_t profile_specific; /* profile-specific info               */
	uint16_t length;           /* number of 32-bit words in extension */
} rtp_hdr_ext_t;

typedef struct rtp_packet
{
	rtp_hdr_t hdr;
	rtp_hdr_ext_t hdr_ext;
	void* ext_body;
	uint16_t payload_len;
	uint16_t ext_len;
	uint8_t arr[0];
}rtp_packet_t;

//申请一个rtp包
rtp_packet_t* rtp_alloc(int payload_size);

//添加扩展头
void rtp_add_ext_hdr(rtp_packet_t*, uint16_t profile, uint16_t len, void* ext_body);
void rtp_copy_ext_hdr(rtp_packet_t*, uint16_t profile, uint16_t len, void* ext_body);
//添加扩展头，内部分配扩展body
void* rtp_alloc_ext_hdr(rtp_packet_t*, uint16_t profile, uint16_t len);

int rtp_len(rtp_packet_t* rtp);

//打包rtp包，已经进行字节序转换
void rtp_pack(rtp_packet_t* rtp, rtp_parameter_t* param, rtp_session_t* session,
	const void* payload, int payload_len, int payload_off = 0);

//复制一个rtp包的内容（到dest指定的内存中）
int rtp_copy(rtp_packet_t* rtp, void* dest, int dest_len);

//是否可能是一个rtp包，返回0代表是一个rtp包
int rtp_unpack_test(void* src, int len);

int rtp_payload_type(void* src, int len);

//将buffer转为rtp_packet
rtp_packet_t* rtp_unpack(void* src, int len);

void rtp_free(rtp_packet_t*);

void dump(rtp_packet_t*, const char* text);


#endif