#include "playerMgr.h"
#include "gamePlayer.h"
#include "playerPool.h"
#include "timeSystem.h"
#include "outputstream.h"
#include "socketAPI.h"

using namespace tcpnet;

PlayerManager::PlayerManager()
{
	FD_ZERO( &m_ReadFDs[EM_SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[EM_SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[EM_SELECT_BAK] ) ;
	FD_ZERO( &m_ReadFDs[EM_SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[EM_SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[EM_SELECT_USE] ) ;
	m_Timeout[EM_SELECT_BAK].tv_sec = 0;
	m_Timeout[EM_SELECT_BAK].tv_usec = 0;
	m_MinFD = m_MaxFD = INVALID_SOCKET ;
}

//清理
void PlayerManager::cleanUp( )
{
    __ENTER_FUNCTION
	kickAllPlayer( );
	ObjectManager<GamePlayer>::cleanUp( ) ;
	FD_ZERO( &m_ReadFDs[EM_SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[EM_SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[EM_SELECT_BAK] ) ;
	FD_ZERO( &m_ReadFDs[EM_SELECT_USE] ) ;
	FD_ZERO( &m_WriteFDs[EM_SELECT_USE] ) ;
	FD_ZERO( &m_ExceptFDs[EM_SELECT_USE] ) ;
	m_Timeout[EM_SELECT_BAK].tv_sec = 0;
	m_Timeout[EM_SELECT_BAK].tv_usec = 0;
	m_MinFD = m_MaxFD = INVALID_SOCKET ;
    __LEAVE_FUNCTION
}

//网络侦测
bool PlayerManager::select( )
{
	__ENTER_FUNCTION
	bool bRet = false ;
	if( m_MaxFD==INVALID_SOCKET && m_MinFD==INVALID_SOCKET )
	{
		return true ;
	}

	//从备份取出使用fd
	m_Timeout[EM_SELECT_USE].tv_sec  = m_Timeout[EM_SELECT_BAK].tv_sec;
	m_Timeout[EM_SELECT_USE].tv_usec = m_Timeout[EM_SELECT_BAK].tv_usec;

	m_ReadFDs[EM_SELECT_USE]   = m_ReadFDs[EM_SELECT_BAK];
	m_WriteFDs[EM_SELECT_USE]  = m_WriteFDs[EM_SELECT_BAK];
	m_ExceptFDs[EM_SELECT_USE] = m_ExceptFDs[EM_SELECT_BAK];

	__MYTRY 
	{
		int iRet = tcpnet::SocketAPI::socket_select((int)m_MaxFD+1 , 
			&m_ReadFDs[EM_SELECT_USE] , 
			&m_WriteFDs[EM_SELECT_USE] , 
			&m_ExceptFDs[EM_SELECT_USE] , 
			&m_Timeout[EM_SELECT_USE] ) ;
		if (iRet == SOCKET_ERROR)
		{
			//Log::GetInstance()->CacheLog(LOG_FILE_ERROR, "ScenePlayerManager select error: error code = %d, error description = %s", 
			//	GetSocketErrorCode(), GetSocketErrorDescription()) ;
			return false;
		}
	} 
	__MYCATCH
	{}

	return true ;
	__LEAVE_FUNCTION
	return false;
}

//数据接受
bool PlayerManager::processInputs( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	//no player
	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{ 
		return true ;
	}

	//数据读取
	int nPlayerCount = getObjNum() ;
	for( int i=nPlayerCount-1; i>=0; --i )
	{
		if( m_pObjectList[i] == NULL )
			continue ;
		GamePlayer* pPlayer = getObjByIndex(i) ;
		Assert(pPlayer);	//严重错误,不再向下执行,这种情况也基本不可能

		if (!pPlayer->isValid())
		{
			m_pObjectList[i] = NULL;
			removeSocket(pPlayer);
			removeObj(pPlayer);
			continue;
			//Assert(0) ;
		}
		SOCK s = pPlayer->getSocket()->getSocket();
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_ReadFDs[EM_SELECT_USE] ) )
		{
			//连接出现错误
			if( pPlayer->getSocket()->isSockError() )
			{
				bRet = kickPlayer( pPlayer ) ;
				Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
			}
			else
			{
				__MYTRY
				{
					bRet = pPlayer->processInput( ) ;
					if (!bRet)
					{
						throw(1);
					}
				}
				__MYCATCH
				{
					bRet = kickPlayer( pPlayer ) ;
					Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
				}
			}
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//数据发送
bool PlayerManager::processOutputs( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	//no player
	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{ 
		return true ;
	}

	//数据发送
	int nPlayerCount = getObjNum() ;
	for( int i=nPlayerCount-1; i>=0; --i )
	{
		if( m_pObjectList[i] == NULL )
		{
			continue ;
		}
		GamePlayer* pPlayer = getObjByIndex(i) ;
		Assert(pPlayer);	//严重错误,不再向下执行,这种情况也基本不可能

		if (!pPlayer->isValid())
		{
			m_pObjectList[i] = NULL;
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
		Assert( s!=INVALID_SOCKET ) ;

		if( FD_ISSET( s, &m_WriteFDs[EM_SELECT_USE] ) )
		{
			//连接出现错误
			if( pSocket->isSockError() )
			{
				bRet = kickPlayer( pPlayer ) ;
				Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
			}
			else
			{
				__MYTRY
				{
					int size1 = pPlayer->getSocketStream()->getOutputStream()->getLength();
					bRet = pPlayer->processOutput( ) ;
					int size2 = pPlayer->getSocketStream()->getOutputStream()->getLength();
					int size = size1 - size2;
					/*com_writelog(COMLOG_NOTICE, "ScenePlayerManager::ProcessOutputs, channelid=%s, sendsize=%d\n", 
						pPlayer->GetChannelId(), size);*/

					if (!bRet)
					{
						throw(1);
					}
				}
				__MYCATCH
				{
					bRet = kickPlayer( pPlayer ) ;
					Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
				}
			}
		}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//异常连接处理
bool PlayerManager::processExceptions( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	//no player
	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{ 
		return true ;
	}
	for( int i=0; i<getObjNum(); ++i )
	{
		//某个玩家断开网络连接
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
			//Assert(pPlayer);	//严重错误,不再向下执行,这种情况也基本不可能
		}
		if (!pPlayer->isValid())
		{
			m_pObjectList[i] = NULL;
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
		Assert( s!=INVALID_SOCKET ) ;
		if( FD_ISSET( s , &m_ExceptFDs[EM_SELECT_USE] ) )
		{
			bRet = kickPlayer( pPlayer ) ;
			Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
		}
	}
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//消息执行
bool PlayerManager::processCmds( )
{
	__ENTER_FUNCTION

	bool bRet = false ;
	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET)
	{ 
		return true ;
	}
	for( int i=0; i<getObjNum(); ++i )
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
			//Assert(pPlayer);	//严重错误,不再向下执行,这种情况也基本不可能
		}
		if (!pPlayer->isValid())
		{
			m_pObjectList[i] = NULL;
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
		Assert( s!=INVALID_SOCKET ) ;

		//连接出现错误
		if( pSocket->isSockError() )
		{
			bRet = kickPlayer( pPlayer ) ;
			Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
		}
		else
		{
			__MYTRY
			{
				bRet = pPlayer->processCmd( ) ;
				if (!bRet)
				{
					throw(1);
				}
			}
			__MYCATCH
			{
				bRet = kickPlayer( pPlayer ) ;
				Assert(bRet);	//严重错误,不再向下执行,这种情况也基本不可能
			}
		}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}

//逻辑
bool PlayerManager::heartBeat( uint uTime )
{
	__ENTER_FUNCTION

	//设定此桢的时间
	TimeSystem::getSinglePtr()->getThisTime();
	uTime = TimeSystem::getSinglePtr()->getCurrRunTime() ;
	for( int i=0;i<getObjNum();++i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (NULL == pPlayer)
		{
			continue;
		}
		__MYTRY
		{
			bool bRet = pPlayer->heartBeat(uTime,1 ) ;
			if (!bRet)
			{
			  kickPlayer( pPlayer );
			}
		}
		__MYCATCH
		{}
	}

	return true ;
	__LEAVE_FUNCTION
	return false ;
}
//将Player数据加入系统中
bool PlayerManager::addPlayer( GamePlayer* pPlayer )
{
	__ENTER_FUNCTION
		//已经超出能够检测的网络句柄最大数
		if( getObjNum() >= FD_SETSIZE )
		{
			return false ;
		}
		Socket* pSocket = pPlayer->getSocket();
		Assert(pSocket);

		SOCK fd = pSocket->getSocket();
		Assert( fd != INVALID_SOCKET ) ;
		if( FD_ISSET(fd,&m_ReadFDs[EM_SELECT_BAK]) )
		{
			return false ;
		}
		bool ret = addObj( pPlayer ) ;
		if( !ret )
		{
			return false ;
		}

		m_MinFD = Min(fd , m_MinFD);
		m_MaxFD = Max(fd , m_MaxFD);
		FD_SET(fd , &m_ReadFDs[EM_SELECT_BAK]);
		FD_SET(fd , &m_WriteFDs[EM_SELECT_BAK]);
		FD_SET(fd , &m_ExceptFDs[EM_SELECT_BAK]);

	    return true ;
	__LEAVE_FUNCTION
		return false ;
}

//删除一个玩家（并不断网，有可能转移到别的场景）
bool PlayerManager::removePlayer( int nPlayerID )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = PlayerPool<GamePlayer>::getSinglePtr()->getPlayer(nPlayerID) ;
	if (NULL == pGamePlayer)
	{
		return false;
	}
	removeSocket(pGamePlayer);
	removeObj( pGamePlayer );
	return true ;
	__LEAVE_FUNCTION
	return false ;
}
bool PlayerManager::removeSocket( GamePlayer* pPlayer )
{
	__ENTER_FUNCTION

	if (NULL==pPlayer)
		return true;

	Socket* pPlayerSocket = pPlayer->getSocket();
	if (NULL==pPlayerSocket)
		return true;

	SOCK removedFD = pPlayerSocket->getSocket();
	if (INVALID_SOCKET==removedFD)
		return true;

	if (removedFD==m_MinFD || removedFD == m_MaxFD)
	{
		getMaxMinFD(removedFD,m_MaxFD,m_MinFD);
	}

	FD_CLR(removedFD , &m_ReadFDs[EM_SELECT_BAK]);
	FD_CLR(removedFD , &m_ReadFDs[EM_SELECT_USE]);
	FD_CLR(removedFD , &m_WriteFDs[EM_SELECT_BAK]);
	FD_CLR(removedFD , &m_WriteFDs[EM_SELECT_USE]);
	FD_CLR(removedFD , &m_ExceptFDs[EM_SELECT_BAK]);
	FD_CLR(removedFD , &m_ExceptFDs[EM_SELECT_USE]);

	return true;
	__LEAVE_FUNCTION
	return false;
}

//将所有player信息清理,并断开连接
void PlayerManager::kickAllPlayer( )
{
	__ENTER_FUNCTION
	//AutoLock autolock(m_Lock);
	int nPlayerCount = getObjNum() ;
	for( int i=0; i<nPlayerCount; i++ )
	{
		if( m_pObjectList[i] == NULL )
			continue ;
		GamePlayer* pPlayer = getObjByIndex(i);
		if( pPlayer==NULL )
		{
			continue;
		}
		kickPlayer( pPlayer ) ;
	}
	cleanUp();

	__LEAVE_FUNCTION
}

bool PlayerManager::broadCast(Packet* pPacket)
{
	__ENTER_FUNCTION

	for (int i=0; i<getObjNum(); ++i)
	{
		GamePlayer* pPlayer = getObjByIndex(i);
		if (!pPlayer 
			|| pPlayer->getStatus()!=EM_PLAYER_STATUS_NORMAL)
		{
			continue;
		}
		pPlayer->sendPacket(pPacket);
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}
