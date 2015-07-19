#include "socketclient.h"
#include "assert.h"
#include "strUtil.h"
#include "socketAPI.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#include <arpa/inet.h>   //inet_addr,inet_aton
#endif

namespace tcpnet {

SocketClient::SocketClient () 
{
	__ENTER_FUNCTION
	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	memset( m_HostIp, 0, MAX_IP_SIZE ) ;
	m_HostPort = 0 ;
	__LEAVE_FUNCTION
}

SocketClient::~SocketClient () 
{
	__ENTER_FUNCTION
	if (m_SocketID != INVALID_SOCKET)
	{
		close();
	}
	__LEAVE_FUNCTION
}

bool SocketClient::create( const char* host , uint port )
{
	__ENTER_FUNCTION

	bool bRet = createSocket();
	if (!bRet)
	{
		return false;
	}
	Strncpy( m_HostIp, host, MAX_IP_SIZE-1 ) ;
	m_HostPort = port ;
	memset( &m_SockAddr , 0 , sizeof(m_SockAddr) );
	m_SockAddr.sin_family = AF_INET;

	return true;
	__LEAVE_FUNCTION
	return false ;
}

//关闭连接
void SocketClient::close ()
{
	__ENTER_FUNCTION

	if((m_SocketID != INVALID_SOCKET)&& (!isSockError())) 
	{
		socketClose();
	}
	m_SocketID = INVALID_SOCKET ;
	memset( &m_SockAddr, 0, sizeof(SOCKADDR_IN) ) ;
	memset( m_HostIp, 0, MAX_IP_SIZE ) ;
	m_HostPort = 0 ;

	__LEAVE_FUNCTION
}

//连接远程主机
bool SocketClient::connect()
{
	__ENTER_FUNCTION

	m_SockAddr.sin_addr.s_addr = inet_addr( m_HostIp );
	m_SockAddr.sin_port = htons(m_HostPort);
	bool bResult = SocketAPI::socket_connect( m_SocketID , (const struct sockaddr *)&m_SockAddr , sizeof(m_SockAddr) );
	return bResult;

	__LEAVE_FUNCTION
	return false ;
}

//获得主机端口
uint SocketClient::getHostPort () const
{
	__ENTER_FUNCTION
	return m_HostPort; 
	__LEAVE_FUNCTION
}

//获得主机IP
unsigned long SocketClient::getHostIP () const
{
	__ENTER_FUNCTION

	return (unsigned long)(m_SockAddr.sin_addr.s_addr);; 

	__LEAVE_FUNCTION
}

} //namespace tcpnet
