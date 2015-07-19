////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factoryAbstract.h
// @author: by Mr.Chen
// @date: 2014/8/14	15:57
// @brief: 抽象工厂模式（使用反射机制&简单工厂模式来改进抽象工厂模式）
//		当工厂方法模式中不止一类产品时候（如Operation是一类产品），就变成了抽象工厂模式
//		例如，下面程序例子
//		此时，IFactory接口中创建所有产品（User和Department）的抽象方法。
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _FACTORY_ABSTRACT_H
#define _FACTORY_ABSTRACT_H

#include "tNode.h"
#include "tKernel.h"
#include "tClass.h"
#include "strUtil.h"
#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;

namespace FactoryAbstractPattern {

// User表结构
class User
{
public:
	int GetID() { return _id; }
	void SetID(int id) { _id = id; }
	string GetName() { return _name; }
	void SetName(string name) { _name = name; }
private:
	int _id;
	string _name;
};

// Department表结构
class Department
{
public:
	int GetID() { return _id; }
	void SetID(int id) { _id = id; }
private:
	int _id;
};

// User表虚基类（利用tNode实现C++反射）
class IUser : public tNode
{
public:
	virtual void Insert(User* user) = 0;
	virtual User* GetUser(int id) = 0;
	WX_DECLARE_DYNAMIC(IUser);
};
//WX_IMPLEMENT_DYNAMIC_VIRTUAL(IUser, GETCLASS(tNode));

// Department表虚基类（利用tNode实现C++反射）
class IDepartment : public tNode
{
public:
	virtual void Insert(Department* department) = 0;
	virtual Department* GetDepartment(int id) = 0;
	WX_DECLARE_DYNAMIC(IDepartment);
};
//WX_IMPLEMENT_DYNAMIC_VIRTUAL(IDepartment, GETCLASS(tNode));

// SqlServer的User表
class SqlServerUser : public IUser
{
public:
	void Insert(User* user)
	{
		cout << "在SqlServer中给User表中增加一条记录" <<endl;
	}
	User* GetUser(int id)
	{
		cout << "在SqlServer中根据ID得到User表中的一条记录" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(SqlServerUser);
};
//WX_IMPLEMENT_DYNAMIC(SqlServerUser, GETCLASS(IUser));

// SqlServer的Department表
class SqlServerDepartment : public IDepartment
{
public:
	void Insert(Department* department)
	{
		cout << "在SqlServer中给Department表中增加一条记录" <<endl;
	}
	Department* GetDepartment(int id)
	{
		cout << "在SqlServer中根据ID得到Department表中的一条记录" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(SqlServerDepartment);
};
//WX_IMPLEMENT_DYNAMIC(SqlServerDepartment, GETCLASS(IDepartment));

// Access的User表
class AccessUser : public IUser
{
public:
	void Insert(User* user)
	{
		cout << "在Access中给User表中增加一条记录" <<endl;
	}
	User* GetUser(int id)
	{
		cout << "在Access中根据ID得到User表中的一条记录" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(AccessUser);
};
//WX_IMPLEMENT_DYNAMIC(AccessUser, GETCLASS(IUser));

// Access的Department表
class AccessDepartment : public IDepartment
{
public:
	void Insert(Department* department)
	{
		cout << "在Access中给Department表中增加一条记录" <<endl;
	}
	Department* GetDepartment(int id)
	{
		cout << "在Access中根据ID得到Department表中的一条记录" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(AccessDepartment);
};
//WX_IMPLEMENT_DYNAMIC(AccessDepartment, GETCLASS(IDepartment));

// 使用反射的简单工厂模式
class FactoryUseReflect
{
public:
	static IUser* CreateUser();
	static IDepartment* CreateDepartment();

protected:
	static int GetFactoryID() 
	{ 
		static int nLastUID = 0; 
		return ++nLastUID; 
	}

private:
	static std::string DBNamePrefix;
};

void test();

} //namespace FactoryAbstractPattern
using namespace FactoryAbstractPattern;

#endif // _FACTORY_ABSTRACT_H

