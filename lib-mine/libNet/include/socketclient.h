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
	//����socket
	bool create(const char* host , uint port);
	//�ر�����
	void close();
	//����Զ������
	bool connect();
	//��������˿�
	uint getHostPort() const;
	//�������IP
	unsigned long getHostIP() const;

public :
	//����IP
	char m_HostIp[MAX_IP_SIZE];
	//�����˿�
	uint m_HostPort;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
