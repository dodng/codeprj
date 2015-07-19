#include "assert.h"
#include "macroDef.h"
#include <stdio.h>
#include <stdarg.h>

namespace sysUtil {

	void __show__(char* str)
	{
		//±£´æÈÕÖ¾
#ifdef __SAVELOG__
		if (LogSystem::GetInstance())
		{
			LogSystem::GetInstance()->SaveExceptionLog(str);
		}
#endif
		printf("Assert:%s", str);
		throw(1);
	}

	void	__assert__(const char* file, uint line, const char* myFunction, const char* myException)
	{
		char strLog[MAX_SHOWMESSAGE_LENGTH] = {0};

#ifdef _WIN32
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]", file, line, myFunction, myException ) ;
#endif

#ifdef _LINUX64
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]\n", file, line, myFunction, myException ) ;
#endif
		__show__(strLog);
	}

	void	__assertex__(const char* file, uint line, const char* myFunction, const char* myException, const char* msg)
	{
		char strLog[MAX_SHOWMESSAGE_LENGTH] = {0};

#ifdef _WIN32
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]", file, line, myFunction, myException ,msg ) ;
#endif

#ifdef _LINUX64
		tsnprintf( strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]\n", file, line, myFunction, myException ,msg ) ;
#endif
		__show__(strLog);
	}

	void	__assertspecial__(const char* file, uint line, const char* myFunction, const char* myException, const char* msg, ...)
	{
		char strLog[MAX_SHOWMESSAGE_LENGTH] = {0};

		va_list argp;
		va_start(argp, msg);
		char buf[MAX_SHOWMESSAGE_LENGTH] = {0};

		int nCount = tvsnprintf(buf, MAX_SHOWMESSAGE_LENGTH, msg, argp);
		va_end(argp);

#ifdef _WIN32
		tsnprintf(strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]", file, line, myFunction, myException, buf);
#endif

#ifdef _LINUX64
		tsnprintf(strLog, sizeof(strLog), "[%s][%d][%s][%s]\n[%s]\n", file, line, myFunction, myException, buf);
#endif
		__show__(strLog);
	}

}
