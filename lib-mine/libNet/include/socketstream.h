////////////////////////////////////////////////////////////////////////////////////
// 
// @file: socketstream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:08
// @brief: socket������
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
	//��ʼ��
	bool		init(Socket* pSocket,
		int inbufferSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int inmaxBufferSize = MAX_SOCKETINPUT_BUFFERSIZE,
		int outbufferSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int outmaxBufferSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//����
	void		cleanUp( ) ;
	//�������
	int			fill( char* buf, int length, EM_TYPE_MSG_MODE mode ,int& nSize) ;
	//��
	bool		readPacket( Packet* pPacket ) ;
	//д
	bool		writePacket( Packet* pPacket ) ;
	//��������
	int			reveive( ) ;
	//����
	int			send();
	//�������Stream
	InputStream* getInputStream() { return m_pInputStream;}
	//������Stream
	OutputStream* getOutputStream() { return m_pOutputStream;}

private :
	Socket*			m_pSocket ;			//socket
	InputStream*	m_pInputStream;		//���뻺��
	OutputStream*	m_pOutputStream;	//�������
};

} //namespace tcpnet
using namespace tcpnet;

#endif

