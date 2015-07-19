////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_designpattern.h
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:05
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include <iostream>
using std::endl;
using std::cout;
#include "singleton.h"
#include "strategy.h"
#include "factory.h"
#include "factoryMethod.h"
#include "factoryAbstract.h"
#include "observer.h"

//����ģʽ����
class TestSingleton : public Singleton<TestSingleton>
{
public:
	void test() {
		cout <<"test singleton and MLock." << endl;
	}
};
void test_single_pattern();

//����ģʽ����
void test_strategy_pattern();

//�򵥹���ģʽ
void test_factory_pattern();

//��������ģʽ
void test_factory_method_pattern();

//���󹤳�ģʽ
void test_factory_abstract_pattern();

//�۲���ģʽ
void test_observer_pattern();

