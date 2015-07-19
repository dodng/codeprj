////////////////////////////////////////////////////////////////////////////////////
// 
// @file: scenePlayerMgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	16:13
// @brief: 进入逻辑后的连接管理器
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SCENE_PLAYER_MGR_H_
#define __SCENE_PLAYER_MGR_H_

#include "type.h"
#include "playerMgr.h"
#include "lock.h"
#include "gamePlayer.h"

class Channel;
class ScenePlayerManager : public PlayerManager
{
public :
	//初始化
	virtual bool init(int nCout) ;
    //
    virtual bool kickPlayer( GamePlayer* pPlayer );
	//删除一个玩家（并不断网，有可能转移到别的场景）
   virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD);
	//Channel*		GetScene(){ return m_pScene; };									//频道指针
	//void			SetScene( Channel* pScene ){  m_pScene = pScene; }	//设置频道
    
private :
	//场景
	Channel*	m_pScene;	// 所在频道的指针
};

#endif
