#include "inputstream.h"
#include "socketstream.h"
#include "assert.h"
#include "macroDef.h"
#include "packet.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace tcpnet {

extern int g_socketErrorCode;

InputStream::InputStream() 
{
	__ENTER_FUNCTION

	m_nBufferLength = 0 ;
	m_nMaxBufferLength = 0;
	m_Head = 0;
	m_Tail = 0;
	m_pBuffer = NULL;

	__LEAVE_FUNCTION
}

InputStream::~InputStream( ) 
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

//³õÊ¼»¯
bool	InputStream::init(int bufferSize, int maxBufferSize)
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

int InputStream::getLength( )const
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

int InputStream::read( char* buf, int length) 
{
	__ENTER_FUNCTION
	if ((buf == NULL)||(length <= 0))
	{
		return 0;
	}

	if ( length > getLength() )
		return 0 ;

	if ( m_Head < m_Tail ) 
	{
		memcpy( buf, &m_pBuffer[m_Head], length ) ;
	} 
	else 
	{
		uint rightLen = m_nBufferLength-m_Head ;
		if( (uint)length<=rightLen )
		{
			memcpy( buf, &m_pBuffer[m_Head], length ) ;
		} 
		else 
		{
			memcpy( buf, &m_pBuffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], m_pBuffer, length-rightLen ) ;
		}
	}
	m_Head = (m_Head+length)%m_nBufferLength ;
	return length ;
	__LEAVE_FUNCTION
	return 0 ;
}

bool	InputStream::readPacket(SocketStream& socketstream, Packet* pPacket )
{
	__ENTER_FUNCTION
	if (pPacket == NULL)
		return false;

	bool ret = skip( PACKET_HEADER_SIZE ) ;
	if( !ret )
		return false ;

	return pPacket->fill(socketstream, EM_TYPE_MSG_MODE_READ) ;
	__LEAVE_FUNCTION
	return false;
}

bool InputStream::peek( char* buf, int length ) 
{
	__ENTER_FUNCTION

	if ((buf == NULL)||(length <= 0))
	{
		return false;
	}

	if( length>getLength() )
		return false ;

	if( m_Head<m_Tail ) 
	{
		memcpy( buf , &m_pBuffer[m_Head] , length );

	} 
	else 
	{
		int rightLen = m_nBufferLength-m_Head ;
		if( length<=rightLen ) 
		{
			memcpy( &buf[0], &m_pBuffer[m_Head], length ) ;
		} 
		else 
		{
			memcpy( &buf[0], &m_pBuffer[m_Head], rightLen ) ;
			memcpy( &buf[rightLen], &m_pBuffer[0], length-rightLen ) ;
		}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool InputStream::skip( int length ) 
{
	__ENTER_FUNCTION
	if (length <= 0)
		return false ;

	if( length>getLength( ) )
		return false ;

	m_Head = (m_Head+length)%m_nBufferLength ;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

void InputStream::initsize( )
{
	__ENTER_FUNCTION
	m_Head = 0 ;
	m_Tail = 0 ;
	delete[] m_pBuffer;
	m_pBuffer = new char[DEFAULT_SOCKETINPUT_BUFFERSIZE];
	m_nBufferLength = DEFAULT_SOCKETINPUT_BUFFERSIZE ;
	memset( m_pBuffer, 0, m_nBufferLength ) ;
	__LEAVE_FUNCTION
}

int InputStream::reveive( Socket* pSocket ) 
{
	__ENTER_FUNCTION
	if (pSocket == NULL)
		throw 1;

	int nFilled = 0 ;
	int nReceived = 0 ;
	int nFree = 0 ;
	if ( m_Head <= m_Tail ) 
	{
		if ( m_Head == 0 ) 
		{
			nReceived = 0 ;
			nFree = m_nBufferLength-m_Tail-1 ;
			if( nFree != 0 )
			{
				nReceived = pSocket->receive( &m_pBuffer[m_Tail] , nFree );
				if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
				{
					return 0 ; 
				}
				if (nReceived==0)
				{
					return EM_SOCKET_ERROR_STEP0;
				}
				if (nReceived<0)
				{
					return EM_SOCKET_ERROR_STEP1;
				}
				m_Tail += nReceived;
				nFilled += nReceived;
			}

			if( nReceived == nFree ) 
			{
				uint available = pSocket->available();
				if ( available > 0 ) 
				{
					if( (m_nBufferLength+available+1)>m_nMaxBufferLength )
					{
						initsize( ) ;
						g_socketErrorCode = 0;
						return EM_SOCKET_ERROR_STEP2;
					}
					if( !resize( available+1 ) )
						return 0 ;

					nReceived = pSocket->receive( &m_pBuffer[m_Tail] , available );
					if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
					{
						return 0 ; 
					}
					if (nReceived==0)
					{
						return EM_SOCKET_ERROR_STEP3;
					}
					if (nReceived<0)
					{
						return EM_SOCKET_ERROR_STEP4;
					}

					m_Tail += nReceived;
					nFilled += nReceived;
				}
			}
		} 
		else 
		{
			nFree = m_nBufferLength-m_Tail ;
			nReceived = pSocket->receive( &m_pBuffer[m_Tail], nFree );
			if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
			{
				return 0 ; 
			}
			if (nReceived==0)
			{
				return EM_SOCKET_ERROR_STEP5;
			}
			if (nReceived<0)
			{
				return EM_SOCKET_ERROR_STEP6;
			}

			m_Tail = (m_Tail+nReceived)%m_nBufferLength ;
			nFilled += nReceived ;

			if( nReceived==nFree ) 
			{
				nReceived = 0 ;
				nFree = m_Head-1 ;
				if( nFree!=0 )
				{
					nReceived = pSocket->receive( &m_pBuffer[0] , nFree );
					if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
					{
						return 0 ; 
					}
					if (nReceived==0)
					{
						return EM_SOCKET_ERROR_STEP7;
					}
					if (nReceived<0)
					{
						return EM_SOCKET_ERROR_STEP8;
					}

					m_Tail += nReceived;
					nFilled += nReceived;
				}

				if( nReceived==nFree ) 
				{
					uint available = pSocket->available();
					if ( available > 0 ) 
					{
						if( (m_nBufferLength+available+1)>m_nMaxBufferLength )
						{
							initsize( ) ;
							g_socketErrorCode = 0;
							return EM_SOCKET_ERROR_STEP9;
						}
						if( !resize( available+1 ) )
							return 0 ;

						nReceived = pSocket->receive( &m_pBuffer[m_Tail] , available );
						if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
						{
							return 0 ; 
						}
						if (nReceived==0)
						{
							return EM_SOCKET_ERROR_STEP10;
						}
						if (nReceived<0)
						{
							return EM_SOCKET_ERROR_STEP11;
						}

						m_Tail += nReceived;
						nFilled += nReceived;
					}
				}
			}
		}

	} 
	else 
	{	
		nReceived = 0 ;
		nFree = m_Head-m_Tail-1 ;
		if( nFree!=0 )
		{
			nReceived = pSocket->receive( &m_pBuffer[m_Tail], nFree ) ;
			if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
			{
				return 0 ; 
			}
			if (nReceived==0)
			{
				return EM_SOCKET_ERROR_STEP12;
			}
			if (nReceived<0)
			{
				return EM_SOCKET_ERROR_STEP13;
			}
			m_Tail += nReceived ;
			nFilled += nReceived ;
		}
		if( nReceived==nFree ) 
		{
			uint available = pSocket->available( ) ;
			if ( available>0 ) 
			{
				if( (m_nBufferLength+available+1)>m_nMaxBufferLength )
				{
					initsize( ) ;
					g_socketErrorCode = 0;
					return EM_SOCKET_ERROR_STEP14;
				}
				if( !resize( available+1 ) )
					return 0 ;

				nReceived = pSocket->receive( &m_pBuffer[m_Tail], available ) ;
				if (nReceived==SOCKET_ERROR_WOULD_BLOCK) 
				{
					return 0 ; 
				}
				if (nReceived==0)
				{
					return EM_SOCKET_ERROR_STEP15;
				}
				if (nReceived<0)
				{
					return EM_SOCKET_ERROR_STEP16;
				}

				m_Tail += nReceived ;
				nFilled += nReceived ;
			}
		}
	}
	return nFilled ;
	__LEAVE_FUNCTION
	return 0 ;
}

bool InputStream::resize( int size )
{
	__ENTER_FUNCTION

	size = Max( size, (int)(m_nBufferLength>>1) ) ;
	int newBufferLen = m_nBufferLength + size;
	int length = getLength();

	if ( size < 0 ) 
	{
		if ( newBufferLen < 0 || newBufferLen < length )
			return false ;		
	} 

	char * newBuffer = new char[ newBufferLen ];

	if ( m_Head < m_Tail ) 
	{
		memcpy( newBuffer , &m_pBuffer[m_Head] , m_Tail - m_Head );
	} 
	else if ( m_Head > m_Tail ) 
	{
		memcpy( newBuffer , &m_pBuffer[m_Head] , m_nBufferLength - m_Head );
		memcpy( &newBuffer[ m_nBufferLength - m_Head ] , m_pBuffer , m_Tail );
	}

	delete [] m_pBuffer ;

	m_pBuffer = newBuffer ;
	m_nBufferLength = newBufferLen ;
	m_Head = 0 ;
	m_Tail = length ;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

void InputStream::cleanUp( )
{
	__ENTER_FUNCTION
	m_Head = 0 ;
	m_Tail = 0 ;
	__LEAVE_FUNCTION
}

} //namespace tcpnet 
