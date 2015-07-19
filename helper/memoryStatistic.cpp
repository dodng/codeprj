/////////////////////////////////////////////
#if defined _USE_MEM_STATIS_
/////////////////////////////////////////////

#include "memoryStatistic.h"
#include <stdio.h>
#undef new

Node::Node()
{
    m_pObject=0;
    m_szFileName=0;
}

Node::Node(void* Buffer, size_t s, char* File, int Line)
{
    m_pObject=Buffer;
    m_nSize=s;
    m_szFileName=File;
    m_nLineNum=Line;
}

Node::Node(const Node& Temp)
{
    m_szFileName=Temp.m_szFileName;
    m_nLineNum=Temp.m_nLineNum;
    m_pObject=Temp.m_pObject;
    m_nSize=Temp.m_nSize;
}

bool NodeList::isEmpty()
{
    if (m_List.size() > 0) return false;
    else return true;
}

void NodeList::add(const Node& Temp)
{
    m_List.push_back(Temp);
}

void NodeList::remove(void* pObject)
{
    NodeListIter iter = m_List.begin();
    NodeListIter endIter = m_List.end();
    for (; iter != endIter; ++iter)
    {
	    if (iter->m_pObject == pObject)
	    {
		    m_List.erase(iter);
		    break;
	    }
    }
}

void NodeList::check()
{
    if (!isEmpty())
    {
        cout << "´æÔÚÄÚ´æÐ¹Â©" << endl;
        cout << "Line\tSize\tPath" << endl;
	    NodeListIter iter = m_List.begin();
	    NodeListIter endIter = m_List.end();
	    char s[200] = {0};
	    for (; iter != endIter; ++iter)
	    {
		    char Line[20];
		    itoa(iter->m_nLineNum, Line, 10);
		    cout << Line;
		    cout << "\t";
		    memset(s, 0, sizeof(s));
		    itoa(iter->m_nSize, s, 10);
		    cout << s;
		    cout << "\t";
		    cout << iter->m_szFileName;
		    cout << "\t";
	    }
    }
    else
    {
	    cout << "²»´æÔÚÄÚ´æÐ¹Â©";
    }
}

NodeList g_memNodeList;

void* operator new(size_t Size, char* FileName, int LineNum)
{
    if (Size==(size_t)0) Size=1;
    void* Result=::operator new(Size);
    if (!Result) throw bad_alloc();
    else
    {
#if defined _USE_MEM_STATIS_
        Node Temp(Result, Size, FileName, LineNum);
        g_memNodeList.add(Temp);
#endif
    }
    return Result;
}

void* operator new[](size_t Size, char* FileName, int LineNum)
{
   if (Size==(size_t)0) Size=1;
   void* Result=::operator new[](Size);
   if (!Result) throw bad_alloc();
   else
   {
#if defined _USE_MEM_STATIS_
        Node Temp(Result, Size, FileName, LineNum);
        g_memNodeList.add(Temp);
#endif
    }
    return Result;
}

void operator delete(void* Object)
{
    if (!Object) return;
#if defined _USE_MEM_STATIS_
    g_memNodeList.remove(Object);
#endif
    //::delete(Object);
    free(Object);
}

void operator delete[](void* Object)
{
    if (!Object) return;
#if defined _USE_MEM_STATIS_
    g_memNodeList.remove(Object);
#endif
    //::operator delete[](Object);
    free(Object);
}

void check()
{
    g_memNodeList.check();
}

/////////////////////////////////////////////
#endif  //end of file
/////////////////////////////////////////////