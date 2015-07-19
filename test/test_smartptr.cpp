////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_smartptr.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 1:27
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_smartptr.h"
#include "assert.h"
#include <stdio.h>

void test_shareptr()
{
	SharedPtr<int> sp(new int(10));                //一个指向整数的shared_ptr    
	Assert(sp.unique());                            //现在shared_ptr是指针的唯一持有者     
	SharedPtr<int> sp2 = sp;                       //第二个shared_ptr,拷贝构造函数     
	Assert(sp == sp2 && sp.useCount() == 2);       //两个shared_ptr相等,指向同一个对象,引用计数为2    
	*sp2 = 100;                                     //使用解引用操作符修改被指对象    
	Assert(*sp == 100);                             //另一个shared_ptr也同时被修改     
}

namespace sysUtil
{
	template<>
	void SharedPtr<FILE>::destroy()
	{
    		if(pRep)
        		fclose(pRep);
	}
}

bool test_shareptr2()
{
    FILE* fp = ::fopen("./data/sharefileptr.txt", "w+");
    if(!fp) return false;
    SharedPtr<FILE> obj(fp);
    int a = 0;
    if(a==0)
        return false;
    return true;
}
