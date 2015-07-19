////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_insert.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:09
// @brief: ≤Â»Î≈≈–Ú
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_INSERT_H_
#define __SORT_INSERT_H_

namespace algorithm
{
	// ≤Â»Î≈≈–Ú
	template<class T>
	void	sort_insert(T array[],int length)
	{
		for(int i=1;i<length;i++)
		{
			T w=array[i];
			int j;
			for(j=i-1; j>=0&&w<array[j]; j--)
			{
				array[j+1]=array[j];
			}
			array[j+1]=w;
		}
	}
} // namespace algorithm
using namespace algorithm;

#endif	// __SORT_INSERT_H_

