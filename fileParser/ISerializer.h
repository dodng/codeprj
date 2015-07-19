////////////////////////////////////////////////////////////////////////////////////
// 
// @file: ISerializer.h
// @author: by Mr.Chen
// @date: 2014/5/15	15:05
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SERIALIZER_H_
#define __SERIALIZER_H_

#include "type.h"
#include "assert.h"

namespace tabReader {

// 扩展类型
// 整型数组
struct IntArray
{
public:
	int m_Count;
	IntArray() 
	{ 
		m_Array = NULL; 
		m_Count = 0; 
	}
	~IntArray() {}
	int operator[](const int i) const 
	{ 
		Assert(i < m_Count && i >= 0); 
		return m_Array[i]; 
	}
	int count() const { return m_Count; }
	bool isValid() const
	{
		if(NULL == m_Array) return false;
		else return true;
	}
	int* init(int nSize)
	{
		if (nSize > 0)
		{
			m_Array = new int[nSize];
			m_Count = nSize;
			return m_Array;
		}
		return NULL;
	}
	bool isContains(const int value) const
	{
		for (int i = 0; i < count(); ++i)
		{
			if (value == m_Array[i])
			{
				return true;
			}
		}
		return false;
	}
private:
	int* m_Array;
};

// 浮点数组
struct FloatArray
{
	float*	m_Array;
	int		m_Count;
	FloatArray() { m_Array = NULL; m_Count = 0; }
	~FloatArray() {}
	float operator[](const int i) const { Assert(i < m_Count && i >= 0); return m_Array[i]; }
};

// 基类
class ISerializer
{
public:
	virtual ISerializer& operator<< (int& value)		= 0;
	virtual ISerializer& operator<< (uint& value)		= 0;
	virtual ISerializer& operator<< (int64& value)	= 0;
	virtual ISerializer& operator<< (uint64& value)	= 0;
	virtual ISerializer& operator<< (float& value)	= 0;
	virtual ISerializer& operator<< (char*& value)	= 0;
	virtual ISerializer& operator<< (IntArray& value)	= 0;
	virtual ISerializer& operator<< (FloatArray& value)= 0;
	virtual void skipField() = 0;
	virtual void setCheckColumn(bool isCheck) = 0;
};

} // namespace tabReader
using namespace tabReader;

#endif // __SERIALIZER_H_

