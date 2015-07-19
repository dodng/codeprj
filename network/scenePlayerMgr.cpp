#include "scenePlayerMgr.h"
#include "playerPool.h"
#include <stdlib.h>
#include "timeSystem.h"
#include "packetfactoryMgr.h"

//初始化
bool	ScenePlayerManager::init(int nCout)
{
	__ENTER_FUNCTION

	bool bRet = ObjectManager<GamePlayer>::init(nCout);
	return bRet ;

	__LEAVE_FUNCTION
	return false ;
}

void	ScenePlayerManager::getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD)
{
    __ENTER_FUNCTION

    maxFD = minFD = INVALID_SOCKET;
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

bool ScenePlayerManager::kickPlayer( GamePlayer* pPlayer )
{
	__ENTER_FUNCTION
	Assert( pPlayer ) ;
	Assert(m_pScene);
    bool bRet = false;

	////清除Socket信息
	//const char* channelid = GetScene()->getChannelid().c_str();
	//int nPlayerID = pPlayer->GetPlayerID() ;
	//m_pScene->RemovePacket( nPlayerID ) ;
	////char* szIP = pPlayer->GetSocket()->GetIp();
	//bRet = RemovePlayer(nPlayerID) ;
	//Assert(bRet);

	////清除在池中的信息
	//bRet = pPlayer->Free() ;
	//Assert(bRet) ;
	////com_writelog(COMLOG_NOTICE, "ScenePlayerManager::KickPlayer, channelid=%s, playerid=%d\n", channelid, nPlayerID);

	return true ;
	__LEAVE_FUNCTION
	return false ;
}
