////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socket.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:13
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SOCKET_H_
#define __SOCKET_H_

#include "type.h"
#include "socketdef.h"

namespace tcpnet {

extern int getSocketErrorCode();
extern char* getSocketErrorDescription();

////////////////////////////////////////////////////////////////////////////////
//	class Socket
////////////////////////////////////////////////////////////////////////////////
class Socket
{
public :
	Socket () ;
	virtual ~Socket () ;

public :
	//创建socket
	bool createSocket();
	//关闭连接
	void socketClose ();
	//获得端口
	SOCK getSocket() const;
	//设置关闭延时时间
	virtual bool setLinger(uint lingertime) ;
	//设置阻塞或非阻塞模式
	virtual bool setNonBlocking(bool on = true);
	//禁用delay
	virtual bool setNonDelay(bool on = true);
	//是否socket有错误
	virtual bool isSockError()const;
	//获得错误码
	virtual int getSockError()const;
	//获得接受缓冲区buffer大小
	virtual int getReceiveBufferSize()const;
	//设置接收缓冲区大小
	virtual bool setReceiveBufferSize(int size);
	//获得发送缓冲区大小
	virtual int getSendBufferSize()const;
	//设置发送缓冲区大小
	virtual bool setSendBufferSize(int size);
	//发送数据
	virtual int send(const void* buf, int length, int flags = 0);
	//接收数据
	virtual int receive(void* buf, int length, int flags = 0);
	//现有数据
	uint	available()const;
	//获得地址描述
	SOCKADDR_IN* getSocketAddr(){ return &m_SockAddr; };
	void setSocket(SOCK s){ m_SocketID = s;};
	//判断socket是否有效
	bool isSocketValid();
	//获得IP
	char* getIp();

protected:
	//套接字
	SOCK m_SocketID;
	//地址描述
	SOCKADDR_IN m_SockAddr;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
