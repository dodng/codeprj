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

//�����ӽڵ�,���û���ҵ�,����NULL;
tNode*	tNode::lookUpChild(const char* szNodeName)
{
	register ChildList::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if((*it)->getNodeName() == szNodeName) return (tNode*)*it;
	}
	return NULL;
}

//����ӽڵ㵽�ýڵ���
void tNode::addChild(tNode *pNewNode)
{
	if(!pNewNode) return;
	(pNewNode->m_pParentNode) = this;
	m_ChildBuf.push_back(pNewNode);
}

//ɾ��ĳ���ӽڵ�,���ýڵ����ʱ����TRUE,���򷵻�FALSE
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

//ɾ�������ӽڵ�
void tNode::eraseAllChild()
{
	if(m_ChildBuf.empty()) return ;
	m_ChildBuf.clear();
}

//�ͷ��Լ�����ӵ�е���Դ
void tNode::release()
{
	if(m_ChildBuf.empty()) return;
	//�ͷ������ӽڵ�
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
