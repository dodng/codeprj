////////////////////////////////////////////////////////////////////////////////////
// 
// @file: assert.h
// @author: by Mr.Chen
// @date: 2014/5/11	01:37
// @brief: assert
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __ASSERT_H_
#define __ASSERT_H_

#include "type.h"

namespace sysUtil {

	const int MAX_SHOWMESSAGE_LENGTH = 1024; //服务器对话框长度
	////////////////////////////////////////////////////////////////////////////////
	//	assert
	////////////////////////////////////////////////////////////////////////////////
	//异常
	void	__assert__(const char* file, uint line, const char* myFunction, const char* myException);
	void	__assertex__(const char* file, uint line, const char* myFunction, const char* myException, const char* msg);
	void	__assertspecial__(const char* file, uint line, const char* myFunction, const char* myException, const char* msg, ...);

	#if defined(_WIN32)
	#define Assert(myException) ((void)((myException)?0:(__assert__(__FILE__, __LINE__, __FUNCTION__, #myException), 0)))
	#define AssertEx(myException, msg) ((void)((myException)?0:(__assertex__(__FILE__, __LINE__, __FUNCTION__, #myException, msg), 0)))
	#define AssertSpecial(myException, msg, ...) ((void)((myException)?0:(__assertspecial__(__FILE__, __LINE__, __FUNCTION__, #myException, msg, __VA_ARGS__),0)))
	#endif

	#if defined(_LINUX64)
	#define Assert(myException) {if(!(myException)){__assert__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #myException); }}
	#define AssertEx(myException, msg) {if(!(myException)){__assertex__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #myException, msg); }}
	#define AssertSpecial(myException, msg, ...) {if(!(myException)){__assertspecial__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #myException, msg, ##__VA_ARGS__);}}
	#endif

	////////////////////////////////////////////////////////////////////////////////
	//	try...catch
	////////////////////////////////////////////////////////////////////////////////
	#if defined(_WIN32)
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(false,__FUNCTION__);}}
	#endif

	#if defined(_LINUX64)
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(false,__PRETTY_FUNCTION__);}}
	#endif

	#define __MYTRY try
	#define __MYCATCH catch(...)

}// namespace sysUtil
using namespace sysUtil;

#endif
