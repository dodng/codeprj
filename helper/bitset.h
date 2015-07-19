////////////////////////////////////////////////////////////////////////////////////
// 
// @file: bitset.h
// @author: by Mr.Chen
// @date: 2014/5/19	11:51
// @brief: 位集合
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __BIT_SET_H_
#define __BIT_SET_H_

#include "type.h"
#include "assert.h"
#include <string.h>

namespace sysUtil {
//
//该类用于设置位标识
//传入的模板参数为总共需要设置的位数。
//
template<int bitNumber>
class BitSet
{
public:
	enum 
	{
		BYTE_SIZE = 8,
		DIMENSION = (bitNumber+7)/BYTE_SIZE,
	};

public:
	BitSet() { clearAllBits(); }
	bool isSetBit(uint index) const;
	void setBit(uint index) { _updateBit(index, true);}
	void clearBit(uint index) { _updateBit(index, false);}
	void clearAllBits();
	void setAllBits();

private:
	void _updateBit(uint index, bool flag);
	byte m_flags[DIMENSION];
};

template<int bitNumber>
bool	BitSet<bitNumber>::isSetBit(uint index) const
{
	__ENTER_FUNCTION
	Assert(index < bitNumber);
	return (m_flags[index/BYTE_SIZE] &(1<<(index%BYTE_SIZE))) ? true : false;
	__LEAVE_FUNCTION
	return false;
}

template<int bitNumber>
void	BitSet<bitNumber>::clearAllBits()
{
	__ENTER_FUNCTION
	memset(m_flags, 0, sizeof(byte)*DIMENSION);
	__LEAVE_FUNCTION
}

template<int bitNumber>
void	BitSet<bitNumber>::setAllBits()
{
	__ENTER_FUNCTION
	memset(m_flags, 0xff, sizeof(byte)*DIMENSION);
	__LEAVE_FUNCTION
}

template<int bitNumber>
void	BitSet<bitNumber>::_updateBit(uint index, bool flag)
{
	__ENTER_FUNCTION
	Assert(index < bitNumber);
	if (flag)
	{
		m_flags[index/BYTE_SIZE] |= (1<<(index%BYTE_SIZE));
	}
	else
	{
		m_flags[index/BYTE_SIZE] &= ~(1<<(index%BYTE_SIZE));
	}
	__LEAVE_FUNCTION
}

} //namespace sysUtil
using namespace sysUtil;

#endif
