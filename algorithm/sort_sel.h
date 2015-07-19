////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_sel.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:11
// @brief: —°‘Ò≈≈–Ú
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __SORT_SEL_H__
#define __SORT_SEL_H__

#include "algorithm.h"

namespace algorithm
{
	// —°‘Ò≈≈–Ú
	template<class T>
	void	sort_select(T array[],int length)
	{
		int i,j,k;
		for(i=0;i<length;i++)
		{
			j=i;
			for(k=i+1;k<length;k++)
			{
				if(array[k]<array[j])
					j=k;
			}
			if(j!=i)
				swap<T>(array[i],array[j]);
		}
	}
	
};	// namespace algorithm

using namespace algorithm;

#endif	// __SORT_SEL_H__
