////////////////////////////////////////////////////////////////////////////////////
// 
// @file: gamePlayer.h
// @author: by robert
// @date: 2014/5/26	14:36
// @brief: ����
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __GAME_PLAYER_H_
#define __GAME_PLAYER_H_

#include "type.h"
#include "player.h"
#include "timer.h"
#include "strUtil.h"

const int MAX_USER_LEN = 32;

//����״̬
enum EM_PLAYER_STATUS
{
	EM_PLAYER_STATUS_INVALID = -1,
	EM_PLAYER_STATUS_IDLE,						//��ʼ״̬
	EM_PLAYER_STATUS_CONNECT,				//���ӳɹ����״̬�����N����û�պ������Ϣ����Ͽ�������
	EM_PLAYER_STATUS_WAIT,						//�ȴ���������״̬
	EM_PLAYER_STATUS_NORMAL,				//��������״̬
	EM_PLAYER_STATUS_NUM,
};

class GamePlayer : public Player
{
public :
	GamePlayer(bool bSocketClient = false, bool bServer = true) ;
	~GamePlayer() ;

	//��ʼ��
	bool				init() ;
	//��Ϣִ�нӿ�
	virtual bool		processCmd() ;
	//���ݽ��սӿ�
	virtual bool		processInput() ;
	//���ݷ��ͽӿ�
	virtual bool		processOutput() ;
	//�����ӿڣ����������߼�
	virtual bool		heartBeat(uint uTime=0, int nFlag=0) ;
	//�������
	virtual void		cleanUp() ;

	bool				isIdle();

	//�˽ӿ�ֻ���ڱ�ִ���߳��ڴ���
	virtual bool		sendPacket(Packet* pPacket) ;

	//���״̬���á���ȡ�ӿ�
	void				setStatus(EM_PLAYER_STATUS status){ m_Status = status ; }
	EM_PLAYER_STATUS	getStatus() { return m_Status; } ;
	
	//�Ͽ�����
	virtual void		disconnect() ;

	//�ͷŲ��Ͽ�����
	bool				free();
	//��������Ϣ�߳�
	virtual void		resetKickTime();

	//�ǿͻ������ӻ��Ƿ���������
	virtual bool		isGamePlayer() { return true; }
	virtual bool		isServerPlayer() { return false; }

	const char*	getUserId() const { return m_UserId; }
	void				setUserId(const char* id) { strUtil::Strcpy_s(m_UserId, id, sizeof(m_UserId)); }

	//Э������ܽ���
	virtual void		encrypt_sc(char* header, int uLen){/*ENCRYPT(header, uLen, GAMESERVER_TO_CLIENT_KEY)*/}
	virtual void		decryptHead_cs(char* header){/*ENCRYPT_HEAD(header, CLIENT_TO_GAMESERVER_KEY)*/}
	virtual void		decrypt_cs(char* header, int uLen){/*ENCRYPT(header, uLen, CLIENT_TO_GAMESERVER_KEY)*/}

private:
	char m_UserId[MAX_USER_LEN]; //�˺�id
	EM_PLAYER_STATUS m_Status;	//״̬
	Timer m_KickTimer;		//�ߵ���Ҽ�ʱ
};

#endif
