#include "socket.h"
#include "socketAPI.h"
#include "socketdef.h"
#include "assert.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#include <arpa/inet.h>
#include <netinet/tcp.h>
#endif

namespace tcpnet {

int getSocketErrorCode()
{
	extern int g_socketErrorCode;
	return g_socketErrorCode ;
}

char* getSocketErrorDescription()
{
	extern char g_socketError[SOCKET_ERROR_SIZE] ;
	return g_socketError ;
}

Socket::Socket ()
{
	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
}

Socket::~Socket () 
{ 
	if (m_SocketID != INVALID_SOCKET)
	{
		socketClose();
	}
}

bool Socket::createSocket()
{
	__ENTER_FUNCTION
		m_SocketID = SocketAPI::socket_create( AF_INET , SOCK_STREAM , 0 );
	if (m_SocketID == INVALID_SOCKET)
	{
		return false;
	}
	memset( &m_SockAddr , 0 , sizeof(m_SockAddr) );
	m_SockAddr.sin_family = AF_INET;

	return true;
	__LEAVE_FUNCTION
	return false ;
}

//�ر�����
void Socket::socketClose ()
{
	__ENTER_FUNCTION
	SOCK socketID = m_SocketID;
	if((m_SocketID != INVALID_SOCKET)&& (!isSockError())) 
	{
		SocketAPI::socket_close(socketID );
		m_SocketID = INVALID_SOCKET;
		memset( &m_SockAddr , 0 , sizeof(m_SockAddr) );
	}
	__LEAVE_FUNCTION
}

//��ö˿�
SOCK Socket::getSocket ()const
{
	__ENTER_FUNCTION

	return m_SocketID; 

	__LEAVE_FUNCTION
	return -1;
}

//���ùر���ʱʱ��
bool Socket::setLinger (uint lingertime)
{
	__ENTER_FUNCTION

	struct linger ling;

	ling.l_onoff = lingertime > 0 ? 1 : 0;
	ling.l_linger = lingertime;
	return SocketAPI::socket_setsockopt( m_SocketID , SOL_SOCKET , SO_LINGER , &ling , sizeof(ling) );

	__LEAVE_FUNCTION
	return false ;	
}

//�Ƿ�socket�д���
bool Socket::isSockError()const
{
	__ENTER_FUNCTION

	int error=0;
	int length = (int)sizeof(error);
	bool bResult = SocketAPI::socket_getsockopt( m_SocketID , SOL_SOCKET , SO_ERROR , &error, &length );
	if( bResult == true ) 
		return false;
	else 			  
		return true;
	__LEAVE_FUNCTION
	return true ;
}

//��ô�����
int Socket::getSockError()const
{
	int error = SOCKET_ERROR_UNKNOWN;
	__ENTER_FUNCTION

	int length = (int)sizeof(error);
	bool bResult = SocketAPI::socket_getsockopt( m_SocketID , SOL_SOCKET , SO_ERROR , &error, &length );
	if( bResult == false ) 
		return SOCKET_ERROR_UNKNOWN;
	else 			  
		return error;

	__LEAVE_FUNCTION
	return error;
}


//���������������ģʽ
bool Socket::setNonBlocking (bool on)
{
	__ENTER_FUNCTION

		return SocketAPI::setsocketnonblocking( m_SocketID , on );

	__LEAVE_FUNCTION
	return false ;
}

bool Socket::setNonDelay (bool on)
{
	__ENTER_FUNCTION
	int intOn = on;
	return SocketAPI::socket_setsockopt( m_SocketID ,IPPROTO_TCP, TCP_NODELAY, (const void *) &intOn, sizeof(intOn));

	__LEAVE_FUNCTION
	return false ;
}

//��ý��ջ�������С
int Socket::getReceiveBufferSize ()const 
{ 
	__ENTER_FUNCTION

	int ReceiveBufferSize = 0;
	int size = (int)sizeof(ReceiveBufferSize);
	SocketAPI::socket_getsockopt( m_SocketID , SOL_SOCKET , SO_RCVBUF , &ReceiveBufferSize, &size );

	return ReceiveBufferSize;
	__LEAVE_FUNCTION
	return 0 ;
}

//���ý��ջ�������С
bool Socket::setReceiveBufferSize (int size)
{ 
	__ENTER_FUNCTION

	return (bool)(SocketAPI::socket_setsockopt( m_SocketID , SOL_SOCKET , SO_RCVBUF , &size, sizeof(int) ) ) ;

	__LEAVE_FUNCTION
	return false ;
}

//��÷��ͻ�������С
int Socket::getSendBufferSize ()const 
{ 
	__ENTER_FUNCTION

	int SendBufferSize;
	int size = (int)sizeof(SendBufferSize);
	SocketAPI::socket_getsockopt( m_SocketID , SOL_SOCKET , SO_SNDBUF , &SendBufferSize, &size );
	return SendBufferSize;

	__LEAVE_FUNCTION
	return 0 ;
}

//���÷��ͻ�������С
bool Socket::setSendBufferSize (int size)
{ 
	__ENTER_FUNCTION

		return (bool)(SocketAPI::socket_setsockopt( m_SocketID , SOL_SOCKET , SO_SNDBUF , &size, sizeof(int) ) );
	__LEAVE_FUNCTION
	return false ;
}

//��������
int Socket::send(const void* buf, int length, int flags)
{
	__ENTER_FUNCTION

		return SocketAPI::socket_send( m_SocketID , buf , length , flags );

	__LEAVE_FUNCTION
	return 0 ;
}

//��������
int Socket::receive(void* buf, int length, int flags)
{
	__ENTER_FUNCTION

		return SocketAPI::socket_recv( m_SocketID , buf , length , flags );

	__LEAVE_FUNCTION
	return 0 ;
}

//��������
uint Socket::available()const
{
	__ENTER_FUNCTION
		return SocketAPI::socket_available( m_SocketID );
	__LEAVE_FUNCTION
	return 0 ;
}

//�ж�socket�Ƿ���Ч
bool Socket::isSocketValid()
{
	__ENTER_FUNCTION

	if (m_SocketID == INVALID_SOCKET)
	{
		return false;
	}
	
	return true;

	__LEAVE_FUNCTION
	return false;
}

//���IP
char* Socket::getIp()
{
	__ENTER_FUNCTION

	return inet_ntoa(m_SockAddr.sin_addr);

	__LEAVE_FUNCTION
	return NULL;
}

} //namespace tcpnet
