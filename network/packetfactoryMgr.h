////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packetfactoryMgr.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __PACKET_FACTORY_MGR_H_
#define __PACKET_FACTORY_MGR_H_

#include "type.h"
#include "packet.h"
#include "packetfactory.h"
#include "packetDef.h"
#include "singleton.h"

namespace tcpnet {

////////////////////////////////////////////////////////////////////////////////
//	class PacketFactoryManager
////////////////////////////////////////////////////////////////////////////////
class PacketFactoryManager : public Singleton<PacketFactoryManager>
{
public:
	PacketFactoryManager();
	~PacketFactoryManager();

public:
	// ��ʼ��
	bool							init();
	// ���������
	Packet*						createPacket(Packet_ID id);
	// ɾ�������
	bool							removePacket(Packet* pPacket);
	// ��ȡ���������ܳ���
	int								getPacketMaxSize(Packet_ID id) const;
	//
	void							addFactory( PacketFactory* pFactory ) ;
	//���PacketID
	bool							checkPacketID(Packet_ID id);

private:
	// ע�������
	void							__registerFactory();

private:
	PacketFactory**		m_pFactory;		//������
	int*							m_pStatistic;	//����������ͳ��
};

} //namespace tcpnet
using namespace tcpnet;

#endif
