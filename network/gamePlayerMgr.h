////////////////////////////////////////////////////////////////////////////////////
// 
// @file: gamePlayerMgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	14:53
// @brief: 登陆链接管理器
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GAME_PLAYER_MGR_H_
#define __GAME_PLAYER_MGR_H_

#include "type.h"
#include "packet.h"
#include "socketserver.h"
#include "socketdef.h"
#include "lock.h"
#include "singleton.h"
#include "thread.h"
#include "gamePlayer.h"
#include "playerMgr.h"

struct PACKET_CACHE
{
	Packet*		m_pPacket;		//协议包
	Player_ID	m_nPlayerID ;	//连接ID
	bool		m_bRemove;		//删除标记
	PACKET_CACHE()
	{
		m_pPacket = NULL;
		m_nPlayerID = -1;
		m_bRemove = false;
	}
	void CleanUp()
	{
		m_pPacket = NULL;
		m_nPlayerID = -1;
		m_bRemove = false;
	}
};

class GamePlayerManager : public PlayerManager, public Singleton<GamePlayerManager>
{
public:
	static const int MAX_PACKET_CACHE_SIZE = 4096;

public :
	GamePlayerManager();
	~GamePlayerManager();

	//初始化
	virtual bool init(int nCout, const char* host, int port) ;
	//数据接受
	virtual bool processInputs() ;
	//消息执行
	virtual bool processCmds() ;
	//新连接处理
	bool acceptNewConnection() ;
	//处理缓存消息
	bool processCacheCmds() ;
	//支持数据同步，即可以在不同线程内调用
	bool sendPacket(Packet* pPacket, int PlayerID, bool bRemove=false) ;
	//读取缓存消息
	bool recivePacket(Packet*& pPacket, int& PlayerID, bool& bRemove) ;
	//重新调整缓存大小
	bool resizeCache() ;
	//删除无效Player的消息
	virtual bool removePacket(int PlayerID) ;

	//将Player数据加入系统中
	virtual bool addPlayer(GamePlayer* pPlayer) ;
	//将Player信息清除，并断开连接
	virtual bool kickPlayer(GamePlayer* pPlayer) ;
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD);
	void relive();

private:
	SocketServer* m_pServerSocket ;	//用于侦听的服务器Socket
	SOCK m_ServerSocket;				
	MLock m_Lock ;							//锁
	PACKET_CACHE* m_PacketQueue ;//当前的消息缓存队列
	int	 m_PacketQueueSize ;				//当前的消息缓存队列长度
	int	 m_Head ;								//头标记
	int	 m_Tail ;								//尾标记

public :
	ThreadId  m_ThreadID ;
};

#endif
