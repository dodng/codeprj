
#include "factoryMethod.h"

namespace FactoryMethodPattern {

FactoryMgr::FactoryMgr()
{
	pFactoryArry = new IFactory*[OPER_NUM];
	for (int i = 0; i < OPER_NUM; i++)
	{
		pFactoryArry[i] = NULL;
	}
	_registerFactory();
}

FactoryMgr::~FactoryMgr()
{
	delete []pFactoryArry;
	pFactoryArry = NULL;
}

void FactoryMgr::addFactory(IFactory* pFactory)
{
	if (NULL == pFactory || NULL != pFactoryArry[pFactory->getID()])
	{
		return;
	}
	pFactoryArry[pFactory->getID()] = pFactory;
}

Operation* FactoryMgr::createOperate(int id)
{
	if (id <= OPER_INVALID || id >= OPER_NUM)
	{
		return NULL;
	}
	IFactory *pFactory = pFactoryArry[id];
	if (NULL == pFactory)
	{
		return NULL;
	}
	return pFactory->createOperate();
}

// ע�����й���
void FactoryMgr::_registerFactory()
{
	// �ӷ�����
	IFactory *pOperFactory = new AddFactory();
	addFactory(pOperFactory);
	// ��������
	pOperFactory = new SubFactory();
	addFactory(pOperFactory);
	// �˷�����
	pOperFactory = new MulFactory();
	addFactory(pOperFactory);
	// ��������
	pOperFactory = new DivFactory();
	addFactory(pOperFactory);
}

void test()
{
	//// �ӷ�����
	//IFactory *pOperFactory = new AddFactory();
	//double result = 0.0;
	//if (NULL != pOperFactory)
	//{
	//	Operation *pOperObj = pOperFactory->createOperate();
	//	if (NULL != pOperObj)
	//	{
	//		pOperObj->setA(1.0);
	//		pOperObj->setB(2.0);
	//		result = pOperObj->getResult();
	//	}
	//}
	// ��������ĳ�ַ�����ʹ�øüӷ�����ֻ��newһ�Σ��ٷ�װһ��
	// �����д��FacrotyMgr���������еĹ�����������Ҫ���Ӹ�ID
	FactoryMethodPattern::FactoryMgr factoryMgr;
	double result = 0.0;
	FactoryMethodPattern::Operation* pOperObj = factoryMgr.createOperate(OPER_ADD);
	if (NULL != pOperObj)
	{
		pOperObj->setA(1.0);
		pOperObj->setB(2.0);
		result = pOperObj->getResult();
	}

	FactoryMethodPattern::User user;
	FactoryMethodPattern::Department dept;
	IDBFactory *pFactory = new AccessFactory(); //new SqlServerFactory();
	FactoryMethodPattern::IUser *pUser = pFactory->CreateUser();
	pUser->Insert(&user);
	pUser->GetUser(0);
	FactoryMethodPattern::IDepartment* pDept = pFactory->CreateDepartment();
	pDept->Insert(&dept);
	pDept->GetDepartment(0);
}

} //namespace FactoryMethodPattern

