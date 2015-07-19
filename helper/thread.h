////////////////////////////////////////////////////////////////////////////////////
// 
// @file: Thread.h
// @author: by mr.chen
// @date: 2014/5/1	13:35
// @brief: Ïß³Ì, ref from BAIDU and PWRD.
//          
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __THREAD_H_
#define __THREAD_H_

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#if defined(_WIN32)
#include <windows.h>
#include <process.h>

#elif defined(_LINUX64)
#include <pthread.h>
#endif

namespace sysUtil {

	class Thread
	{
	public:
		enum THREAD_STATUS
		{
			TS_SLEEP,	//Ë¯Ãß×´Ì¬
			TS_RUN,		//ÔËÐÐ×´Ì¬
			TS_STOP,	//ÍË³ö×´Ì¬
		};

	#if defined(_WIN32)
		typedef unsigned int ThreadId;
	#elif defined(_LINUX64)
		typedef pthread_t ThreadId;
	#endif

		typedef void* (* cb_t)(void* param);

		struct ThreadStartInfo
		{
			cb_t	function;
			void*	param;
			ThreadStartInfo() : function(NULL), param(NULL)
			{}
		};

	public:
		Thread();
		virtual ~Thread();

		int		start(cb_t function, void* param, bool startAtOnce=true);
		void	resume();
		void	wait();

		THREAD_STATUS	getStatus() const { return m_status; }
		ThreadId		getTid() const { return m_Id; }

	private:
	#if defined(_WIN32)
		HANDLE			m_handle;
	#endif
		THREAD_STATUS	m_status;
		ThreadId		m_Id;
		ThreadStartInfo	m_startInfo;
	};

} // namespace sysUtil
using namespace sysUtil;

#endif	//__THREAD_H_

