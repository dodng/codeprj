////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_merge.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:09
// @brief: 归并排序
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_MERGE_H_
#define __SORT_MERGE_H_

namespace algorithm
{
	// 归并两个序列
	template<class T>
	void	merge(T array[], int first, int mid, int last, T temp[])
	{
		int i = first, j = mid + 1;
		int m = mid, n = last;
		int k = 0;
		while (i <= m && j <= n)  
		{
			if (array[i] < array[j]) 
				temp[k++] = array[i++];
			else
				temp[k++] = array[j++];
		}
		while (i <= m)
			temp[k++] = array[i++];
		while (j <= n)
			temp[k++] = array[j++];

		for (i = 0; i < k; i++)
			array[first + i] = temp[i];
	}

	template<class T>
	void	sort_merge(T array[], int first, int last, T temp[])
	{
		if (first < last)
		{
			int mid = (first + last) / 2;
			sort_merge<T>(array, first, mid, temp); 
			sort_merge<T>(array, mid + 1, last, temp);
			merge<T>(array, first, mid, last, temp);
		}
	}

	// 归并排序
	template<class T>
	void	sort_merge(T array[], int length)
	{
		T* temp = new T[length];
		sort_merge<T>(array, 0, length-1, temp);
		delete[] temp;
		temp = NULL;
	}
	
};	// namespace algorithm
using namespace algorithm;

#endif	// __SORT_MERGE_H_

