////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socketstream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:08
// @brief: socket缓冲区
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __SOCKET_STREAM_H_
#define __SOCKET_STREAM_H_

#include "type.h"
#include "socket.h"
#include "socketdef.h"

namespace tcpnet {

class Packet;
class InputStream;
class OutputStream;
////////////////////////////////////////////////////////////////////////////////
//	class SocketStream
////////////////////////////////////////////////////////////////////////////////
class SocketStream
{
public :
	SocketStream() ;
	virtual ~SocketStream( ) ;

public :
	//初始化
	bool		init(Socket* pSocket,
		int inbufferSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int inmaxBufferSize = MAX_SOCKETINPUT_BUFFERSIZE,
		int outbufferSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int outmaxBufferSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//清理
	void		cleanUp( ) ;
	//填充数据
	int			fill( char* buf, int length, EM_TYPE_MSG_MODE mode ,int& nSize) ;
	//读
	bool		readPacket( Packet* pPacket ) ;
	//写
	bool		writePacket( Packet* pPacket ) ;
	//接收数据
	int			reveive( ) ;
	//发送
	int			send();
	//获得输入Stream
	InputStream* getInputStream() { return m_pInputStream;}
	//获得输出Stream
	OutputStream* getOutputStream() { return m_pOutputStream;}

private :
	Socket*			m_pSocket ;			//socket
	InputStream*	m_pInputStream;		//输入缓冲
	OutputStream*	m_pOutputStream;	//输出缓冲
};

} //namespace tcpnet
using namespace tcpnet;

#endif

