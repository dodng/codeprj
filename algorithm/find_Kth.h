////////////////////////////////////////////////////////////////////////////////////
// 
// @file: find_Kth.h
// @author: by Mr.Chen
// @date: 2014/5/14	14:22
// @brief: ���ҵ�kС����
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __FIND_KTH_H_
#define __FIND_KTH_H_

#include "sort_quick.h"

namespace algorithm
{
	//���ҵ�k��С��Ԫ��
	template <class T>
	T find_Kth(T array[], int first, int last, int k)
	{
		T ans;
		if (first == last)
			ans = array[first];
		else
		{
			int splitPoint = partition<T>(array, first, last);
			if (k < splitPoint)
				ans = find_Kth(array, first, splitPoint-1, k);
			else if (k > splitPoint)
				ans = find_Kth(array, splitPoint+1, last, k);//ע��k���±꣬��˲�Ҫд��k-split
			else
				ans = array[splitPoint];
		}
		return ans;
	}
};	// namespace algorithm

using namespace algorithm;

#endif	// __FIND_KTH_H_
