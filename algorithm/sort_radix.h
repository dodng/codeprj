////////////////////////////////////////////////////////////////////////////////////
// 
// @file: sort_radix.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:11
// @brief: ��������
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __SORT_RADIX_H_
#define __SORT_RADIX_H_

#include "assert.h"

namespace algorithm
{
	// 10����
	const int RADIX=10;

	// �������s�ĵ�mλ
	// m==0 ��ʾ���λ����
	// m==MAX_DIGITS-1 ��ʾ��λ����
	template<int MAX_DIGITS>
	int getkey(int s, int m)
	{
		Assert(m>=0 && m<MAX_DIGITS);
		int keys[RADIX] = {0};
		int i = MAX_DIGITS-1;
		while (s)
		{
			keys[i]=s%RADIX;
			s=s/RADIX;
			--i;
		}
		return keys[m];
	}

	// һ�˻��������ռ�&�ַ���
	template<int MAX_DIGITS>
	void	radixPass(int array1[],int array2[], int count[], int length, int m)
	{
		// Collect
		for(int key=0; key<RADIX; key++)
			count[key]=0;

		for(int i=0; i<length; i++)
		{
			int key = getkey<MAX_DIGITS>(array1[i], m);
			count[key]++;
		}

		for(int key=1; key<RADIX; key++)
			count[key]=count[key-1]+count[key];

		// Distribute
		for(int i=length-1; i>=0; i--)
		{
			int key = getkey<MAX_DIGITS>(array1[i], m);
			array2[count[key]-1]=array1[i];
			count[key]--;
		}
	}

	// ��������
	template<int MAX_DIGITS>
	void	sort_radix(int array[], int length)
	{
		int *arraytemp = new int[length];
		int *count = new int[RADIX];
		int i = MAX_DIGITS-1;
		bool bReverse = false;
		while(i >= 0)
		{
			if (!bReverse)
				radixPass<MAX_DIGITS>(array,arraytemp,count,length,i);
			else
				radixPass<MAX_DIGITS>(arraytemp,array,count,length,i);
			--i;
			bReverse = !bReverse;
		}
		if (bReverse)
		{
			for(int i=0; i<length; i++)
				array[i]=arraytemp[i];
		}
	}
	
};	// namespace algorithm

using namespace algorithm;

#endif	// __SORT_RADIX_H_

