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
	// 初始化
	bool							init();
	// 创建网络包
	Packet*						createPacket(Packet_ID id);
	// 删除网络包
	bool							removePacket(Packet* pPacket);
	// 获取包的最大可能长度
	int								getPacketMaxSize(Packet_ID id) const;
	//
	void							addFactory( PacketFactory* pFactory ) ;
	//检查PacketID
	bool							checkPacketID(Packet_ID id);

private:
	// 注册包工厂
	void							__registerFactory();

private:
	PacketFactory**		m_pFactory;		//包工厂
	int*							m_pStatistic;	//包创建数量统计
};

} //namespace tcpnet
using namespace tcpnet;

#endif
