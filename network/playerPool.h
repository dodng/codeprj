////////////////////////////////////////////////////////////////////////////////////
// 
// @file: playerPool.h
// @author: by robert
// @date: 2014/11/3	17:30
// @brief: ���ӳ�
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PLAYER_POOL_H_
#define __PLAYER_POOL_H_

#include "type.h"
#include "player.h"
#include "lock.h"
#include "singleton.h"
#include "macroDef.h"

template<class Player_T>
class PlayerPool : public Singleton<PlayerPool<Player_T> >
{
public :
	~PlayerPool()
	{
		SAFE_DELETE(m_pPlayerPool);
	}

	//��ʼ��
	bool init(int nPlayerCount)
	{
		Assert(nPlayerCount>0);
		m_nCapity = nPlayerCount;
		m_pPlayerPool = new Player_T[m_nCapity];
		return true;
	}

	//������ҵ�PlayerIDȡ������ָ��
	Player_T* getPlayer( int PlayerID )
	{
		Assert(m_pPlayerPool);
		if(( PlayerID >= m_nCapity )||(PlayerID<0))
		{
			return NULL;
		}
		return &m_pPlayerPool[PlayerID];
	}

	//�ӿ������ݳ������һ���������
	Player_T* allocPlayer()
	{
		AutoMLock autoLock(m_Lock);
		for(int i=0;i<m_nCapity;++i)
		{
			if (m_pPlayerPool[i].isIdle())
			{
				m_pPlayerPool[i].cleanUp();
				m_pPlayerPool[i].setPoolID(i);
				return &m_pPlayerPool[i];
			}	
		}
		return NULL;
	}

	//��һ�����������ջ���
	bool recyclePlayer( int PlayerID )
	{
		AutoMLock autoLock(m_Lock);
		if(( PlayerID >= m_nCapity )||(PlayerID<0))
		{
			return false;
		}
		m_pPlayerPool[PlayerID].cleanUp();
		return true;
	}

private:
	MLock      m_Lock ;		//��
	Player_T* m_pPlayerPool;	//��
	int           m_nCapity;	
};

#endif	// __PLAYER_POOL_H_

