#pragma once
//
// This is a public header file, it must only include public header files.

#ifndef BYTE_ORDER_H
#define BYTE_ORDER_H

#include <stdint.h>
#if !defined(_WIN32)
#include <endian.h>
#else
#include <WinSock2.h>
#endif

namespace sockets
{

// the inline assembler code makes type blur,
// so we disable warnings for a while.
#if defined(__clang__) /*|| __GNUC_PREREQ (4,6)*/
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#if !defined(_WIN32)
	inline uint64_t hostToNetwork64(uint64_t host64)
	{
		return htobe64(host64);
	}

	inline uint32_t hostToNetwork32(uint32_t host32)
	{
		return htobe32(host32);
	}

	inline uint16_t hostToNetwork16(uint16_t host16)
	{
		return htobe16(host16);
	}

	inline uint64_t networkToHost64(uint64_t net64)
	{
		return be64toh(net64);
	}

	inline uint32_t networkToHost32(uint32_t net32)
	{
		return be32toh(net32);
	}

	inline uint16_t networkToHost16(uint16_t net16)
	{
		return be16toh(net16);
	}
#else
	inline uint64_t hostToNetwork64(uint64_t host64)
	{
		uint32_t pre = 0;
		uint32_t suf = 0;
		memcpy(&pre, &host64, sizeof(uint32_t));
		memcpy(&suf, &host64 + sizeof(uint32_t), sizeof(uint32_t));
		uint32_t npre = (uint32_t)htonl(pre);
		uint32_t nsuf = (uint32_t)htonl(nsuf);
		uint64_t ret = 0;
		memcpy(&ret, &nsuf, sizeof(nsuf));
		memcpy(&ret + sizeof(nsuf), &npre, sizeof(npre));
		return ret;
	}

	inline uint32_t hostToNetwork32(uint32_t host32)
	{
		return htonl(host32);
	}

	inline uint16_t hostToNetwork16(uint16_t host16)
	{
		return htons(host16);
	}

	/*inline uint64_t networkToHost64(uint64_t net64)
	{
		return be64toh(net64);
	}*/

	inline uint32_t networkToHost32(uint32_t net32)
	{
		return ntohl(net32);
	}

	inline uint16_t networkToHost16(uint16_t net16)
	{
		return ntohs(net16);
	}
#endif

#if defined(__clang__)/* || __GNUC_PREREQ (4,6)*/
#pragma GCC diagnostic pop
#else
#pragma GCC diagnostic warning "-Wconversion"
#pragma GCC diagnostic warning "-Wold-style-cast"
#endif


}

#endif 
