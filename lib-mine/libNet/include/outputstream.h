////////////////////////////////////////////////////////////////////////////////////
// 
// @file: outputstream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:13
// @brief: 输出缓冲区
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __OUTPUT_STREAM_H_
#define __OUTPUT_STREAM_H_

#include "type.h"
#include "socketdef.h"

namespace tcpnet {

class Packet ;
class Socket;
class SocketStream;
////////////////////////////////////////////////////////////////////////////////
//	class OutputStream
////////////////////////////////////////////////////////////////////////////////
class OutputStream
{
public :
	OutputStream() ;
	virtual ~OutputStream( ) ;

public :
	//初始化
	bool		init(int bufferSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int maxBufferSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//清理
	void		cleanUp( ) ;
	//写入数据
	int			write(const char* buf, int len) ;
	bool		writePacket(SocketStream& socketstream, Packet* pPacket ) ;
	//修改数据
	bool		modifyValue(const void*);
	//发送
	int			flush(Socket* pSocket) ;
	//重新初始化
	void		initsize() ;
	bool		resize(int size) ;
	//获得容量
	int			capacity() const { return m_nBufferLength ; }
	//获得缓冲长度
	int			getLength() const ;
	//获得缓冲区指针
	char*	getBuffer() const { return m_pBuffer ; }
	//是否缓冲区为空
	bool		isEmpty() const { return m_Head==m_Tail ; }
	//获取头尾
	int			getHead() const { return m_Head; }
	int			getTail() const { return m_Tail; }

private :
	char*m_pBuffer ;					//缓冲区
	int		m_nBufferLength;			//缓冲区长度
	int		m_nMaxBufferLength ;	//缓冲区最大长度
	int		m_Head ;					//头标记
	int		m_Tail ;					//尾标记
};

} //namespace tcpnet
using namespace tcpnet;

#endif
