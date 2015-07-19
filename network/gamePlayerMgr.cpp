#include "gamePlayerMgr.h"
#include "gamePlayer.h"
#include "playerPool.h"
#include "math.h"
#include "packetfactoryMgr.h"
#include <stdlib.h>
#include "timeSystem.h"
#include "socketdef.h"

const int DEFAULT_CLIENT_SEND_BUFFER_SIZE = 64*1024;
const int DEFAULT_CLIENT_RECV_BUFFER_SIZE = 64*1024;

WX_IMPLEMENT_SINGLEON(GamePlayerManager);

GamePlayerManager::GamePlayerManager()
{
	__ENTER_FUNCTION

	m_PacketQueue = new PACKET_CACHE[MAX_PACKET_CACHE_SIZE];
	Assert(m_PacketQueue) ;
	m_PacketQueueSize = MAX_PACKET_CACHE_SIZE ;
	m_Head = 0 ;
	m_Tail = 0 ;

	__LEAVE_FUNCTION
}

GamePlayerManager::~GamePlayerManager()
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY( m_PacketQueue );
	SAFE_DELETE( m_pServerSocket ) ;
	__LEAVE_FUNCTION
}

bool GamePlayerManager::init(int nCout, const char* host, int port)
{
	__ENTER_FUNCTION
	bool bRet = ObjectManager<GamePlayer>::init(nCout);
	Assert( bRet ) ;

	m_pServerSocket = new  SocketServer() ;
	Assert( m_pServerSocket ) ;
	
	bRet =  m_pServerSocket->create(host, port);
	Assert( bRet ) ;

	bRet = m_pServerSocket->bind() ;
	Assert( bRet ) ;

	bRet = m_pServerSocket->listen( 5 ) ;
	Assert( bRet ) ;

	bRet = m_pServerSocket->setNonBlocking() ;
	Assert( bRet ) ;
	
	m_ServerSocket = m_pServerSocket->getSocket();

	FD_SET(m_ServerSocket , &m_ReadFDs[EM_SELECT_BAK]);
	FD_SET(m_ServerSocket , &m_WriteFDs[EM_SELECT_BAK]);
	FD_SET(m_ServerSocket , &m_ExceptFDs[EM_SELECT_BAK]);
	m_Timeout[EM_SELECT_BAK].tv_sec = 0;
	m_Timeout[EM_SELECT_BAK].tv_usec = 0;
	m_MinFD = m_MaxFD = m_ServerSocket;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::addPlayer( GamePlayer* pPlayer )
{
	__ENTER_FUNCTION

	Assert(getObjNum() <= 1024 /*&&
        && getObjNum() <= ConfigManager::GetInstance()->m_PoolSizeConfig.m_nMaxPlayer*/);
	
	return PlayerManager::addPlayer(pPlayer);

	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::acceptNewConnection( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	//从玩家池中找出一个空闲的玩家数据集
	GamePlayer* pClient = PlayerPool<GamePlayer>::getSinglePtr()->allocPlayer() ;
	if( pClient == NULL )
	{
		return false ;
	}
	Socket* pSocket = pClient->getSocket();
	Assert(pSocket);

	__MYTRY
	{
		//接受客户端接入Socket句柄
		bRet = m_pServerSocket->accept( pClient->getSocket() ) ;
		if( !bRet )
		{
			//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "GamePlayerManager AcceptNewConnection Accept error: error code = %d, error description = %s", 
			//	GetSocketErrorCode(), GetSocketErrorDescription()) ;
			goto EXCEPTION ;
		}

		SOCK mysocket = pSocket->getSocket();
		if( mysocket == INVALID_SOCKET )
		{
			//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "GamePlayerManager AcceptNewConnection socket invalid") ;
			goto EXCEPTION ;
		}

		bRet = pSocket->setNonBlocking() ;
		if( !bRet )
		{
			goto EXCEPTION ;
		}

		bRet = pSocket->setSendBufferSize(DEFAULT_CLIENT_SEND_BUFFER_SIZE);
		if( !bRet )
		{
			goto EXCEPTION ;
		}

		bRet = pSocket->setReceiveBufferSize(DEFAULT_CLIENT_RECV_BUFFER_SIZE);
		if (!bRet)
		{
			goto EXCEPTION ;
		}

		bRet = pSocket->setNonDelay() ;
		if( !bRet )
		{	
			goto EXCEPTION ;
		}

		if( pSocket->isSockError() )
		{
			goto EXCEPTION ;
		}

		bRet = pSocket->setLinger(0) ;
		if( !bRet )
		{	
			goto EXCEPTION ;
		}
		//初始化基本玩家信息
		bRet = pClient->init( ) ;
		if( !bRet )
		{
			//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "GamePlayerManager AcceptNewConnection client init error ") ;
			goto EXCEPTION ;
		}
		//设置当前客户端连接的状态
		pClient->setStatus( EM_PLAYER_STATUS_CONNECT ) ;
		bRet = addPlayer( pClient ) ;
		Assert(bRet);
	}
	__MYCATCH
	{
		//MyExceptionLog( ) ;
		goto EXCEPTION ;
	}

	//Log::GetInstance()->SaveSystemLog("GamePlayerManager AcceptNewConnection(SOCKET=%d, IP=%s) PlayerID=%d ...OK", 
	//	pSocket->GetSocket(), pSocket->GetIp(), pClient->GetPlayerID()) ;
	//com_writelog(COMLOG_NOTICE, "GamePlayerManager::AcceptNewConnection, playerid=%d\n",pClient->GetPlayerID());

	return true ;

EXCEPTION:
	bRet = PlayerPool<GamePlayer>::getSinglePtr()->recyclePlayer( pClient->getPlayerID() ) ;
	return false ;
	__LEAVE_FUNCTION
	return false ;
}

void	GamePlayerManager::getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD)
{
	__ENTER_FUNCTION

	maxFD = minFD = m_ServerSocket;
	for( int i=0; i<getObjNum(); i++ )
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
		}
		Socket* pPlayerSocket = pPlayer->getSocket();
		if (NULL == pPlayerSocket)
		{
			continue;
		}
		SOCK playerSocket = pPlayerSocket->getSocket();
		if (removedFD == playerSocket)
		{
			continue;
		}
		if( maxFD < playerSocket )
		{
			maxFD = playerSocket ;
		}
		if (minFD > playerSocket)
		{
			minFD = playerSocket;
		}		
	}	
	__LEAVE_FUNCTION
}

bool GamePlayerManager::processInputs( )
{
	__ENTER_FUNCTION

	//no player
	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{ 
		return true ;
	}
	__MYTRY
	{
		//新连接接入：
		if( FD_ISSET(m_ServerSocket, &m_ReadFDs[EM_SELECT_USE]) )
		{
			for( int i=0; i<32; i++ )
			{
				if( !acceptNewConnection() )
					break;
			}
		}
	}
	__MYCATCH
	{}

	return PlayerManager::processInputs( );

	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::processCmds( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{ 
		return true ;
	}

	for( int i=0;i<getObjNum();++i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
		}
		if (!pPlayer->isValid())
		{
			removeSocket(pPlayer);
			removeObj(pPlayer);
			continue;
		}
		Socket* pSocket = pPlayer->getSocket();
		if (NULL == pSocket)
		{
			continue;
		}
		SOCK s = pSocket->getSocket();
		if( s == m_ServerSocket)
		{
			continue ;
		}
		if(pSocket->isSockError())
		{
			kickPlayer(pPlayer);
		}
		else
		{
			__MYTRY
			{
				bRet = pPlayer->processCmd( ) ;
				if (!bRet)
				{
					kickPlayer(pPlayer);
				}
			}
			__MYCATCH
			{
				kickPlayer( pPlayer );
			}
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::recivePacket( Packet*& pPacket, int& PlayerID, bool& bRemove )
{
	__ENTER_FUNCTION

	AutoMLock autolock(m_Lock);
	//没有消息
	if( m_PacketQueue[m_Head].m_pPacket==NULL )
	{
		return false ;
	}

	pPacket  = m_PacketQueue[m_Head].m_pPacket ;
	PlayerID = m_PacketQueue[m_Head].m_nPlayerID ;
	bRemove  = m_PacketQueue[m_Head].m_bRemove ;

	m_PacketQueue[m_Head].m_pPacket	= NULL ;
	m_PacketQueue[m_Head].m_nPlayerID = -1;
	m_PacketQueue[m_Head].m_bRemove	= false ;

	++m_Head;
	if( m_Head>=m_PacketQueueSize ) 
		m_Head = 0 ;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::removePacket( int PlayerID )
{
	__ENTER_FUNCTION

	AutoMLock autolock(m_Lock);
	int Cur = m_Head ;

	for( int i=0; i<m_PacketQueueSize; i++ )
	{
		if( m_PacketQueue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQueue[Cur].m_nPlayerID == PlayerID )
		{
			m_PacketQueue[Cur].m_bRemove = true ;
		}

		Cur ++ ;
		if( Cur>=m_PacketQueueSize ) Cur = 0 ;
	}
	return true ;

	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::sendPacket( Packet* pPacket, int PlayerID, bool bRemove )
{
	__ENTER_FUNCTION

	AutoMLock autolock(m_Lock);
	//缓冲区满
	if (m_PacketQueue[m_Tail].m_pPacket != NULL)
	{
		bool ret = resizeCache( ) ;
		Assert( ret ) ;
	}
	m_PacketQueue[m_Tail].m_pPacket = pPacket ;
	m_PacketQueue[m_Tail].m_nPlayerID = PlayerID ;
	m_PacketQueue[m_Tail].m_bRemove = bRemove ;
	++m_Tail;

	if (m_Tail>=m_PacketQueueSize)
	{
		m_Tail = 0;
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::resizeCache( )
{
	__ENTER_FUNCTION
	//一定有数据
	Assert(m_PacketQueue[m_Tail].m_pPacket != NULL);
	PACKET_CACHE* pNew = new PACKET_CACHE[m_PacketQueueSize + MAX_PACKET_CACHE_SIZE];
	if( pNew==NULL )
		return false ;

	if(m_Head < m_Tail)
		memcpy(pNew, &(m_PacketQueue[m_Head]), sizeof(PACKET_CACHE)*(m_Tail - m_Head) );
	else if(m_Head >= m_Tail)
	{
		memcpy(pNew, &(m_PacketQueue[m_Head]), sizeof(PACKET_CACHE)*(m_PacketQueueSize - m_Head) );
		memcpy(&pNew[m_PacketQueueSize - m_Head], m_PacketQueue, sizeof(PACKET_CACHE)*m_Tail );
	}

	memset( m_PacketQueue, 0, sizeof(PACKET_CACHE)*m_PacketQueueSize );
	SAFE_DELETE_ARRAY( m_PacketQueue ) ;
	m_PacketQueue = pNew ;

	m_Head = 0 ;
	m_Tail = m_PacketQueueSize ;
	m_PacketQueueSize = m_PacketQueueSize+MAX_PACKET_CACHE_SIZE ;

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::processCacheCmds()
{
	__ENTER_FUNCTION
	Packet* pPacket = NULL ;
	int nPlayerID = -1;
	bool bRemove = false;
	for( int i=0; i<m_PacketQueueSize; i++ )
	{
		bool bRet = recivePacket( pPacket, nPlayerID, bRemove ) ;
		if( !bRet )
		{
			return true;
		}
		Assert( pPacket ) ;
		if( bRemove)
		{
			bRet = PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
			continue;
		}
		bool bNeedRemove = true ;
		if( nPlayerID==-1 )
		{
			__MYTRY
			{
				pPacket->execute(NULL) ;
			}
			__MYCATCH
			{}
		}
		else
		{
			__MYTRY
			{
				GamePlayer* pPlayer = PlayerPool<GamePlayer>::getSinglePtr()->getPlayer( nPlayerID ) ;
				//这种情况存在
				if (pPlayer != NULL)
				{
					int nRet = PACKET_EXE_ERROR ;
					__MYTRY
					{
						nRet = pPacket->execute(pPlayer) ;
					}
					__MYCATCH
					{
						nRet = PACKET_EXE_ERROR ;
					}

					if( nRet == PACKET_EXE_ERROR )
					{
						kickPlayer( pPlayer ) ;
					}
					else if( nRet == PACKET_EXE_BREAK )
					{
					}
					else if( nRet == PACKET_EXE_CONTINUE )
					{
					}
					else if( nRet == PACKET_EXE_NOTREMOVE )
					{
						bNeedRemove = false ;
					}
				}
			}
			__MYCATCH
			{}
		}

		//回收消息
		if( bNeedRemove )
			PacketFactoryManager::getSinglePtr()->removePacket( pPacket ) ;
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

bool GamePlayerManager::kickPlayer( GamePlayer* pPlayer )
{
	__ENTER_FUNCTION
	Assert(pPlayer);

	int nPlayerID = pPlayer->getPlayerID();
	SOCK fd = pPlayer->getSocket()->getSocket() ;
	char* szIP = pPlayer->getSocket()->getIp();
	//Log::GetInstance()->SaveSystemLog("GamePlayerManager KickPlayer(SOCKET=%d, IP=%s), PlayerID=%d, UserID=%s", fd, szIP, nPlayerID, pPlayer->GetUserID()) ;

	removePacket(nPlayerID);
	removePlayer(nPlayerID);

	//清除在池中的信息
	bool bRet = pPlayer->free() ;
	Assert(bRet) ;
	
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

void GamePlayerManager::relive()
{
   __ENTER_FUNCTION
  // Log::GetInstance()->SaveSystemLog("GamePlayerManager Relive!!!");

   Assert(m_pServerSocket);
   Assert(m_ServerSocket>=0);
   kickAllPlayer();
   m_Head = 0 ;
   m_Tail = 0 ;
   FD_ZERO( &m_ReadFDs[EM_SELECT_BAK] ) ;
   FD_ZERO( &m_WriteFDs[EM_SELECT_BAK] ) ;
   FD_ZERO( &m_ExceptFDs[EM_SELECT_BAK] ) ;
   FD_ZERO( &m_ReadFDs[EM_SELECT_USE] ) ;
   FD_ZERO( &m_WriteFDs[EM_SELECT_USE] ) ;
   FD_ZERO( &m_ExceptFDs[EM_SELECT_USE] ) ;
   m_Timeout[EM_SELECT_BAK].tv_sec = 0;
   m_Timeout[EM_SELECT_BAK].tv_usec = 0;

   FD_SET(m_ServerSocket , &m_ReadFDs[EM_SELECT_BAK]);
   FD_SET(m_ServerSocket , &m_WriteFDs[EM_SELECT_BAK]);
   FD_SET(m_ServerSocket , &m_ExceptFDs[EM_SELECT_BAK]);

   m_Timeout[EM_SELECT_BAK].tv_sec = 0;
   m_Timeout[EM_SELECT_BAK].tv_usec = 0;
   m_MinFD = m_MaxFD = m_ServerSocket;
   
   __LEAVE_FUNCTION
}

