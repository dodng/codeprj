////////////////////////////////////////////////////////////////////////////////////
// 
// @file: outputstream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:13
// @brief: ���������
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
	//��ʼ��
	bool		init(int bufferSize = DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
		int maxBufferSize = MAX_SOCKETOUTPUT_BUFFERSIZE);
	//����
	void		cleanUp( ) ;
	//д������
	int			write(const char* buf, int len) ;
	bool		writePacket(SocketStream& socketstream, Packet* pPacket ) ;
	//�޸�����
	bool		modifyValue(const void*);
	//����
	int			flush(Socket* pSocket) ;
	//���³�ʼ��
	void		initsize() ;
	bool		resize(int size) ;
	//�������
	int			capacity() const { return m_nBufferLength ; }
	//��û��峤��
	int			getLength() const ;
	//��û�����ָ��
	char*	getBuffer() const { return m_pBuffer ; }
	//�Ƿ񻺳���Ϊ��
	bool		isEmpty() const { return m_Head==m_Tail ; }
	//��ȡͷβ
	int			getHead() const { return m_Head; }
	int			getTail() const { return m_Tail; }

private :
	char*m_pBuffer ;					//������
	int		m_nBufferLength;			//����������
	int		m_nMaxBufferLength ;	//��������󳤶�
	int		m_Head ;					//ͷ���
	int		m_Tail ;					//β���
};

} //namespace tcpnet
using namespace tcpnet;

#endif
