#include "player.h"
#include "socket.h"
#include "socketstream.h"
#include "packet.h"
#include "packetfactoryMgr.h"
#include "inputstream.h"
#include "outputstream.h"
#include "socketclient.h"
#include "assert.h"
#include "macroDef.h"
#include "playerPool.h"

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

WX_IMPLEMENT_SINGLEON(PlayerPool<Player>);

namespace tcpnet {

Player::Player( bool bSocketClient, bool bServer )
{
	__ENTER_FUNCTION

	m_pSocket = NULL ;
	m_pSocketStream	= NULL ;
	m_PlayerManagerID = -1;
	m_PlayerID	= -1;

	//�ṩ����,��ֱ��Ϊsocket
	if (bSocketClient)
	{
		m_pSocket = new SocketClient();
		Assert(m_pSocket);
	}
	else	//��Ϊ���ӷ�,��socketClient
	{
		m_pSocket = new Socket();
		Assert(m_pSocket);
	}
	m_pSocketStream = new SocketStream();
	Assert(m_pSocketStream);
	if (bServer)
	{
		bool bRet = m_pSocketStream->init(m_pSocket, 
			8*DEFAULT_SOCKETINPUT_BUFFERSIZE,
			8*MAX_SOCKETINPUT_BUFFERSIZE, 
			8*DEFAULT_SOCKETOUTPUT_BUFFERSIZE, 
			8*MAX_SOCKETOUTPUT_BUFFERSIZE);
		if (!bRet)
		{
			Assert(0);
		}
	}
	else
	{
		bool bRet = m_pSocketStream->init(m_pSocket);
		if (!bRet)
		{
			Assert(0);
		}
	}
	__LEAVE_FUNCTION
}

Player::~Player( )
{
	__ENTER_FUNCTION

	SAFE_DELETE( m_pSocket ) ;
	SAFE_DELETE( m_pSocketStream ) ;

	__LEAVE_FUNCTION
}

void Player::cleanUp( )
{
	__ENTER_FUNCTION
	
	if (m_pSocket)
	{
		m_pSocket->socketClose() ;
	}
	if (m_pSocketStream)
	{
		m_pSocketStream->cleanUp() ;
	}

	__LEAVE_FUNCTION
}

void Player::setSocket(SOCK s)
{
	__ENTER_FUNCTION
	if (m_pSocket)
	{
		m_pSocket->setSocket(s);
	}
	__LEAVE_FUNCTION
}

void Player::disconnect( )
{
	__ENTER_FUNCTION

	if (m_pSocket)
	{
		m_pSocket->socketClose() ;
	}

	__LEAVE_FUNCTION
}

bool Player::isValid( )
{
	__ENTER_FUNCTION

	if( m_pSocket == NULL )
		return false ;
	if( !m_pSocket->isSocketValid() ) 
		return false ;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Player::processInput( )
{
	__ENTER_FUNCTION
	Assert(m_pSocketStream);

	__MYTRY 
	{
		int nRet = m_pSocketStream->reveive( ) ;
		if( nRet <= SOCKET_ERROR )
		{
			//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "socket Reveive error: error code = %d, error description = %s", 
			//	GetSocketErrorCode(), GetSocketErrorDescription()) ;
			return false ;
		}
	} 
	__MYCATCH
	{
		//MyExceptionLog();
		return false ;
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Player::processCmd( )
{
	__ENTER_FUNCTION
	Assert(m_pSocketStream);
	InputStream* pInputStream = m_pSocketStream->getInputStream();
	Assert(pInputStream);
	PacketFactoryManager* pPacketFactoryManager = PacketFactoryManager::getSinglePtr();
	Assert(pPacketFactoryManager);

	Packet* pPacket = NULL;

	__MYTRY
	{
		////TGW ����
		//if (IsGamePlayer() && GetFlag())
		//{
		//	//���TGW��ͷ
		//	char packetTGW[PACKET_TGW_SIZE] = {0};
		//	if( !pInputStream->Peek(&packetTGW[0], PACKET_TGW_SIZE) )
		//	{
		//		return true; //��һ֡�ٴ���
		//	}
		//	pInputStream->Skip(PACKET_TGW_SIZE);
		//	SetFlag(false);
		//}
		
		for( int i=0;i<PACKET_COUNT_PER_TICK; i++ )
		{
			char packethead[PACKET_HEADER_SIZE];
			//�����Ϣͷ
			if( !pInputStream->peek(&packethead[0], PACKET_HEADER_SIZE) )
			{
				break ;	//��Ϣͷ�������
			}

			///Э���ͷ����
			#ifdef __ENCRYPT__
			{
				decryptHead_cs(packethead);
			}
			#endif

			Packet_ID packetID = PACKET_ID_INVALID;
			int packetuint = 0;
			int packetSize = 0;
			char packetEncrypt = 0;
			memcpy( &packetID, &packethead[0], sizeof(Packet_ID) ) ;	
			memcpy( &packetEncrypt, &packethead[sizeof(Packet_ID)], sizeof(char) );
			memcpy( &packetSize, &packethead[sizeof(Packet_ID)+sizeof(char)], sizeof(int) );

			///Э�����--Begin
			#ifdef __ENCRYPT__
			{
				int nSize = packetSize+PACKET_HEADER_SIZE;
				int nHead = pInputStream->GetHead();
				int nTail = pInputStream->GetTail();
				int nBufferLen = pInputStream->Capacity();
				char* szBuffer = pInputStream->GetBuffer();
				if ( nHead < nTail ) 
				{
					decrypt_cs(&szBuffer[nHead], nSize);
				}
				else 
				{
					int rightLen = nBufferLen - nHead;
					if( nSize <= rightLen ) 
					{
						decrypt_cs(&szBuffer[nHead], nSize);
					} 
					else
					{
						decrypt_cs(&szBuffer[nHead], rightLen);
						decrypt_cs(szBuffer, nSize-rightLen);
					}
				}
			}
			#endif
			///Э�����--End
			
			//packetID��Ч
			if(!pPacketFactoryManager->checkPacketID(packetID))
			{
				//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "PacketID is Invalid or PacketID is not Reg in factory, packetID = %d", packetID) ;
				bool bRet = pInputStream->skip(PACKET_HEADER_SIZE + packetSize);
				if (!bRet)
				{
				}
				return false ;
			}

			int nMaxPacketSize = pPacketFactoryManager->getPacketMaxSize(packetID);
			if (nMaxPacketSize<0)
			{
				return false ;
			}

			if(( packetSize > nMaxPacketSize)||(packetSize<0))
			{
				//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "PacketSize error, packetID=%d, packetsize is %d, maxpacketsize is %d!!!", 
				//	packetID, packetSize, nMaxPacketSize) ;
				return false ;
			}

			//��Ϣû�н���ȫ
			if( pInputStream->getLength()<PACKET_HEADER_SIZE+packetSize )
			{
				break;
			}

			pPacket = PacketFactoryManager::getSinglePtr()->createPacket( packetID ) ;
			if (pPacket == NULL)
			{
				return false;
			}

			__MYTRY
			{
				
				//��ȡ����
				bool bRet = m_pSocketStream->readPacket( pPacket ) ;
				//��ȡ��Ϣ���ݴ���
				if(( bRet == false ) || (pPacket->getSize() != packetSize))
				{
					//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "PacketSize error, packetID=%d, packetsize is %d, recvpacketsize is %d, maxpacketsize is %d!!!", 
					//	packetID, packetSize, pPacket->GetSize(), nMaxPacketSize) ;
					PacketFactoryManager::getSinglePtr()->removePacket( pPacket );
					return false ;
				}

				bool bNeedRemove = true ;
				bool bException = false ;
				int nRet = PACKET_EXE_ERROR;
				resetKickTime( );
				__MYTRY
				{
					nRet = pPacket->execute( this ) ;
				}
				__MYCATCH
				{
					nRet = PACKET_EXE_ERROR ;
				}
				//�����쳣���󣬶Ͽ����������
				if( nRet==PACKET_EXE_ERROR )
				{
					if( pPacket ) 
						PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
					return false ;
				}
				//��ǰ��Ϣ�Ľ���ִ�н�ֱֹͣ���¸�ѭ��ʱ�ż����Ի����е����ݽ�����Ϣ��ʽ����ִ�С�
				//����Ҫ���ͻ��˵�ִ�д�һ������ת�Ƶ�����һ������ʱ����Ҫ�ڷ���ת����Ϣ��ִ���ڱ��߳���ֹͣ��
				else if( nRet==PACKET_EXE_BREAK )
				{
					if( pPacket ) 
						PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
					break ;
				}
				//��������ʣ�µ���Ϣ
				else if( nRet==PACKET_EXE_CONTINUE )
				{
				}
				//��������ʣ�µ���Ϣ�����Ҳ����յ�ǰ��Ϣ
				else if( nRet==PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = false ;
				}
				else
				{//δ֪�ķ���ֵ
				}
				if( pPacket && bNeedRemove ) 
					PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
			}
			__MYCATCH
			{
				return false ;
			}
		}
	}
	__MYCATCH
	{
		return false ;
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool Player::processOutput( )
{
	__ENTER_FUNCTION
	Assert(m_pSocketStream);

	__MYTRY
	{
		int nSize = m_pSocketStream->getOutputStream()->getLength() ;
		if( nSize == 0 )
		{
			return true ;
		}

		int nRet = m_pSocketStream->send( ) ;
		if( nRet <= SOCKET_ERROR )
		{
			//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "Socket send error,error code = %d, error description = %s", 
			//	GetSocketErrorCode(), GetSocketErrorDescription()) ;
			return false ;
		}
	} 
	__MYCATCH
	{
		return false ;
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}


bool Player::sendPacket( Packet* pPacket )
{
	__ENTER_FUNCTION
	Assert(m_pSocketStream);
	Assert(pPacket);

	int nSizeBefore = m_pSocketStream->getOutputStream()->getLength();
	int nSizeAfter = 0;
	int nWriteSize = 0;
	bool bRet = false;
	__MYTRY
	{
		//��ѯ��ǰ��βλ�á���¼д��ǰλ��
		int nTail_Begin = m_pSocketStream->getOutputStream()->getTail();
		bRet = m_pSocketStream->writePacket(pPacket);
		Assert(bRet);
		//��ѯ��ǰ��βλ�á���¼д����λ��
		int nTail_End = m_pSocketStream->getOutputStream()->getTail();
		nSizeAfter = m_pSocketStream->getOutputStream()->getLength();
		nWriteSize = nSizeAfter-nSizeBefore;
		
		///Э�����--Begin
		#ifdef __ENCRYPT__
		{
			int nHead = m_pSocketStream->getOutputStream()->getHead();
			int nTail = m_pSocketStream->getOutputStream()->getTail();
			int nBufferLen = m_pSocketStream->getOutputStream()->capacity();
			char* szBuffer = m_pSocketStream->getOutputStream()->getBuffer();
			if ( nHead < nTail ) 
			{
				//int nSize = nTail_End - nTail_Begin;
				encrypt_sc(&(szBuffer[nTail_Begin]), nWriteSize);
			} 
			else
			{
				if (nTail_End < nTail_Begin)
				{
					int rightLen = nBufferLen - nTail_Begin;
					encrypt_sc(&(szBuffer[nTail_Begin]), rightLen);
					encrypt_sc(szBuffer, nWriteSize-rightLen);
				}
				else
				{
					encrypt_sc(&(szBuffer[nTail_Begin]), nWriteSize);
				}
			}
		}
		#endif
		///Э�����--End
	}
	__MYCATCH
	{
		return false ;
	}
	if(pPacket->getSize() != nWriteSize-PACKET_HEADER_SIZE)
	{
		//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "Packet Size Error! ID=%d, SIZE=%d, should=%d", pPacket->GetPacketID(), pPacket->GetSize(), nSizeAfter - nSizeBefore - PACKET_HEADER_SIZE);
	}

	return bRet ;
	__LEAVE_FUNCTION
	return false ;
}

} //namespace tcpnet
