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

//����ģʽ
void test_strategy_pattern()
{
	StrategyPattern::test();
}

//����ģʽ
void test_factory_pattern()
{
	FactoryPattern::test();
}

//��������ģʽ
void test_factory_method_pattern()
{
	FactoryMethodPattern::test();
}

//���󹤳�ģʽ
void test_factory_abstract_pattern()
{
	FactoryAbstractPattern::test();
}

//�۲���ģʽ
void test_observer_pattern()
{
    ObserverPattern::test();
}
