////////////////////////////////////////////////////////////////////////////////////
// 
// @file: gamePlayer.h
// @author: by robert
// @date: 2014/5/26	14:36
// @brief: 连接
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GAME_PLAYER_H_
#define __GAME_PLAYER_H_

#include "type.h"
#include "player.h"
#include "timer.h"
#include "strUtil.h"

const int MAX_USER_LEN = 32;

//连接状态
enum EM_PLAYER_STATUS
{
	EM_PLAYER_STATUS_INVALID = -1,
	EM_PLAYER_STATUS_IDLE,						//初始状态
	EM_PLAYER_STATUS_CONNECT,				//连接成功后的状态，如果N秒内没收后面的消息，则断开此连接
	EM_PLAYER_STATUS_WAIT,						//等待进入正常状态
	EM_PLAYER_STATUS_NORMAL,				//进入正常状态
	EM_PLAYER_STATUS_NUM,
};

class GamePlayer : public Player
{
public :
	GamePlayer(bool bSocketClient = false, bool bServer = true) ;
	~GamePlayer() ;

	//初始化
	bool				init() ;
	//消息执行接口
	virtual bool		processCmd() ;
	//数据接收接口
	virtual bool		processInput() ;
	//数据发送接口
	virtual bool		processOutput() ;
	//心跳接口，处理所有逻辑
	virtual bool		heartBeat(uint uTime=0, int nFlag=0) ;
	//清除数据
	virtual void		cleanUp() ;

	bool				isIdle();

	//此接口只能在本执行线程内处理
	virtual bool		sendPacket(Packet* pPacket) ;

	//玩家状态设置、读取接口
	void				setStatus(EM_PLAYER_STATUS status){ m_Status = status ; }
	EM_PLAYER_STATUS	getStatus() { return m_Status; } ;
	
	//断开连接
	virtual void		disconnect() ;

	//释放并断开连接
	bool				free();
	//重置无消息踢除
	virtual void		resetKickTime();

	//是客户端连接还是服务器连接
	virtual bool		isGamePlayer() { return true; }
	virtual bool		isServerPlayer() { return false; }

	const char*	getUserId() const { return m_UserId; }
	void				setUserId(const char* id) { strUtil::Strcpy_s(m_UserId, id, sizeof(m_UserId)); }

	//协议包加密解密
	virtual void		encrypt_sc(char* header, int uLen){/*ENCRYPT(header, uLen, GAMESERVER_TO_CLIENT_KEY)*/}
	virtual void		decryptHead_cs(char* header){/*ENCRYPT_HEAD(header, CLIENT_TO_GAMESERVER_KEY)*/}
	virtual void		decrypt_cs(char* header, int uLen){/*ENCRYPT(header, uLen, CLIENT_TO_GAMESERVER_KEY)*/}

private:
	char m_UserId[MAX_USER_LEN]; //账号id
	EM_PLAYER_STATUS m_Status;	//状态
	Timer m_KickTimer;		//踢掉玩家计时
};

#endif
