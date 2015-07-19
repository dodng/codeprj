////////////////////////////////////////////////////////////////////////////////////
// 
// @file: playerMgr.h
// @author: by Mr.Chen
// @date: 2014/11/3	17:15
// @brief: 连接基类
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __PLAYER_MGR_H_
#define __PLAYER_MGR_H_

#include "type.h"
#include "socketdef.h"
#include "packet.h"
#include "objectMgr.h"
#include "lock.h"
#include "assert.h"
#include "macroDef.h"


class GamePlayer;
class PlayerManager : public ObjectManager<GamePlayer>
{
public :
	PlayerManager();
	~PlayerManager() {}

	//初始化
	virtual bool init() {return true;}
	//清理
	virtual void cleanUp();
	//网络侦测
	virtual bool select();
	//数据接受
	virtual bool processInputs();
	//数据发送
	virtual bool processOutputs();
	//异常连接处理
	virtual bool processExceptions();
	//消息执行
	virtual bool processCmds();
	//逻辑
	virtual bool heartBeat(uint uTime);
	//将Player数据加入系统中
	virtual bool addPlayer(GamePlayer* pPlayer);
	//删除一个玩家（并不断网，有可能转移到别的场景）
	virtual bool removePlayer(int nPlayerID);
	virtual bool removeSocket(GamePlayer* pPlayer);
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD) {}
	//将Player信息清除，并断开连接
	virtual bool kickPlayer(GamePlayer* pPlayer) {return true;}
	//将所有player信息清理,并断开连接
	virtual void kickAllPlayer();
	virtual bool broadCast(Packet* pPacket);

protected:
	fd_set		m_ReadFDs[EM_SELECT_MAX];		//读fd
	fd_set		m_WriteFDs[EM_SELECT_MAX];		//写fd
	fd_set		m_ExceptFDs[EM_SELECT_MAX];	//异常fd
	timeval		m_Timeout[EM_SELECT_MAX];		//select延迟返回时间配置
	SOCK		m_MinFD;					//最小fd
	SOCK		m_MaxFD;				//最大fd
	int				m_nFDSize ;				//FD个数
};

#endif	//__PLAYER_MGR_H_

