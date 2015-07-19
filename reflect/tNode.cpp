#include "tNode.h"

namespace reflect {

WX_IMPLEMENT_DYNAMIC(tNode, NULL);

tNode::tNode() : m_pParentNode(NULL)
{
}

tNode::tNode(const char* szName)
{ 
	m_strName = szName;	
}

//查找子节点,如果没有找到,返回NULL;
tNode*	tNode::lookUpChild(const char* szNodeName)
{
	register ChildList::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if((*it)->getNodeName() == szNodeName) return (tNode*)*it;
	}
	return NULL;
}

//添加子节点到该节点上
void tNode::addChild(tNode *pNewNode)
{
	if(!pNewNode) return;
	(pNewNode->m_pParentNode) = this;
	m_ChildBuf.push_back(pNewNode);
}

//删除某个子节点,当该节点存在时返回TRUE,否则返回FALSE
bool tNode::eraseChild(tNode* pNode)
{
	if(m_ChildBuf.empty()) return false;
	register ChildList::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if(pNode == *it)
		{
			m_ChildBuf.erase(it);
			return true;
		}
	}
	return false;
}

//删除所有子节点
void tNode::eraseAllChild()
{
	if(m_ChildBuf.empty()) return ;
	m_ChildBuf.clear();
}

//释放自己所所拥有的资源
void tNode::release()
{
	if(m_ChildBuf.empty()) return;
	//释放所有子节点
	register ChildList::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		(*it)->release();
		delete (*it);
		*it = NULL;
	}
	m_ChildBuf.clear();
}

void tNode::tick()
{
	if(m_ChildBuf.empty()) return;

	register ChildList::iterator it = m_ChildBuf.end(), itNext;
	it--;

	do
	{
		itNext = it;
		if(itNext == m_ChildBuf.begin()) itNext = m_ChildBuf.end();
		else itNext--;

		((tNode*)(*it))->tick();

		if(itNext == m_ChildBuf.end()) break;

		it = itNext;
	}while(true);
}

} // namespace reflect
