////////////////////////////////////////////////////////////////////////////////////
// 
// @file: find_binary.h
// @author: by mr.chen
// @date: 2014/5/14	14:21
// @brief: 二分查找
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __FIND_BINARY_H_
#define __FIND_BINARY_H_

namespace algorithm
{
	//二分查找
	template <class T>
	int find_binary(T array[],int length,T key)
	{
		int low=0,high=length-1,mid=0;

		while(low <= high)
		{
			mid=(low+high)/2;
			if(array[mid]==key) return mid;
			else if(array[mid]<key) low=mid+1;
			else high=mid-1;
		}
		return -1;
	}
};	//namespace algorithm

using namespace algorithm;

#endif	// __FIND_BINARY_H_
