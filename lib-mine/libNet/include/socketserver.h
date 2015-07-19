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
	//����socket
	bool create(const char* host, int port);
	//�ر�����
	void close();
	//��������˿�
	uint getPort()const;
	//�������IP
	unsigned long getIP()const;
	//���õ�ַ
	bool reuseAddr(bool on = true);
	//��
	bool bind();
	//����
	bool listen( int backlog );
	//��������
	SOCK accept( struct sockaddr* addr, int* addrlen );
	bool accept ( Socket* socket ) ;
public :
	//����IP
	char m_Ip[MAX_IP_SIZE];
	//�����˿�
	uint m_Port;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
