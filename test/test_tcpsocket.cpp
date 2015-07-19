////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_tcpsocket.cpp
// @author: by Mr.Chen
// @date: 2015/4/23/ 0:22
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_tcpsocket.h"

int test_tcpsocketIO()
{
#ifdef _WIN32

	//SelectServer::startSrv();
	//MultiThreadSelectServer::startSrv();
	//EventSelect::startSrv();
	//EventOverlap::startSrv();
	//CompletionOverlap::startSrv();
	CompletionPort::startSrv();

#endif

	return 0;
}
