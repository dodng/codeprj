////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packetFactory.h
// @author: by Mr.Chen
// @date: 2014/5/11	01:51
// @brief: ��Ϣ������
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __PACKET_FACTORY_H_
#define __PACKET_FACTORY_H_

#include "type.h"
#include "packet.h"
#include "lock.h"
#include "assert.h"

using tcpnet::Packet;
namespace tcpnet {

////////////////////////////////////////////////////////////////////////////////
//	class Packet
////////////////////////////////////////////////////////////////////////////////
class PacketFactory
{
public:
	PacketFactory()
	{
		m_pCache = NULL;
		m_NewCount = 0;
		m_CreateCount = 0;
		m_RemoveCount = 0;
	}
	virtual ~PacketFactory()
	{
		while (m_pCache != NULL)
		{
			Packet* pPacket = m_pCache;
			m_pCache = m_pCache->m_pNext;
			delete pPacket;
			pPacket = NULL;
		}
	}
public:
	// ����
	Packet* createPacket()
	{
		AutoMLock autolock(m_Lock);
		Packet* pRet = NULL;
		if (m_pCache != NULL)
		{
			pRet = m_pCache;
			m_pCache = m_pCache->m_pNext;
		}
		else
		{
			__MYTRY
			{
				pRet = __create();
				++m_NewCount;
			}
			__MYCATCH
			{
				pRet = NULL;
			}
		}
		if (pRet)
		{
			pRet->cleanUp();
		}
		
		++m_CreateCount;
		return pRet;
	}
	// ����
	bool removePacket(Packet* pPacket)
	{
		AutoMLock autolock(m_Lock);
		if (pPacket == NULL) return false;	

		pPacket->m_pNext = m_pCache;
		m_pCache = pPacket;

		++m_RemoveCount;
		return true;
	}
public:
	// ��ȡ�����ͱ��
	virtual	Packet_ID getPacketID() const = 0;
	// ��ȡ���������ܳ��� �����ڼ�飩
	virtual int getPacketMaxSize() const = 0;
private:
	// ��������
	virtual Packet* __create() = 0;
private:
	//����Ĳ�Ʒ���������
	Packet* m_pCache;	
	MLock m_Lock;	//��
	int	m_NewCount;	//��������
	int   m_CreateCount; //��������
	int   m_RemoveCount;	//ɾ������
};

} //namespace tcpnet

using namespace tcpnet;

#endif
