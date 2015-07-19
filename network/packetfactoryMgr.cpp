#include "packetfactoryMgr.h"
#include "assert.h"

WX_IMPLEMENT_SINGLEON(tcpnet::PacketFactoryManager);

namespace tcpnet {

PacketFactoryManager::PacketFactoryManager()
{
	__ENTER_FUNCTION
	m_pFactory = new PacketFactory*[PACKET_ID_NUMBER];
	Assert(m_pFactory);
	m_pStatistic = new int[PACKET_ID_NUMBER];
	Assert(m_pStatistic);
	for (int i=0; i<PACKET_ID_NUMBER; ++i)
	{
		m_pFactory[i] = NULL;
		m_pStatistic[i] = 0;
	}
	__LEAVE_FUNCTION
}

PacketFactoryManager::~PacketFactoryManager()
{
	__ENTER_FUNCTION
	delete[] m_pFactory;
	delete[] m_pStatistic;
	__LEAVE_FUNCTION
}

bool PacketFactoryManager::init()
{
	__ENTER_FUNCTION
	__registerFactory();
	return true;
	__LEAVE_FUNCTION
	return false;
}

Packet* PacketFactoryManager::createPacket(Packet_ID id)
{
	__ENTER_FUNCTION
	if ((m_pFactory[id] == NULL)||(id<0)||(id>=PACKET_ID_NUMBER))
	{
		Assert(0);
	}
	Packet* pPacket = NULL;
	PacketFactory* pFactory = m_pFactory[id];
	if (pFactory != NULL)
	{
		//增加包的创建计数
		++m_pStatistic[id];
		pPacket = pFactory->createPacket();
		if (pPacket == NULL)
		{
			Assert(0);
		}
	}

	return pPacket;
	__LEAVE_FUNCTION
	return NULL;
}

bool PacketFactoryManager::removePacket(Packet* pPacket)
{
	__ENTER_FUNCTION
	Assert(pPacket);

	bool bRet = false;
	Packet_ID id = pPacket->getPacketID();
	PacketFactory* pFactory = m_pFactory[id];
	if (pFactory != NULL)
	{
		bRet = pFactory->removePacket(pPacket);

	}
	return bRet;
	__LEAVE_FUNCTION
	return false;
}

int PacketFactoryManager::getPacketMaxSize(Packet_ID id) const
{
	if (id<=PACKET_ID_INVALID || id>PACKET_ID_NUMBER || m_pFactory[id]==NULL)
	{
		return -1;
	}
	return m_pFactory[id]->getPacketMaxSize();
}

void PacketFactoryManager::addFactory( PacketFactory* pFactory ) 
{
	__ENTER_FUNCTION

	if( m_pFactory[pFactory->getPacketID()]!=NULL ) 
	{//重复设定
		Assert( false ) ;
		return ;
	}

	m_pFactory[pFactory->getPacketID()] = pFactory ;

	__LEAVE_FUNCTION
}

bool	PacketFactoryManager::checkPacketID(Packet_ID id)
{
	__ENTER_FUNCTION

	if (id<=PACKET_ID_INVALID || id>PACKET_ID_NUMBER || m_pFactory[id] == NULL)
	{
		return false;
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

void PacketFactoryManager::__registerFactory()
{
	__ENTER_FUNCTION

	PacketFactory *pPacketFactory = NULL;
	//NEW_NORET(pPacketFactory, CGTestPacketFactory()); AddFactory(pPacketFactory);

	__LEAVE_FUNCTION
}

} //namespace tcpnet
