////////////////////////////////////////////////////////////////////////////////////
// 
// @file: scenePlayerMgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	16:13
// @brief: �����߼�������ӹ�����
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
	//��ʼ��
	virtual bool init(int nCout) ;
    //
    virtual bool kickPlayer( GamePlayer* pPlayer );
	//ɾ��һ����ң������������п���ת�Ƶ���ĳ�����
   virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD);
	//Channel*		GetScene(){ return m_pScene; };									//Ƶ��ָ��
	//void			SetScene( Channel* pScene ){  m_pScene = pScene; }	//����Ƶ��
    
private :
	//����
	Channel*	m_pScene;	// ����Ƶ����ָ��
};

#endif
