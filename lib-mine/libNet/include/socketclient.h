////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socketclient.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:12
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_CLIENT_H_
#define __SOCKET_CLIENT_H_

#include "socket.h"
#include "socketdef.h"

namespace tcpnet {

////////////////////////////////////////////////////////////////////////////////
//	class SocketClient
////////////////////////////////////////////////////////////////////////////////
class SocketClient: public Socket
{
public :
	SocketClient () ;
	virtual ~SocketClient () ;

public :
	//创建socket
	bool create(const char* host , uint port);
	//关闭连接
	void close();
	//连接远程主机
	bool connect();
	//获得主机端口
	uint getHostPort() const;
	//获得主机IP
	unsigned long getHostIP() const;

public :
	//主机IP
	char m_HostIp[MAX_IP_SIZE];
	//主机端口
	uint m_HostPort;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
