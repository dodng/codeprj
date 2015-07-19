////////////////////////////////////////////////////////////////////////////////////
// 
// @file: player.h
// @author: by Mr.Chen
// @date: 2014/5/19	12:07
// @brief: ����ˣ���������
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
	//����socket
	void			    setSocket(SOCK s);
	Socket*		    getSocket() { return m_pSocket; }
	SocketStream* getSocketStream() { return m_pSocketStream;}
	//�Ͽ��뵱ǰ��ҵ���������
	virtual void	disconnect() ;
	//�жϵ�ǰ��ҵ����������Ƿ���Ч
	virtual	bool	isValid() ; 
	virtual bool   isIdle() { return false; }
	//��������
	virtual bool	processInput() ;
	//�������
	virtual bool	processOutput() ;
	//����Э��
	virtual bool	processCmd() ;
	//�����ӿڣ����������߼�
	virtual bool	heartBeat( uint uTime=0, int nFlag=0 ) {return true;}
	//��������
	virtual bool	sendPacket( Packet* pPacket ) ;
	//����,ȡ��������id
	int					getMgrID(){ return m_PlayerManagerID ; }
	void				setMgrID( int id ){ m_PlayerManagerID = id ; }
	//��ȡ�������PID�Ľӿڣ�PlayerID ����player�ط������������ֵ
	Player_ID		getPlayerID(){ return m_PlayerID ; } 
	int					getPoolID(){ return m_PlayerID; }
	void				setPoolID( int id ){ m_PlayerID = id ; } 
	//��������Ϣ�߳�
	virtual void	resetKickTime() {}
	//�ǿͻ������ӻ��Ƿ���������
	virtual bool	isGamePlayer() = 0 ;
	virtual bool	isServerPlayer() = 0 ;
	//Tencent TGW��
	virtual bool	getFlag() const {return false;}
	virtual void	setFlag(bool val) {}
	//Э������ܽ���
	virtual void	encrypt_sc(char* header, int uLen)=0;
	virtual void	decryptHead_cs(char* header)=0;
	virtual void	decrypt_cs(char* header, int uLen)=0;

protected :
	//�������Ӿ��
	Socket* m_pSocket ;
	//����������ݻ���
	SocketStream* m_pSocketStream ;
	//�ڹ������е�id
	int	 m_PlayerManagerID;
	//����λ��
	Player_ID m_PlayerID;
};

} //namespace tcpnet
using namespace tcpnet;

#endif
