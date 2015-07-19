////////////////////////////////////////////////////////////////////////////////////
// 
// @file: algorithm.h
// @author: by mr.chen
// @date: 2014/5/14	14:21
// @brief: 基础算法定义
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ALGORITHM_H_
#define __ALGORITHM_H_

namespace algorithm
{
	template <class T>
	void swap(T a, T b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

}; // //namespace algorithm
using namespace algorithm;

#endif //__ALGORITHM_H_
