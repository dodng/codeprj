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

//单例模式测试
class TestSingleton : public Singleton<TestSingleton>
{
public:
	void test() {
		cout <<"test singleton and MLock." << endl;
	}
};
void test_single_pattern();

//策略模式测试
void test_strategy_pattern();

//简单工厂模式
void test_factory_pattern();

//工厂方法模式
void test_factory_method_pattern();

//抽象工厂模式
void test_factory_abstract_pattern();

//观察者模式
void test_observer_pattern();

