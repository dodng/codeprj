////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player.h
// @author: by Mr.Chen
// @date: 2014/5/19	12:07
// @brief: 接入端，网络连接
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "type.h"
#include "socketdef.h"
#include "socket.h"
#include "packet.h"
#include "socketstream.h"

namespace tcpnet {

using tcpnet::Socket;
using tcpnet::Packet;
using tcpnet::SocketStream;
////////////////////////////////////////////////////////////////////////////////
//	class Player
////////////////////////////////////////////////////////////////////////////////
class Player
{
public :
	Player(bool bSocketClient=false, bool bServer=false) ;
	~Player() ;
	
	virtual bool	init() { return true; }
	virtual void	cleanUp();
	//设置socket
	void			    setSocket(SOCK s);
	Socket*		    getSocket() { return m_pSocket; }
	SocketStream* getSocketStream() { return m_pSocketStream;}
	//断开与当前玩家的网络连接
	virtual void	disconnect() ;
	//判断当前玩家的网络连接是否有效
	virtual	bool	isValid() ; 
	virtual bool   isIdle() { return false; }
	//处理输入
	virtual bool	processInput() ;
	//处理输出
	virtual bool	processOutput() ;
	//处理协议
	virtual bool	processCmd() ;
	//心跳接口，处理所有逻辑
	virtual bool	heartBeat( uint uTime=0, int nFlag=0 ) {return true;}
	//发送数据
	virtual bool	sendPacket( Packet* pPacket ) ;
	//设置,取管理器中id
	int					getMgrID(){ return m_PlayerManagerID ; }
	void				setMgrID( int id ){ m_PlayerManagerID = id ; }
	//读取设置玩家PID的接口，PlayerID 是由player池分配出来的索引值
	Player_ID		getPlayerID(){ return m_PlayerID ; } 
	int					getPoolID(){ return m_PlayerID; }
	void				setPoolID( int id ){ m_PlayerID = id ; } 
	//重置无消息踢除
	virtual void	resetKickTime() {}
	//是客户端连接还是服务器连接
	virtual bool	isGamePlayer() = 0 ;
	virtual bool	isServerPlayer() = 0 ;
	//Tencent TGW包
	virtual bool	getFlag() const {return false;}
	virtual void	setFlag(bool val) {}
	//协议包加密解密
	virtual void	encrypt_sc(char* header, int uLen)=0;
	virtual void	decryptHead_cs(char* header)=0;
	virtual void	decrypt_cs(char* header, int uLen)=0;

protected :
	//网络连接句柄
	Socket* m_pSocket ;
	//输入输出数据缓存
	SocketStream* m_pSocketStream ;
	//在管理器中的id
	int	 m_PlayerManagerID;
	//池中位置
	Player_ID m_PlayerID;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
