////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socketserver.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:10
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_SERVER_H_
#define __SOCKET_SERVER_H_

#include "socket.h"
#include "socketAPI.h"
#include "socketdef.h"

namespace tcpnet {

////////////////////////////////////////////////////////////////////////////////
//	class SocketServer
////////////////////////////////////////////////////////////////////////////////
class SocketServer: public Socket
{
public :
	SocketServer () ;
	virtual ~SocketServer () ;

public :
	//创建socket
	bool create(const char* host, int port);
	//关闭连接
	void close();
	//获得主机端口
	uint getPort()const;
	//获得主机IP
	unsigned long getIP()const;
	//重用地址
	bool reuseAddr(bool on = true);
	//绑定
	bool bind();
	//监听
	bool listen( int backlog );
	//接受连接
	SOCK accept( struct sockaddr* addr, int* addrlen );
	bool accept ( Socket* socket ) ;
public :
	//主机IP
	char m_Ip[MAX_IP_SIZE];
	//主机端口
	uint m_Port;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
