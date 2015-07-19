////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_designpattern.cpp
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:08
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_designpattern.h"

//test singleton and MLock
WX_IMPLEMENT_SINGLEON(TestSingleton)
void test_single_pattern()
{
	TestSingleton::getSinglePtr()->test();
}

//策略模式
void test_strategy_pattern()
{
	StrategyPattern::test();
}

//工厂模式
void test_factory_pattern()
{
	FactoryPattern::test();
}

//工厂方法模式
void test_factory_method_pattern()
{
	FactoryMethodPattern::test();
}

//抽象工厂模式
void test_factory_abstract_pattern()
{
	FactoryAbstractPattern::test();
}

//观察者模式
void test_observer_pattern()
{
    ObserverPattern::test();
}
