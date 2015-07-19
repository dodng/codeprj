#include "outputstream.h"
#include "socketstream.h"
#include "assert.h"
#include "macroDef.h"
#include "socketdef.h"
#include "packet.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace tcpnet {

extern int  g_socketErrorCode;

OutputStream::OutputStream() 
{
	__ENTER_FUNCTION
	m_nBufferLength = 0 ;
	m_nMaxBufferLength = 0;
	m_Head = 0;
	m_Tail = 0;
	m_pBuffer = NULL;
	__LEAVE_FUNCTION
}

OutputStream::~OutputStream( ) 
{
	__ENTER_FUNCTION
	delete[] m_pBuffer;
	m_nBufferLength = 0 ;
	m_nMaxBufferLength = 0;
	m_Head = 0;
	m_Tail = 0;
	m_pBuffer = NULL;
	__LEAVE_FUNCTION
}

//初始化
bool	OutputStream::init(int bufferSize, int maxBufferSize)
{
	__ENTER_FUNCTION
	if ((bufferSize>0)&&(maxBufferSize>0))
	{
		m_nBufferLength = bufferSize ;
		m_nMaxBufferLength = maxBufferSize;
		m_Head = 0;
		m_Tail = 0;
		m_pBuffer = new char[ m_nBufferLength ];
		memset( m_pBuffer, 0, m_nBufferLength*sizeof(char) ) ;
		return true ;
	}
	__LEAVE_FUNCTION
	return false;
}

int OutputStream::getLength( )const
{
	__ENTER_FUNCTION

	if( m_Head<m_Tail )
		return m_Tail-m_Head;

	else if( m_Head>m_Tail ) 
		return m_nBufferLength-m_Head+m_Tail ;

	return 0 ;
	__LEAVE_FUNCTION
	return 0 ;
}

int OutputStream::write( const char* buf, int length ) 
{
	__ENTER_FUNCTION

	if ((buf == NULL)||(length <= 0))
	{
		return 0;
	}

	int nFree = ( (m_Head<=m_Tail)?(m_nBufferLength-m_Tail+m_Head-1):(m_Head-m_Tail-1) ) ;
	if( length>=nFree )
	{
		if( !resize( length-nFree+1 ) )
			return 0 ;
	}

	if( m_Head<=m_Tail ) 
	{	
		if( m_Head==0 ) 
		{
			nFree = m_nBufferLength - m_Tail - 1;
			memcpy( &m_pBuffer[m_Tail], buf, length ) ;
		} 
		else 
		{
			nFree = m_nBufferLength-m_Tail ;
			if( length<=nFree )
			{
				memcpy( &m_pBuffer[m_Tail], buf, length ) ;
			}
			else 
			{
				memcpy( &m_pBuffer[m_Tail], buf, nFree ) ;
				memcpy( m_pBuffer, &buf[nFree], length-nFree ) ;
			}
		}
	} 
	else 
	{	
		memcpy( &m_pBuffer[m_Tail], buf, length ) ;
	}

	m_Tail = (m_Tail+length)%m_nBufferLength ;
	return length;

	__LEAVE_FUNCTION
	return 0 ;
}

bool OutputStream::writePacket(SocketStream& socketstream, Packet* pPacket )
{
	__ENTER_FUNCTION

	if (pPacket == NULL)
		return false;

	Packet_ID packetID = pPacket->getPacketID() ;
	int w = write( (char*)&packetID , sizeof(Packet_ID) );
	if (w <= 0)
	{
		return false;
	}
	char packetEncryptFlag = pPacket->getEncryptFlag( ) ;
	w = write( (char*)&packetEncryptFlag , sizeof(char) );
	if (w <= 0)
	{
		return false;
	}
	char* pBuf = &m_pBuffer[m_Tail];
	int packetINT ;
	char* pPacketINT = (char*)&packetINT;
	w = write( (char*)&packetINT, sizeof(int) ) ;
	if (w <= 0)
	{
		return false;
	}
	pPacket->setSize(0);
	bool bRet = pPacket->fill( socketstream , EM_TYPE_MSG_MODE_WRITE ) ;
	if (bRet)
	{
		int packetSize = pPacket->getSize() ;
		packetINT = packetSize;
		if (m_Head < m_Tail)
		{
			int nbegin = m_Tail-packetSize-sizeof(int);
			char* pBuf = &m_pBuffer[nbegin];
			memcpy( pBuf, pPacketINT, sizeof(int) ) ;
		}
		else
		{
			if (m_Tail >= (packetSize+sizeof(int)))
			{
				char* pBuf = &m_pBuffer[m_Tail - packetSize - sizeof(int)];
				memcpy( pBuf, pPacketINT, sizeof(int) ) ;
			}
			else if(m_Tail > packetSize)
			{
				int nbegin = m_nBufferLength - (sizeof(int) - (m_Tail - packetSize));
				char* pBuf = &m_pBuffer[nbegin];
				memcpy( pBuf, pPacketINT, sizeof(int) - (m_Tail - packetSize)) ;
				memcpy( m_pBuffer, &pPacketINT[sizeof(int) - (m_Tail - packetSize)], m_Tail - packetSize ) ;
			}
			else
			{
				int nbegin = m_nBufferLength - (packetSize - m_Tail) - sizeof(int);
				char* pBuf = &m_pBuffer[nbegin];
				memcpy( pBuf, pPacketINT, sizeof(int) ) ;
			}
		}
	}
	
	return bRet;
	__LEAVE_FUNCTION
	return false ;
}

void OutputStream::initsize( )
{
	__ENTER_FUNCTION
	m_Head = 0 ;
	m_Tail = 0 ;
	delete[] m_pBuffer;
	m_pBuffer = new char[DEFAULT_SOCKETOUTPUT_BUFFERSIZE] ;
	m_nBufferLength = DEFAULT_SOCKETOUTPUT_BUFFERSIZE ;
	memset( m_pBuffer, 0, m_nBufferLength ) ;
	__LEAVE_FUNCTION
}

int OutputStream::flush( Socket* pSocket ) 
{
	__ENTER_FUNCTION
	if ( pSocket == NULL)
		throw 1;
	
	int nFlushed = 0;
	int nSent = 0;
	int nLeft;

	//如果单个客户端的缓存太大，则重新设置缓存，并将此客户端断开连接
	if( m_nBufferLength>m_nMaxBufferLength )
	{
		initsize( ) ;
		return EM_SOCKET_ERROR_STEP0;
	}

#if defined(_WIN32)
	uint flag = MSG_DONTROUTE ;
#elif defined(_LINUX64)
	int flag = MSG_NOSIGNAL ;
#endif

	__MYTRY 
	{
		if ( m_Head < m_Tail ) 
		{
			nLeft = m_Tail - m_Head;

			while ( nLeft > 0 ) 
			{
				nSent = pSocket->send( &m_pBuffer[m_Head] , nLeft , (int)flag );
				if (nSent==SOCKET_ERROR_WOULD_BLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return EM_SOCKET_ERROR_STEP1 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		} 
		else if( m_Head > m_Tail ) 
		{
			nLeft = m_nBufferLength - m_Head;

			while ( nLeft > 0 ) 
			{
				nSent = pSocket->send( &m_pBuffer[m_Head] , nLeft , (int)flag );
				if (nSent==SOCKET_ERROR_WOULD_BLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return EM_SOCKET_ERROR_STEP2 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
			m_Head = 0;

			nLeft = m_Tail;

			while ( nLeft > 0 ) 
			{
				nSent = pSocket->send( &m_pBuffer[m_Head] , nLeft , (int)flag );
				if (nSent==SOCKET_ERROR_WOULD_BLOCK) return 0 ; 
				if (nSent==SOCKET_ERROR) return EM_SOCKET_ERROR_STEP3 ;
				if (nSent==0) return 0;

				nFlushed += nSent;
				nLeft -= nSent;
				m_Head += nSent;
			}
		}

		if ( m_Head != m_Tail ) 
		{
			g_socketErrorCode = 0;
			return EM_SOCKET_ERROR_STEP4 ;
		}

	}
	__MYCATCH
	{
		if( nSent>0 )
		{
			m_Head += nSent;
		}
	} 

	m_Head = m_Tail = 0 ;

	return nFlushed;
	__LEAVE_FUNCTION
	return 0 ;
}

bool OutputStream::resize( int size )
{
	__ENTER_FUNCTION
	size = Max( size, (int)(m_nBufferLength>>1) ) ;

	int newBufferLen = m_nBufferLength+size ;
	int length = getLength( ) ;

	if( size<0 ) 
	{
		if( newBufferLen<0 || newBufferLen<length )
			return false ;
	} 

	char * newBuffer = new char[ newBufferLen ] ;
	if( newBuffer == NULL )
		return false ;

	if( m_Head<m_Tail ) 
	{
		memcpy( newBuffer, &m_pBuffer[m_Head], m_Tail-m_Head ) ;
	} 
	else if( m_Head>m_Tail ) 
	{
		memcpy( newBuffer, &m_pBuffer[m_Head] , m_nBufferLength-m_Head );
		memcpy( &newBuffer[m_nBufferLength-m_Head], m_pBuffer, m_Tail );
	}

	delete[] m_pBuffer;
	m_pBuffer = newBuffer;
	m_nBufferLength = newBufferLen;
	m_Head = 0;
	m_Tail = length;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

void OutputStream::cleanUp( )
{
	__ENTER_FUNCTION
	m_Head = 0 ;
	m_Tail = 0 ;
	__LEAVE_FUNCTION
}

} //namespace tcpnet 
