////////////////////////////////////////////////////////////////////////////////////
// 
// @file: objectMgr.h
// @author: by robert
// @date: 2014/5/26	15:20
// @brief: ���������
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __OBJECT_MGR_H_
#define __OBJECT_MGR_H_

#include "type.h"
#include "assert.h"
#include "macroDef.h"

template<class T> 
class ObjectManager
{
public:
	ObjectManager()
	{
		__ENTER_FUNCTION
		m_nCapacity = 0;
		m_nObjectNumber    = 0;
		m_pObjectList = NULL;
		__LEAVE_FUNCTION
	}

	virtual ~ObjectManager()
	{
		__ENTER_FUNCTION
		SAFE_DELETE_ARRAY(m_pObjectList);
		__LEAVE_FUNCTION
	}

	//��ʼ��
	virtual bool init(int nCount)
	{
		__ENTER_FUNCTION
		Assert(nCount > 0);
		Assert(m_pObjectList == NULL);

		m_nObjectNumber = 0;
		m_nCapacity = nCount;
		m_pObjectList = new T*[m_nCapacity];
		for (int i=0; i<m_nCapacity; ++i) 
		{
			m_pObjectList[i] = NULL;
		}
		return true;
		__LEAVE_FUNCTION
		return false;
	}

	//����
	virtual void cleanUp()
	{
		__ENTER_FUNCTION

		m_nObjectNumber = 0;
		for (int i=0; i<m_nCapacity; ++i) 
		{
			m_pObjectList[i] = NULL;
		}
		__LEAVE_FUNCTION
	}

	//�߼��ӿ�
	virtual bool heartBeat(uint uTime)
	{
		return true;
	}

	//����
	bool addObj(T* pObject)
	{
		__ENTER_FUNCTION
		Assert(pObject);
		if (m_nObjectNumber>=m_nCapacity)
		{
			return false;
		}
		m_pObjectList[m_nObjectNumber] = pObject;
		pObject->setMgrID(m_nObjectNumber);
		++m_nObjectNumber;
		return true;
		__LEAVE_FUNCTION
		return false;
	}

	//ɾ��
	bool removeObj(T* pObject)
	{
		__ENTER_FUNCTION
		Assert(pObject);
		int nIndex = pObject->getMgrID();
		Assert(nIndex<m_nCapacity);
		if (nIndex<0)
		{
			return true;
		}
		Assert(m_nObjectNumber>0);
		m_pObjectList[nIndex] = m_pObjectList[m_nObjectNumber - 1];
		if (nIndex != m_nObjectNumber -1)
		{
			if (m_pObjectList[nIndex] != NULL)
			{
				m_pObjectList[nIndex]->setMgrID(nIndex);
			}
		}

		m_pObjectList[m_nObjectNumber -1] = NULL;
		pObject->setMgrID(-1);
		--m_nObjectNumber;

		return true;
		__LEAVE_FUNCTION
		return false;
	}

	//�Ƿ���
	bool	hasObject( )
	{
		__ENTER_FUNCTION
		if (m_nObjectNumber>0)
		{
			return true;
		}
		return false;
		__LEAVE_FUNCTION
		return false;
	}

	//�������
	int getObjNum( ){ return m_nObjectNumber ; }

	//���
	T* getObjByIndex(int nIndex)
	{
		__ENTER_FUNCTION
		if((nIndex<m_nObjectNumber)&&(nIndex>=0))
		{
			return m_pObjectList[nIndex];
		}
		return NULL;
		__LEAVE_FUNCTION
		return NULL;
	}

protected:
	int		m_nCapacity;
	int    m_nObjectNumber;
	T**	m_pObjectList;
};

#endif
