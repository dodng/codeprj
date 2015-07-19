#include <iostream>
using namespace std;
using std::string;
//first include
#include "socketdef.h"
#include "ssample.h"
//test SEH
#include "log.h"
#include "SEHFile.h"
#include "test_designpattern.h"
#include "test_bitset.h"
#include "test_cbfunc.h"
#include "test_mysql.h"
#include "test_smartptr.h"
#include "test_spider.h"
#include "test_tinyxml.h"
#include "test_reflectV2.h"

int main()
{
    LogSystem::getSinglePtr()->init(1024*1024);
	/*__try
	{
		int *pTest = NULL;
		int aaa = *pTest;
	}
	__except (
		X_ProcessException(GetExceptionInformation()), \
		TerminateProcess(GetCurrentProcess(), 0), \
		EXCEPTION_EXECUTE_HANDLER) 
	{}*/

/*	test_single_pattern();
	test_strategy_pattern();
	test_factory_pattern();
	test_factory_method_pattern();
	test_factory_abstract_pattern();
    test_observer_pattern();*/

    //test_cbfunc();
    //test_mysql();
    //test_shareptr2();
    //test_spider();
    test_reflectV2();

	cin.get();
	return 0;
}
