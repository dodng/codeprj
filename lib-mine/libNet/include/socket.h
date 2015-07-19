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
	//����socket
	bool createSocket();
	//�ر�����
	void socketClose ();
	//��ö˿�
	SOCK getSocket() const;
	//���ùر���ʱʱ��
	virtual bool setLinger(uint lingertime) ;
	//���������������ģʽ
	virtual bool setNonBlocking(bool on = true);
	//����delay
	virtual bool setNonDelay(bool on = true);
	//�Ƿ�socket�д���
	virtual bool isSockError()const;
	//��ô�����
	virtual int getSockError()const;
	//��ý��ܻ�����buffer��С
	virtual int getReceiveBufferSize()const;
	//���ý��ջ�������С
	virtual bool setReceiveBufferSize(int size);
	//��÷��ͻ�������С
	virtual int getSendBufferSize()const;
	//���÷��ͻ�������С
	virtual bool setSendBufferSize(int size);
	//��������
	virtual int send(const void* buf, int length, int flags = 0);
	//��������
	virtual int receive(void* buf, int length, int flags = 0);
	//��������
	uint	available()const;
	//��õ�ַ����
	SOCKADDR_IN* getSocketAddr(){ return &m_SockAddr; };
	void setSocket(SOCK s){ m_SocketID = s;};
	//�ж�socket�Ƿ���Ч
	bool isSocketValid();
	//���IP
	char* getIp();

protected:
	//�׽���
	SOCK m_SocketID;
	//��ַ����
	SOCKADDR_IN m_SockAddr;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
