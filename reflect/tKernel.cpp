#include "tKernel.h"
#include "strUtil.h"
#include <vector>
using std::vector;

WX_IMPLEMENT_SINGLEON(reflect::tKernel)

namespace reflect {

tKernel::tKernel() : m_theRoot("root"), m_pFirstClass(NULL)
{}

tKernel::~tKernel()
{}

//将某个类注册到全局类检索表函数
void tKernel::registerClass(tClass* pNewClass)
{
	if(!pNewClass) return;

	if(m_ClassMap[string(pNewClass->szClassName)] != NULL)
	{
		throw("Multi register class:%s", pNewClass->szClassName);
	}
	pNewClass->pNextClass = m_pFirstClass;
	m_pFirstClass = pNewClass;
	//将新的类加入到类检索表
	(m_ClassMap)[pNewClass->szClassName] = pNewClass;
}

//检查两个类是否具有继承关系
bool tKernel::isKindOf(const tClass* pThisClass, const tClass* pBaseClass) const
{
	if(!pBaseClass || !pThisClass) return false;

	register const tClass *pClass = pThisClass;
	while(pClass != NULL)
	{
		if(pClass == pBaseClass)
		{
			return true;
		}
		pClass = pClass->pBaseClass;
	}
	return false;
}

//根据字符串形式类名取得某定义类
tClass* tKernel::openClass(const char* szClassName)
{
	map<string,tClass*>::iterator it = m_ClassMap.find(szClassName);
	if(it == m_ClassMap.end()) return NULL;
	else return it->second;
}

//根据字符串形式的路径来创建新节点
tNode*	tKernel::newNode(const char* szClassName, const char* szPosition, const char* szNodeName)
{
	if(!szClassName || !szPosition || !szNodeName) return NULL;

	//打开类
	tClass *pClass = openClass(szClassName);
	if(!pClass)
	{
		throw("(tKernel::NewNode)Can't create new Node[%s]", szClassName);
	}

	vector<string> vPathSplitBuf;
	ConvertStringToVector(szPosition, vPathSplitBuf, "\\/");

	tNode *pCurrentNode = &m_theRoot;
	tNode *pFindNode = NULL;

	//搜索父节点
	for(register int i=0; i<(int)vPathSplitBuf.size(); i++)
	{
		tNode *pFindNode = pCurrentNode->lookUpChild(vPathSplitBuf[i].c_str());
		if(pFindNode == NULL)
		{
			tNode *pNewNode = new tNode(vPathSplitBuf[i].c_str());
			pCurrentNode->addChild(pNewNode);
			pFindNode = pNewNode;
		}
		pCurrentNode = pFindNode;
	}

	//查找是否该节点已经存在
	if(pCurrentNode->lookUpChild(szNodeName))
	{
		throw("(tKernel::NewNode)The Node[%s] has exist!", szNodeName);
	}

	tNode *pNewNode = (tNode*)pClass->CreateObject(szNodeName);
	pCurrentNode->addChild(pNewNode);
	return pNewNode;
}

//根据父节点加入新的节点
tNode*	tKernel::newNode(const char* szClassName, tNode* pParentNode, const char* szNodeName)
{
	if(!szClassName || !pParentNode || !szNodeName) return NULL;

	//打开类
	tClass *pClass = openClass(szClassName);
	if(!pClass)
	{
		throw("(tKernel::NewNode)Can't create new Node[%s]", szClassName);
	}

	//查找是否该节点已经存在
	if(pParentNode->lookUpChild(szNodeName))
	{
		throw("(tKernel::NewNode)The Node[%s] has exist!", szNodeName);
	}

	tNode *pNewNode = (tNode *)pClass->CreateObject(szNodeName);
	pParentNode->addChild(pNewNode);

	return pNewNode;
}


tNode*	tKernel::getNode(const char* szPathName)
{
	if(!szPathName) return NULL;

	std::vector<string> vPathSplitBuf;
	ConvertStringToVector(szPathName, vPathSplitBuf, "\\/");
	if(vPathSplitBuf.empty()) return NULL;

	tNode *pCurrentNode = &m_theRoot;
	for(register int i=0; i<(int)vPathSplitBuf.size(); i++)
	{
		tNode *pFindNode = pCurrentNode->lookUpChild(vPathSplitBuf[i].c_str());
		if(pFindNode == NULL)
		{
			return NULL;
		}
		pCurrentNode = pFindNode;
	}

	return pCurrentNode;
}

} //namespace reflect
