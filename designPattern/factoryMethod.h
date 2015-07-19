////////////////////////////////////////////////////////////////////////////////////
// 
// @file: factoryMethod.h
// @author: by Mr.Chen
// @date: 2014/8/14	15:28
// @brief: ��������ģʽ����չ����-�޸ķ�գ�
//	ע������򵥹���ģʽ������
//	1��(��)����ģʽ��ֻ��һ��������ȥ������Ӽ��˳���������ʵ����
//	2����������ģʽ���ж���������ֱ��мӼ��˳��ȹ��������ø��ԵĹ���
//		ȥ��������Ե�������ʵ����
//	3�����Խ�Ϲ���������ʹ�á�
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _FACTORY_METHOD_H
#define _FACTORY_METHOD_H

#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

namespace FactoryMethodPattern {

////////////////////////////////////////////
/// �������
////////////////////////////////////////////
class Operation
{
public:
	virtual double getResult() { return 0; }
public:
	void	setA(double val) { _memberA = val; }
	double	getA() { return _memberA; }
	void	setB(double val) { _memberB = val; }
	double	getB() { return _memberB; }
protected:
	double _memberA;
	double _memberB;
};

// �ӷ�������
class OperationAdd : public Operation
{
public:
	double getResult()
	{
		return _memberA + _memberB;
	}
};

// ����������
class OperationSub : public Operation
{
	double getResult()
	{
		return _memberA - _memberB;
	}
};

// �˷�������
class OperationMul : public Operation
{
	double getResult()
	{
		return _memberA * _memberB;
	}
};

// ����������
class OperationDiv : public Operation
{
	double getResult()
	{
		double result = 0.0;
		if (_memberB == 0)
		{
			throw;// new exception("��������Ϊ0");
		}
		result = _memberA / _memberB;
		return result;
	}
};

////////////////////////////////////////////
/// ���������
////////////////////////////////////////////
class IFactory
{
public:
	virtual Operation* createOperate() = 0;
	virtual int getID() = 0;
};

enum
{
	OPER_INVALID = -1,
	OPER_ADD,
	OPER_SUB,
	OPER_MUL,
	OPER_DIV,
	OPER_NUM,
};

// �ӷ�����
class AddFactory : public IFactory
{
public:
	Operation* createOperate()
	{
		return new OperationAdd();
	}
	int getID()
	{
		return OPER_ADD;
	}
};

// ��������
class SubFactory : public IFactory
{
public:
	Operation* createOperate()
	{
		return new OperationSub();
	}
	int getID()
	{
		return OPER_SUB;
	}
};

// �˷�����
class MulFactory : public IFactory
{
public:
	Operation* createOperate()
	{
		return  new OperationMul();
	}
	int getID()
	{
		return OPER_MUL;
	}
};

// ��������
class DivFactory : public IFactory
{
	Operation* createOperate()
	{
		return new OperationDiv();
	}
	int getID()
	{
		return OPER_DIV;
	}
};

// ����������
class FactoryMgr
{
public:
	FactoryMgr();
	~FactoryMgr();
	void addFactory(IFactory* pFactory);
	Operation* createOperate(int id);

private:
	void _registerFactory();

private:
	IFactory** pFactoryArry;
};


///////////////////////////////////////////
///����������ģʽ�в�ֹһ���Ʒʱ����Operation��һ���Ʒ��
///��ͱ���˳��󹤳�ģʽ
///////////////////////////////////////////
// User���ݱ�ṹ
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
// Department���ݱ�ṹ
class Department
{
public:
	int GetID() { return _id; }
	void SetID(int id) { _id = id; }
private:
	int _id;
};

// DB-User�������
class IUser
{
public:
	virtual void Insert(User* user) = 0;
	virtual User* GetUser(int id) = 0;
};
// DB-Department�������
class IDepartment
{
public:
	virtual void Insert(Department* department) = 0;
	virtual Department* GetDepartment(int id) = 0;
};
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
};
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
};
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
};
// Access��User��
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
};

// ���������
class IDBFactory
{
public:
	virtual IUser* CreateUser() = 0;
	virtual IDepartment* CreateDepartment() = 0;
};
// 
class SqlServerFactory : public IDBFactory
{
public:
	IUser* CreateUser()
	{
		return new SqlServerUser();
	}
	IDepartment* CreateDepartment()
	{
		return new SqlServerDepartment();
	}
};
//
class AccessFactory : public IDBFactory
{
public:
	IUser* CreateUser()
	{
		return new AccessUser();
	}
	IDepartment* CreateDepartment()
	{
		return new AccessDepartment();
	}
};

void test();

} //namespace FactoryMethodPattern
using namespace FactoryMethodPattern;

#endif // _FACTORY_METHOD_H
