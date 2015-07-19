#include "factoryAbstract.h"
#include "tKernel.h"

namespace FactoryAbstractPattern {


WX_IMPLEMENT_DYNAMIC_VIRTUAL(IUser, GETCLASS(tNode));
WX_IMPLEMENT_DYNAMIC_VIRTUAL(IDepartment, GETCLASS(tNode));

WX_IMPLEMENT_DYNAMIC(SqlServerUser, GETCLASS(IUser));
WX_IMPLEMENT_DYNAMIC(SqlServerDepartment, GETCLASS(IDepartment));

WX_IMPLEMENT_DYNAMIC(AccessUser, GETCLASS(IUser));
WX_IMPLEMENT_DYNAMIC(AccessDepartment, GETCLASS(IDepartment));

IUser* FactoryUseReflect::CreateUser()
{
	std::string className = DBNamePrefix + "User";
	char szName[512] = {0};
	strUtil::Snprintf(szName, 512, "%d", GetFactoryID());
	return (IUser*)(tKernel::getSinglePtr()->newNode(className.c_str(), "bin", szName));
}

IDepartment* FactoryUseReflect::CreateDepartment()
{
	std::string className = DBNamePrefix + "Department";
	char szName[512] = {0};
	strUtil::Snprintf(szName, 512, "%d", GetFactoryID());
	return (IDepartment*)(tKernel::getSinglePtr()->newNode(className.c_str(), "bin", szName));
}

// 这个可从配置文件中读取
std::string FactoryUseReflect::DBNamePrefix = "Access";

void test()
{
	using FactoryAbstractPattern::SqlServerUser;
	using FactoryAbstractPattern::SqlServerDepartment;
	using FactoryAbstractPattern::AccessUser;
	using FactoryAbstractPattern::AccessDepartment;
	// 注册类
	tKernel::getSinglePtr()->registerClass(GETCLASS(SqlServerUser));
	tKernel::getSinglePtr()->registerClass(GETCLASS(SqlServerDepartment));
	tKernel::getSinglePtr()->registerClass(GETCLASS(AccessUser));
	tKernel::getSinglePtr()->registerClass(GETCLASS(AccessDepartment));

	FactoryAbstractPattern::User user;
	FactoryAbstractPattern::Department dept;
	FactoryAbstractPattern::IUser *pUser = FactoryUseReflect::CreateUser();
	FactoryAbstractPattern::IDepartment *pDept = FactoryUseReflect::CreateDepartment();
	pUser->Insert(&user);
	pUser->GetUser(0);
	pDept->Insert(&dept);
	pDept->GetDepartment(0);
}

} //namespace FactoryAbstractPattern
