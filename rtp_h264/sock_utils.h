#ifndef CROSS_TCP_CLIENT_H
#define CROSS_TCP_CLIENT_H

#include <stdint.h>
#include <string>
#include <vector>
#if defined(_WIN32)
//#include <windows.h>
#include<ws2tcpip.h>
#include <winsock2.h>
typedef UINT in_addr_t;
#else
#include <netdb.h>
#include <sys/socket.h>
#endif
namespace sockets
{
	void Init();
	void Destroy();
	//void perr_exit(const char *s);
	void err_exit(const char *s);
	int Connect(int fd, const struct sockaddr *sa, socklen_t salen);
	int Socket(int family, int type, int protocol);
	int Read(int fd, void *ptr, size_t nbytes);
	int Write(int fd, const void *ptr, size_t nbytes);
	void Close(int fd);
	in_addr_t getIpaddrByName(const char* name, char* buff, const int bufferSize);

	void getIpByDoname(const std::string&, std::vector<std::string>&);
	bool isIp(const std::string& str);
	bool isNum(const std::string& str);

	uint64_t hostToNetwork64(uint64_t host64);

	uint32_t ip2a(const char* ip);

	///上层API
	int ConnectTcp(const char* ip_str, int port, int timeout = 0);
	int SendUdpData(int fd, const char* ip_str, int port, const char* data, int len);

}
#endif