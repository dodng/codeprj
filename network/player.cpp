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

	//提供服务方,则直接为socket
	if (bSocketClient)
	{
		m_pSocket = new SocketClient();
		Assert(m_pSocket);
	}
	else	//作为连接方,需socketClient
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
		////TGW 改造
		//if (IsGamePlayer() && GetFlag())
		//{
		//	//检查TGW包头
		//	char packetTGW[PACKET_TGW_SIZE] = {0};
		//	if( !pInputStream->Peek(&packetTGW[0], PACKET_TGW_SIZE) )
		//	{
		//		return true; //下一帧再处理
		//	}
		//	pInputStream->Skip(PACKET_TGW_SIZE);
		//	SetFlag(false);
		//}
		
		for( int i=0;i<PACKET_COUNT_PER_TICK; i++ )
		{
			char packethead[PACKET_HEADER_SIZE];
			//检查消息头
			if( !pInputStream->peek(&packethead[0], PACKET_HEADER_SIZE) )
			{
				break ;	//消息头不能填充
			}

			///协议包头解密
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

			///协议解密--Begin
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
			///协议解密--End
			
			//packetID无效
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

			//消息没有接收全
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
				
				//读取数据
				bool bRet = m_pSocketStream->readPacket( pPacket ) ;
				//读取消息内容错误
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
				//出现异常错误，断开此玩家连接
				if( nRet==PACKET_EXE_ERROR )
				{
					if( pPacket ) 
						PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
					return false ;
				}
				//当前消息的解析执行将停止直到下个循环时才继续对缓存中的数据进行消息格式化和执行。
				//当需要将客户端的执行从一个场景转移到另外一个场景时：需要在发送转移消息后将执行在本线程中停止。
				else if( nRet==PACKET_EXE_BREAK )
				{
					if( pPacket ) 
						PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
					break ;
				}
				//继续解析剩下的消息
				else if( nRet==PACKET_EXE_CONTINUE )
				{
				}
				//继续解析剩下的消息，并且不回收当前消息
				else if( nRet==PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = false ;
				}
				else
				{//未知的返回值
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
		//查询当前包尾位置。记录写包前位置
		int nTail_Begin = m_pSocketStream->getOutputStream()->getTail();
		bRet = m_pSocketStream->writePacket(pPacket);
		Assert(bRet);
		//查询当前包尾位置。记录写包后位置
		int nTail_End = m_pSocketStream->getOutputStream()->getTail();
		nSizeAfter = m_pSocketStream->getOutputStream()->getLength();
		nWriteSize = nSizeAfter-nSizeBefore;
		
		///协议加密--Begin
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
		///协议加密--End
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
