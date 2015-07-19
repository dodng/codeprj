////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_algorithm.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:16
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include <iostream>
using std::cout;
using std::endl;

#include "sort_bubble.h"
#include "sort_heap.h"
#include "sort_insert.h"
#include "sort_merge.h"
#include "sort_quick.h"
#include "sort_radix.h"
#include "sort_sel.h"
#include "find_binary.h"
#include "find_Kth.h"
#include "hash_table.h"
#include "rbtree.h"
#include "md5.h"

template <class T>
void Output(T *array, int length)
{
	for (int i=0; i<length; ++i)
		std::cout<<array[i]<<" ";
	cout<<endl;
}

void test_algorithm();

