////////////////////////////////////////////////////////////////////////////////////
// 
// @file: memoryStatistic.h
// @author: by Mr.Chen
// @date: 2015/4/16	17:26
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __MEMORY_STATISTIC_H_
#define __MEMORY_STATISTIC_H_

#include <stdlib.h>
#include <iostream>
#include <list>

using std::list;
using std::endl;
using std::cout;
using std::bad_alloc;

// �ڴ�ڵ�
class Node
{
public:
	Node();
	Node(void* Buffer, size_t s, char* File, int Line);
	Node(const Node& Temp);

public:
	void*	m_pObject;		//�ڴ������õ�ָ��
	size_t	m_nSize;			//�ڴ����Ĵ�С
	char*	m_szFileName;	//�ڴ������������ļ�λ��
	int		m_nLineNum;		//�ڴ������������к�
};

// �ڴ�ڵ���
class NodeList
{
public:
	bool isEmpty();
	void add(const Node& Temp);
	void remove(void* Object);
	void check();

public:
	list<Node> m_List;
};

// typedef
typedef list<Node>::iterator NodeListIter;


// ����
extern void* operator new(size_t Size, char* FileName, int LineNum);
extern void* operator new[](size_t Size, char* FileName, int LineNum);
extern void operator delete(void* Object);
extern void operator delete[](void* Object);

//
extern void check();
extern NodeList g_memNodeList;

#define new new(__FILE__, __LINE__ )

//#if defined (_WIN32)
//#ifdef _DEBUG 
//#define DEBUG_CLIENTBLOCK  new( _CLIENT_BLOCK, __FILE__, __LINE__) 
//#else
//#define DEBUG_CLIENTBLOCK
//#endif
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h> 
//#ifdef _DEBUG 
//#define new DEBUG_CLIENTBLOCK 
//#endif
//#endif

#endif

