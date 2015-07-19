
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

// 注册所有工厂
void FactoryMgr::_registerFactory()
{
	// 加法工厂
	IFactory *pOperFactory = new AddFactory();
	addFactory(pOperFactory);
	// 减法工厂
	pOperFactory = new SubFactory();
	addFactory(pOperFactory);
	// 乘法工厂
	pOperFactory = new MulFactory();
	addFactory(pOperFactory);
	// 除法工厂
	pOperFactory = new DivFactory();
	addFactory(pOperFactory);
}

void test()
{
	//// 加法运算
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
	// 可以利用某种方法，使得该加法工厂只被new一次，再封装一层
	// 例如可写个FacrotyMgr，管理所有的工厂，这里需要增加个ID
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

