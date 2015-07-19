#include "socketstream.h"
#include "inputstream.h"
#include "outputstream.h"
#include "assert.h"
#include "socketdef.h"

namespace tcpnet {

SocketStream::SocketStream() 
{
	__ENTER_FUNCTION

	m_pSocket = NULL;
	m_pInputStream = NULL;
	m_pOutputStream = NULL;

	__LEAVE_FUNCTION
}

SocketStream::~SocketStream( ) 
{
	__ENTER_FUNCTION

	delete[] m_pInputStream;
	delete[] m_pOutputStream;
	m_pSocket = NULL;
	m_pInputStream = NULL;
	m_pOutputStream = NULL;

	__LEAVE_FUNCTION
}

//≥ı ºªØ
bool	SocketStream::init(Socket* pSocket,
						   int inbufferSize, 
						   int inmaxBufferSize,
						   int outbufferSize, 
						   int outmaxBufferSize)
{
	__ENTER_FUNCTION
	if (pSocket ==NULL )
	{
		return false;
	}
	if ((inbufferSize<=0)&&(inmaxBufferSize<=0))
	{
		return false;
	}
	if ((outbufferSize<=0)&&(outmaxBufferSize<=0))
	{
		return false;
	}
	if ((inbufferSize>inmaxBufferSize)||(outbufferSize>outmaxBufferSize))
	{
		return false;
	}

	m_pSocket = pSocket;
	m_pInputStream = new InputStream();
	if (m_pInputStream == NULL)
	{
		return false;
	}
	m_pOutputStream = new OutputStream();
	if (m_pInputStream == NULL)
	{
		return false;
	}

	bool bRet = m_pInputStream->init(inbufferSize, inmaxBufferSize);
	if (!bRet)
	{
		return false;
	}
	bRet = m_pOutputStream->init(outbufferSize, outmaxBufferSize);
	if (!bRet)
	{
		return false;
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

int SocketStream::fill( char* buf, int length, EM_TYPE_MSG_MODE mode, int& nSize) 
{
	__ENTER_FUNCTION
	if ((m_pInputStream == NULL)||(m_pOutputStream == NULL))
	{
		return 0;
	}
	if ((mode != EM_TYPE_MSG_MODE_READ)&&(mode != EM_TYPE_MSG_MODE_WRITE))
	{
		return 0;
	}
	if ( buf == NULL )
		return 0 ;

	if ( length <= 0 )
		return 0 ;

	if (mode == EM_TYPE_MSG_MODE_READ)
	{
		length = m_pInputStream->read(buf, length);
		nSize = nSize + length;
	}
	else if (mode == EM_TYPE_MSG_MODE_WRITE)
	{
		length =m_pOutputStream->write(buf, length);
		nSize = nSize + length;
	}
	return length ;
	__LEAVE_FUNCTION
	return 0 ;
}

bool SocketStream::readPacket( Packet* pPacket ) 
{
	__ENTER_FUNCTION
	if ((pPacket == NULL)||(m_pInputStream == NULL))
	{
		return false;
	}

	return m_pInputStream->readPacket(*this, pPacket);

	__LEAVE_FUNCTION
	return false ;
}

bool SocketStream::writePacket( Packet* pPacket )
{
	__ENTER_FUNCTION
	if ((pPacket == NULL)||(m_pOutputStream == NULL))
	{
		return false;
	}

	return m_pOutputStream->writePacket(*this, pPacket);
	__LEAVE_FUNCTION
	return false ;
}

int SocketStream::reveive( )
{
	__ENTER_FUNCTION
	if ((m_pSocket == NULL)||(m_pInputStream == NULL))
	{
		return SOCKET_ERROR;
	}

	return m_pInputStream->reveive(m_pSocket);
	__LEAVE_FUNCTION
	return false ;
}

int SocketStream::send( )
{
	__ENTER_FUNCTION
	if ((m_pSocket == NULL)||(m_pOutputStream == NULL))
	{
		return SOCKET_ERROR;
	}

	return m_pOutputStream->flush(m_pSocket);
	__LEAVE_FUNCTION
	return false ;
}

void SocketStream::cleanUp( )
{
	__ENTER_FUNCTION

	m_pInputStream->cleanUp();
	m_pOutputStream->cleanUp();

	__LEAVE_FUNCTION
}

} //namespace tcpnet
