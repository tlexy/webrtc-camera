#ifndef RTP_H264_DEF_H
#define RTP_H264_DEF_H

#include <stdint.h>

#define NALU_TYPE_MASK 0X07

#define NALU_TYPE_SPS 0x07
#define NALU_TYPE_PPS 0x08
#define NALU_TYPE_SEI 0x06

typedef struct {
    //byte 0
    unsigned char TYPE : 5;
    unsigned char NRI : 2;
    unsigned char F : 1;

} NALU_HEADER; /**//* 1 BYTES */

typedef struct {
    //byte 0
    unsigned char TYPE : 5;
    unsigned char NRI : 2;
    unsigned char F : 1;


} FU_INDICATOR; /**//* 1 BYTES */

typedef struct {
    //byte 0
    unsigned char TYPE : 5;
    unsigned char R : 1;
    unsigned char E : 1;
    unsigned char S : 1;
} FU_HEADER; /**//* 1 BYTES */

typedef struct
{
    int len;
    char* payload;
}NALU_UNIT;

typedef struct
{
    NALU_HEADER hdr;
    int len;
    char* payload;

}RTP_NALU_UNIT;

#endif