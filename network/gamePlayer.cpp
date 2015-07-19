#include "gamePlayer.h"
#include "playerPool.h"
#include "packetfactoryMgr.h"
#include "timeSystem.h"
#include "playerPool.h"

WX_IMPLEMENT_SINGLEON(PlayerPool<GamePlayer>);

GamePlayer::GamePlayer( bool bSocketClient, bool bServer):Player(bSocketClient, bServer)
{
__ENTER_FUNCTION

	m_Status = EM_PLAYER_STATUS_IDLE ;
	memset(m_UserId,0,sizeof(m_UserId));

__LEAVE_FUNCTION
}

GamePlayer::~GamePlayer( )
{
__ENTER_FUNCTION
	m_Status = EM_PLAYER_STATUS_INVALID;

__LEAVE_FUNCTION
}

bool	GamePlayer::init( )
{
	__ENTER_FUNCTION

	uint uTime = TimeSystem::getSinglePtr()->getCurrRunTime();
	m_KickTimer.beginTimer(2*60*1000, uTime);

	return true ;
	__LEAVE_FUNCTION
	return false;
}

void	GamePlayer::cleanUp( )
{
__ENTER_FUNCTION
	Player::cleanUp( ) ;
	m_Status = EM_PLAYER_STATUS_IDLE;
	memset(m_UserId,0,sizeof(m_UserId));
	m_KickTimer.endTimer();
__LEAVE_FUNCTION
}

bool	GamePlayer::isIdle()
{
	__ENTER_FUNCTION
	if (m_Status == EM_PLAYER_STATUS_IDLE && m_PlayerID<0)
	{
		return true;
	}
	return false;
	__LEAVE_FUNCTION
	return false;
}

bool	GamePlayer::processCmd()
{
__ENTER_FUNCTION
	return Player::processCmd( ) ;
__LEAVE_FUNCTION
	return false ;
}

bool	GamePlayer::processInput( )
 {
__ENTER_FUNCTION
	return Player::processInput( ) ;
__LEAVE_FUNCTION
	return false ;
}

bool	GamePlayer::processOutput( )
{
__ENTER_FUNCTION
	return Player::processOutput( ) ;
__LEAVE_FUNCTION
	return false ;
}

bool	GamePlayer::sendPacket( Packet* pPacket )
{
__ENTER_FUNCTION
	Assert(pPacket);
	return Player::sendPacket( pPacket ) ;
__LEAVE_FUNCTION
	return false ;
}

bool	GamePlayer::heartBeat( uint uTime, int nFlag )
{
	__ENTER_FUNCTION
	if (m_KickTimer.isSet())
	{
		//Ìßµô
		if (m_KickTimer.isReachTerm(uTime))
		{
			m_KickTimer.endTimer();
			return false;
		}
	}
	return true;
	__LEAVE_FUNCTION
	return false ;
}

void	GamePlayer::disconnect( )
{
	__ENTER_FUNCTION

	Player::disconnect( ) ;

	__LEAVE_FUNCTION
}

bool	GamePlayer::free( )
{
	__ENTER_FUNCTION
	PlayerPool<GamePlayer>::getSinglePtr()->recyclePlayer(getPlayerID());
	return true ;
	__LEAVE_FUNCTION
	return false ;
}

void GamePlayer::resetKickTime( )
{
	__ENTER_FUNCTION

	m_KickTimer.endTimer();
	m_KickTimer.beginTimer(2*60*1000, TimeSystem::getSinglePtr()->getCurrRunTime());

	__LEAVE_FUNCTION
}
