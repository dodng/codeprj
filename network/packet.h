////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packet.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:25
// @brief: tcp������ṹ
//
////////////////////////////////////////////////////////////////////////////////////

/*
|-----------------------------------------------------------------------------------------------------------------------|
| �����ͱ�ţ�2Byte��| ���ܱ�ʶ��1Byte��| �����ݳ��ȣ�4Byte��|	��������|
|------------------------------------------------------------------------------------------------------------------------|
*/

#ifndef __PACKET_H_
#define __PACKET_H_

#include "type.h"
#include "socketdef.h"
#include "socketstream.h"

namespace tcpnet {

class Player;
////////////////////////////////////////////////////////////////////////////////
//	class Packet
////////////////////////////////////////////////////////////////////////////////
class Packet
{
	friend class PacketFactory;
public:
	Packet()
	{ 
		m_pNext = NULL;
		m_nSize = 0;
	}
	virtual ~Packet() {}
	//��ð�id
	virtual Packet_ID	getPacketID() const = 0;
	//��ü��ܱ�ʶ
	virtual char		getEncryptFlag() const { return 0; };
	//������ݴ�С
	virtual int		getSize() const { return m_nSize; };
	//���ð���С
	virtual void		setSize(int nSize) { m_nSize = nSize; };
	//��ִ��
	virtual int		execute( Player* pPlayer ) = 0 ;
	//������ݴ�С
	virtual bool		fill( SocketStream& ioStream , EM_TYPE_MSG_MODE mode) = 0 ;
	//��������
	virtual void		cleanUp(){ m_nSize = 0;};
	
protected:
	int	m_nSize;		//���ݴ�С
private:
	Packet* m_pNext;		//����
};

} //namespace tcpnet

using namespace tcpnet;

#endif
