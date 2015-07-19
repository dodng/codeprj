////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_quick.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:10
// @brief: ��������
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_Q_H_
#define __SORT_Q_H_

#include "algorithm.h"

namespace algorithm
{
	//////////////////////////////////////////////////////////////////////////////////////
	/// ����λ��
	//////////////////////////////////////////////////////////////////////////////////////
	template <class T>
	int	 median(T array[], int low, int mid, int high)
	{
		if(array[low] <= array[high])
		{
			if(array[low] <= array[mid])
			{
				if(array[mid] <= array[high]) return mid;
				else return high;
			}
			else return low;
		}
		else
		{
			if(array[high] <= array[mid])
			{
				if(array[mid] <= array[high]) return mid;
				else return high;
			}
			else return high;
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// �ֻ��ֳ�2��
	/////////////////////////////////////////////////////////////////////////////
	template <class T>
	int	 partition(T array[], int low, int high) 
	{
		//ѡ��E[low],E[(low+high)/2],E[high]���м�ֵ��Ϊpivot
		int pivotLoc = median(array, low, (low+high)/2, high);
		swap<T>(array[pivotLoc], array[low]);
		T pivot = array[low];
		while(low < high)
		{
			while(low<high && array[high]>=pivot) --high;
			array[low] = array[high];
			while(low<high && array[low]<=pivot) ++low;
			array[high] = array[low];
		}
		array[low] = pivot;
		return low;
	}

	//////////////////////////////////////////////////////////////////////
	/// �������� version 1
	//////////////////////////////////////////////////////////////////////
	template<class T>
	void	qsort_V1(T array[], int low, int high)
	{
		//��a[0...n-1]����
		if(low < high)
		{
			int pivotLoc = partition(array,low,high);
			qsort_V1(array,low,pivotLoc-1);
			qsort_V1(array,pivotLoc+1,high);
		}
	}

	//////////////////////////////////////////////////////////////////////
	/// �������� version 2
	//////////////////////////////////////////////////////////////////////
	template<class T>
	void	qsort_V2(T array[], int first, int last)
	{
		//ֻ�Զ̵������н��еݹ飬���ٵݹ�ջ���
		//��a[0...n-1]����
		//first2...last2��ʾ���Σ�first1...last1��ʾ�̶�
		int first1, last1, first2, last2;

		first2 = first;
		last2 = last;
		while(last2 > first2) //�����õ���
		{
			//ѡ��E[first2],E[(first2+last2)/2],E[last2]���м�ֵ��Ϊpivot
			int splitPoint = partition(array, first2, last2);
			if(splitPoint < (first2 + last2)/2)
			{
				//���̶θ�ֵ
				first1 = first2;
				last1 = splitPoint-1;
				//�����θ�ֵ
				first2 = splitPoint+1;
				last2 = last2;
			}
			else
			{
				//���̶θ�ֵ
				first1 = splitPoint+1;
				last1 = last2;
				//�����θ�ֵ
				first2 = first2;
				last2 = splitPoint-1;
			}
			qsort_V2(array, first1, last1); //�̶��õݹ�
		}
	}

};	// namespace algorithm
using namespace algorithm;

#endif	// __SORT_Q_H_

