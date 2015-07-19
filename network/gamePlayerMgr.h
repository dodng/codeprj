////////////////////////////////////////////////////////////////////////////////////
// 
// @file: gamePlayerMgr.h
// @author: by Mr.Chen
// @date: 2014/11/4	14:53
// @brief: ��½���ӹ�����
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
	Packet*		m_pPacket;		//Э���
	Player_ID	m_nPlayerID ;	//����ID
	bool		m_bRemove;		//ɾ�����
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

	//��ʼ��
	virtual bool init(int nCout, const char* host, int port) ;
	//���ݽ���
	virtual bool processInputs() ;
	//��Ϣִ��
	virtual bool processCmds() ;
	//�����Ӵ���
	bool acceptNewConnection() ;
	//��������Ϣ
	bool processCacheCmds() ;
	//֧������ͬ�����������ڲ�ͬ�߳��ڵ���
	bool sendPacket(Packet* pPacket, int PlayerID, bool bRemove=false) ;
	//��ȡ������Ϣ
	bool recivePacket(Packet*& pPacket, int& PlayerID, bool& bRemove) ;
	//���µ��������С
	bool resizeCache() ;
	//ɾ����ЧPlayer����Ϣ
	virtual bool removePacket(int PlayerID) ;

	//��Player���ݼ���ϵͳ��
	virtual bool addPlayer(GamePlayer* pPlayer) ;
	//��Player��Ϣ��������Ͽ�����
	virtual bool kickPlayer(GamePlayer* pPlayer) ;
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD);
	void relive();

private:
	SocketServer* m_pServerSocket ;	//���������ķ�����Socket
	SOCK m_ServerSocket;				
	MLock m_Lock ;							//��
	PACKET_CACHE* m_PacketQueue ;//��ǰ����Ϣ�������
	int	 m_PacketQueueSize ;				//��ǰ����Ϣ������г���
	int	 m_Head ;								//ͷ���
	int	 m_Tail ;								//β���

public :
	ThreadId  m_ThreadID ;
};

#endif
