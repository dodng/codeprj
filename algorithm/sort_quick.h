////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_quick.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:10
// @brief: 快速排序
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_Q_H_
#define __SORT_Q_H_

#include "algorithm.h"

namespace algorithm
{
	//////////////////////////////////////////////////////////////////////////////////////
	/// 找中位数
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
	// 分划分成2段
	/////////////////////////////////////////////////////////////////////////////
	template <class T>
	int	 partition(T array[], int low, int high) 
	{
		//选择E[low],E[(low+high)/2],E[high]的中间值作为pivot
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
	/// 快速排序 version 1
	//////////////////////////////////////////////////////////////////////
	template<class T>
	void	qsort_V1(T array[], int low, int high)
	{
		//把a[0...n-1]排序
		if(low < high)
		{
			int pivotLoc = partition(array,low,high);
			qsort_V1(array,low,pivotLoc-1);
			qsort_V1(array,pivotLoc+1,high);
		}
	}

	//////////////////////////////////////////////////////////////////////
	/// 快速排序 version 2
	//////////////////////////////////////////////////////////////////////
	template<class T>
	void	qsort_V2(T array[], int first, int last)
	{
		//只对短的子序列进行递归，减少递归栈深度
		//把a[0...n-1]排序
		//first2...last2表示长段，first1...last1表示短段
		int first1, last1, first2, last2;

		first2 = first;
		last2 = last;
		while(last2 > first2) //长段用迭代
		{
			//选择E[first2],E[(first2+last2)/2],E[last2]的中间值作为pivot
			int splitPoint = partition(array, first2, last2);
			if(splitPoint < (first2 + last2)/2)
			{
				//给短段赋值
				first1 = first2;
				last1 = splitPoint-1;
				//给长段赋值
				first2 = splitPoint+1;
				last2 = last2;
			}
			else
			{
				//给短段赋值
				first1 = splitPoint+1;
				last1 = last2;
				//给长段赋值
				first2 = first2;
				last2 = splitPoint-1;
			}
			qsort_V2(array, first1, last1); //短段用递归
		}
	}

};	// namespace algorithm
using namespace algorithm;

#endif	// __SORT_Q_H_

