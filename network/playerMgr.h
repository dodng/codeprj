////////////////////////////////////////////////////////////////////////////////////
// 
// @file: playerMgr.h
// @author: by Mr.Chen
// @date: 2014/11/3	17:15
// @brief: ���ӻ���
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

	//��ʼ��
	virtual bool init() {return true;}
	//����
	virtual void cleanUp();
	//�������
	virtual bool select();
	//���ݽ���
	virtual bool processInputs();
	//���ݷ���
	virtual bool processOutputs();
	//�쳣���Ӵ���
	virtual bool processExceptions();
	//��Ϣִ��
	virtual bool processCmds();
	//�߼�
	virtual bool heartBeat(uint uTime);
	//��Player���ݼ���ϵͳ��
	virtual bool addPlayer(GamePlayer* pPlayer);
	//ɾ��һ����ң������������п���ת�Ƶ���ĳ�����
	virtual bool removePlayer(int nPlayerID);
	virtual bool removeSocket(GamePlayer* pPlayer);
	virtual void getMaxMinFD(SOCK removedFD,SOCK& maxFD,SOCK& minFD) {}
	//��Player��Ϣ��������Ͽ�����
	virtual bool kickPlayer(GamePlayer* pPlayer) {return true;}
	//������player��Ϣ����,���Ͽ�����
	virtual void kickAllPlayer();
	virtual bool broadCast(Packet* pPacket);

protected:
	fd_set		m_ReadFDs[EM_SELECT_MAX];		//��fd
	fd_set		m_WriteFDs[EM_SELECT_MAX];		//дfd
	fd_set		m_ExceptFDs[EM_SELECT_MAX];	//�쳣fd
	timeval		m_Timeout[EM_SELECT_MAX];		//select�ӳٷ���ʱ������
	SOCK		m_MinFD;					//��Сfd
	SOCK		m_MaxFD;				//���fd
	int				m_nFDSize ;				//FD����
};

#endif	//__PLAYER_MGR_H_

