////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_algorithm.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:17
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_algorithm.h"
#include <string.h>

void test_algorithm()
{
	int a[] = {1,2,5,7,90,40,2000,0,100};
	int length = sizeof(a)/sizeof(int);
	Output<int>(a, length);
	int k = find_Kth<int>(a, 0, length-1, 3);
	std::cout<<k<<std::endl;

	sort_bubble_V3(a, length);
	sort_insert(a, length);
	sort_select(a, length);
	sort_merge(a, length);
	sort_heap<int>(a, length);
	qsort_V2(a, 0, length-1);
	sort_radix<4>(a, length);
	Output<int>(a, length);
	int index = find_binary<int>(a, length, 40);
	cout<<"find index= "<<index<<endl;
	index = find_binary<int>(a, length, 120);
	cout<<"find index= "<<index<<endl;

	HashTable<char*,int> studentTable(100);
	studentTable.Insert("张三",001);
	studentTable.Insert("李四",002);
	studentTable.Insert("王五",003);
	const int *pValue = studentTable.Find("李四");
	if (pValue)
	{
		int value = *pValue;
		cout << "value is : "<<value << endl;
	}

	rbtree<int8> t;
	t.insert(1);
	t.insert(7);
	t.insert(3);
	t.insert(2);
	t.print();

	char str[]="1328913394018403026f75eb3d0dd9a4c0fc80daa04ed99e39";
	//MD5 obj;
	//obj.update(str,strlen(str));
	MD5 obj(str,strlen(str));
	string md5 = obj.toString();

}
