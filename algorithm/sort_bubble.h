////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_bubble.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:07
// @brief: 冒泡排序
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_BUBBLE_H_
#define __SORT_BUBBLE_H_

#include "algorithm.h"

namespace algorithm
{
	// 冒泡排序
	//////////////////////////////////////////////////////////////////////////
	///version 1
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	void	sort_bubble_V1(T array[],int length)
	{
		//把a[0...n-1]排序
		bool change = true;
		for(int i=length-1; i>=1 && change; --i)
		{
			change = false;
			for(int j=0; j<i; ++j)
			{
				if(array[j] > array[j+1])
				{
					swap<T>(array[j],array[j+1]);
					change = true;
				}
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////
	///version 2
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	void	sort_bubble_V2(T array[],int length) //记录最后的交换位置
	{
		//把a[0...n-1]排序
		int numPairs = length-1;
		int lastExchangeIndex = -1;
		while(numPairs > 0)
		{
			lastExchangeIndex = -1;
			for(int i=0; i<numPairs; ++i)
			{
				if(array[i] > array[i+1])
				{
					swap<T>(array[i],array[i+1]);
					lastExchangeIndex = i;
				}
			}
			numPairs = lastExchangeIndex;
		}
	}


	//////////////////////////////////////////////////////////////////////////
	///version 3
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	void	sort_bubble_V3(T array[],int length) //记录开始和最后的交换位置
	{
		//把a[0...n-1]排序
		int numPairs1 = 0;
		int numPairs2 = length-1;
		int firstExchangeIndex = -1;
		int lastExchangeIndex = -1;
		while(numPairs1 < numPairs2)
		{
			int numOfExchange = 0;
			firstExchangeIndex = -1;
			lastExchangeIndex = -1;
			for(int i=numPairs1; i<numPairs2; ++i)
			{
				if(array[i] > array[i+1])
				{
					swap<T>(array[i],array[i+1]);
					numOfExchange++;
					if(1 == numOfExchange)
						firstExchangeIndex = i ? i-1 : i;
					lastExchangeIndex = i;
				}
			}
			numPairs1 = firstExchangeIndex;
			numPairs2 = lastExchangeIndex;
		}
	}
};	// namespace algorithm

using namespace algorithm;

#endif	// __SORT_BUBBLE_H_

