////////////////////////////////////////////////////////////////////////////////////
// 
// @file: packet.h
// @author: by Mr.Chen
// @date: 2014/5/11	02:25
// @brief: tcp网络包结构
//
////////////////////////////////////////////////////////////////////////////////////

/*
|-----------------------------------------------------------------------------------------------------------------------|
| 包类型编号（2Byte）| 加密标识（1Byte）| 包数据长度（4Byte）|	数据内容|
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
	//获得包id
	virtual Packet_ID	getPacketID() const = 0;
	//获得加密标识
	virtual char		getEncryptFlag() const { return 0; };
	//获得数据大小
	virtual int		getSize() const { return m_nSize; };
	//设置包大小
	virtual void		setSize(int nSize) { m_nSize = nSize; };
	//包执行
	virtual int		execute( Player* pPlayer ) = 0 ;
	//获得数据大小
	virtual bool		fill( SocketStream& ioStream , EM_TYPE_MSG_MODE mode) = 0 ;
	//数据清理
	virtual void		cleanUp(){ m_nSize = 0;};
	
protected:
	int	m_nSize;		//数据大小
private:
	Packet* m_pNext;		//缓存
};

} //namespace tcpnet

using namespace tcpnet;

#endif
