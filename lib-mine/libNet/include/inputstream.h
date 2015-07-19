////////////////////////////////////////////////////////////////////////////////////
// 
// @file: inputstream.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:14
// @brief: ���뻺����
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __INPUT_STREAM_H__
#define __INPUT_STREAM_H__

#include "type.h"
#include "socketdef.h"

namespace tcpnet {

class Packet ;
class Socket;
class SocketStream;
////////////////////////////////////////////////////////////////////////////////
//	class InputStream
////////////////////////////////////////////////////////////////////////////////
class InputStream
{
public :
	InputStream() ;
	virtual ~InputStream( ) ;

public :
	//��ʼ��
	bool		init(int bufferSize = DEFAULT_SOCKETINPUT_BUFFERSIZE, 
		int maxBufferSize = MAX_SOCKETINPUT_BUFFERSIZE);
	//����
	void		cleanUp() ;
	//������
	int			read(char* buf, int length) ;
	bool		readPacket(SocketStream& socketstream, Packet* pPacket) ;
	//��̽����
	bool		peek(char* buf, int length) ;
	//����ָ�����Ȼ���
	bool		skip(int length) ;
	//����socket���ݵ�������
	int			reveive(Socket* pSocket);
	//���³�ʼ��
	void		initsize() ;
	//���û�������С
	bool		resize(int size) ;
	//�������
	int			capacity() const { return m_nBufferLength; }
	//���buf����
	int			getLength() const ;
	//��û�����
	char*	getBuffer() const {return m_pBuffer; }
	//�Ƿ�bufΪ��
	bool		isEmpty() const { return m_Head == m_Tail; }
	//���ͷβ
	int			getHead() const { return m_Head; }
	int			getTail() const { return m_Tail; }

private :
	char*m_pBuffer ;					//������
	int		m_nBufferLength;			//����������
	int		m_nMaxBufferLength ;		//��������󳤶�
	int		m_Head ;					//ͷ���
	int		m_Tail ;					//β���
};

} //namespace tcpnet
using namespace tcpnet;

#endif
