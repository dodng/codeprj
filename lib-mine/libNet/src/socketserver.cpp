#include "socketserver.h"
#include "assert.h"
#include "strUtil.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace tcpnet {

SocketServer::SocketServer () 
{
	__ENTER_FUNCTION
	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(m_SockAddr) ) ;
	memset( m_Ip, 0, MAX_IP_SIZE ) ;
	m_Port = 0 ;
	__LEAVE_FUNCTION
}

SocketServer::~SocketServer () 
{
	__ENTER_FUNCTION
	if (m_SocketID != INVALID_SOCKET)
	{
		close();
	}
	__LEAVE_FUNCTION
}

bool SocketServer::create( const char* host , int port)
{
	__ENTER_FUNCTION
	Strncpy( m_Ip, host, MAX_IP_SIZE-1 ) ;
	m_Port = port ;
	bool bRet = createSocket();
	if (!bRet)
	{
		return false;
	}
	bRet = reuseAddr( ) ;
	if (!bRet)
	{
		return false;
	}

	return true;
	__LEAVE_FUNCTION
	return false ;
}

//关闭连接
void SocketServer::close ()
{
	__ENTER_FUNCTION
	if((m_SocketID != INVALID_SOCKET)&& (!isSockError())) 
	{
		socketClose();
	}
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	memset( m_Ip, 0, MAX_IP_SIZE ) ;
	m_Port = 0 ;

	__LEAVE_FUNCTION
}

//获得主机端口
uint SocketServer::getPort ()const
{
	__ENTER_FUNCTION
	return m_Port; 
	__LEAVE_FUNCTION
}

//获得主机IP
unsigned long SocketServer::getIP ()const
{
	__ENTER_FUNCTION

	return (unsigned long)(m_SockAddr.sin_addr.s_addr);; 

	__LEAVE_FUNCTION
}

//绑定
bool SocketServer::bind()
{
	__ENTER_FUNCTION

	m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_SockAddr.sin_port        = htons(m_Port);

	bool result = SocketAPI::socket_bind( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) ) ;
	if( result )
		return true ;
	else
		return false ;

	__LEAVE_FUNCTION
	return false ;
}

//监听
bool SocketServer::listen( int backlog )
{
	__ENTER_FUNCTION

		return SocketAPI::socket_listen( m_SocketID , backlog );

	__LEAVE_FUNCTION
	return false ;	
}

//重用地址
bool SocketServer::reuseAddr ( bool on )
{
	__ENTER_FUNCTION 

	int opt = on == true ? 1 : 0;
	return SocketAPI::socket_setsockopt( m_SocketID , SOL_SOCKET , SO_REUSEADDR , &opt , sizeof(opt) );

	__LEAVE_FUNCTION
	return false;
}

//接受连接
SOCK SocketServer::accept( struct sockaddr* addr, int* addrlen )
{
	__ENTER_FUNCTION

		return SocketAPI::socket_accept( m_SocketID, addr, addrlen ) ;

	__LEAVE_FUNCTION
	return INVALID_SOCKET ;
}

bool SocketServer::accept ( Socket* socket )
{
	__ENTER_FUNCTION
	int addrlen = sizeof(SOCKADDR_IN) ;
	socket->socketClose();
	SOCK s = accept( (struct sockaddr *)(socket->getSocketAddr()), &addrlen ) ;
	if( s == INVALID_SOCKET )
		return false ;
	socket->setSocket(s);
	return true;
	__LEAVE_FUNCTION
	return false ;
}

} //namespace tcpnet
