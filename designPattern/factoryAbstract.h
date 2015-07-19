////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factoryAbstract.h
// @author: by Mr.Chen
// @date: 2014/8/14	15:57
// @brief: ���󹤳�ģʽ��ʹ�÷������&�򵥹���ģʽ���Ľ����󹤳�ģʽ��
//		����������ģʽ�в�ֹһ���Ʒʱ����Operation��һ���Ʒ�����ͱ���˳��󹤳�ģʽ
//		���磬�����������
//		��ʱ��IFactory�ӿ��д������в�Ʒ��User��Department���ĳ��󷽷���
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

// User��ṹ
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

// Department��ṹ
class Department
{
public:
	int GetID() { return _id; }
	void SetID(int id) { _id = id; }
private:
	int _id;
};

// User������ࣨ����tNodeʵ��C++���䣩
class IUser : public tNode
{
public:
	virtual void Insert(User* user) = 0;
	virtual User* GetUser(int id) = 0;
	WX_DECLARE_DYNAMIC(IUser);
};
//WX_IMPLEMENT_DYNAMIC_VIRTUAL(IUser, GETCLASS(tNode));

// Department������ࣨ����tNodeʵ��C++���䣩
class IDepartment : public tNode
{
public:
	virtual void Insert(Department* department) = 0;
	virtual Department* GetDepartment(int id) = 0;
	WX_DECLARE_DYNAMIC(IDepartment);
};
//WX_IMPLEMENT_DYNAMIC_VIRTUAL(IDepartment, GETCLASS(tNode));

// SqlServer��User��
class SqlServerUser : public IUser
{
public:
	void Insert(User* user)
	{
		cout << "��SqlServer�и�User��������һ����¼" <<endl;
	}
	User* GetUser(int id)
	{
		cout << "��SqlServer�и���ID�õ�User���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(SqlServerUser);
};
//WX_IMPLEMENT_DYNAMIC(SqlServerUser, GETCLASS(IUser));

// SqlServer��Department��
class SqlServerDepartment : public IDepartment
{
public:
	void Insert(Department* department)
	{
		cout << "��SqlServer�и�Department��������һ����¼" <<endl;
	}
	Department* GetDepartment(int id)
	{
		cout << "��SqlServer�и���ID�õ�Department���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(SqlServerDepartment);
};
//WX_IMPLEMENT_DYNAMIC(SqlServerDepartment, GETCLASS(IDepartment));

// Access��User��
class AccessUser : public IUser
{
public:
	void Insert(User* user)
	{
		cout << "��Access�и�User��������һ����¼" <<endl;
	}
	User* GetUser(int id)
	{
		cout << "��Access�и���ID�õ�User���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(AccessUser);
};
//WX_IMPLEMENT_DYNAMIC(AccessUser, GETCLASS(IUser));

// Access��Department��
class AccessDepartment : public IDepartment
{
public:
	void Insert(Department* department)
	{
		cout << "��Access�и�Department��������һ����¼" <<endl;
	}
	Department* GetDepartment(int id)
	{
		cout << "��Access�и���ID�õ�Department���е�һ����¼" <<endl;
		return NULL;
	}
	WX_DECLARE_DYNAMIC(AccessDepartment);
};
//WX_IMPLEMENT_DYNAMIC(AccessDepartment, GETCLASS(IDepartment));

// ʹ�÷���ļ򵥹���ģʽ
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

