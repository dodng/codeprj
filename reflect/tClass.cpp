#include "tClass.h"
#include "tNode.h"

namespace reflect {

	tNode*	tClass::CreateObject(const char* szName)	//!< ��������һ��ʵ���ĺ���
	{
		if(pfnCreateObject == NULL) return NULL;
		tNode *pObject = (*pfnCreateObject)();
		pObject->m_strName = szName;
		return pObject;
	}

} // namespace reflect
