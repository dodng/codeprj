////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_heap.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:08
// @brief: ������
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_HEAP_H_
#define __SORT_HEAP_H_

#include "algorithm.h"

namespace algorithm
{
	// �ѵ���
	// ��֪array[s...m]�еļ�¼����array[s]֮�������ѵĶ��壬
	// ����HeapAdjust()ʹarray[s...m]������һ���󶥶ѡ�
	template<class T>
	void	adjust_heap(T array[],int s,int m)
	{
		int rc=array[s];
		for(int j=2*s+1;j<=m;j=2*j+1)
		{
			if(j<m && array[j]<array[j+1]) j++;
			if(rc >= array[j]) break;
			array[s]=array[j];
			s=j;
		}
		array[s]=rc;
	}

	// ��˳���array���ж�����
	template<class T>
	void	sort_heap(T array[],int length)
	{
		for(int i=length/2;i>=0;i--)
			adjust_heap(array,i,length-1);
		for(int i=length-1;i>0;i--)
		{
			swap<T>(array[i],array[0]);
			adjust_heap(array,0,i-1);
		}
	}

};	// namespace algorithm
using namespace algorithm;

#endif	// __SORT_HEAP_H_
