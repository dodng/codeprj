#include "thread.h"
#include "type.h"

namespace sysUtil {

#if defined(_WIN32)
uint __stdcall PlatformThreadFunction(void* p)
{
	if (NULL == p)
		return 0;

	Thread::ThreadStartInfo* pInfo = static_cast<Thread::ThreadStartInfo*>(p);
	pInfo->function(pInfo->param);

	return 0;
}

#elif defined(_LINUX64)
void* PlatformThreadFunction(void* p)
{
	if (NULL == p)
		return NULL;

	Thread::ThreadStartInfo* pInfo = static_cast<Thread::ThreadStartInfo*>(p);
	pInfo->function(pInfo->param);

	return NULL;
}
#endif

Thread::Thread() 
{
#if defined(_WIN32)
	m_handle = NULL;
#endif
	m_status = TS_SLEEP;
	m_Id	 = 0;	
}

Thread::~Thread() 
{
#if defined(_WIN32)
	if (m_handle)
	{
		CloseHandle(m_handle);
		m_handle = NULL;
	}

#endif
	m_status = TS_STOP;
	m_Id	 = 0;
}

int Thread::start(cb_t function, void* param, bool startAtOnce)
{
	m_startInfo.function	= function;
	m_startInfo.param		= param;

#if defined(_WIN32)
	m_handle = (HANDLE)_beginthreadex(NULL, 0, PlatformThreadFunction, &m_startInfo, CREATE_SUSPENDED, &m_Id);

#elif defined(_LINUX64)
	int result = pthread_create(&m_Id, 0, PlatformThreadFunction, &m_startInfo);
	if (result != 0)
	{
		return -1;
	}
#endif
	if (startAtOnce)
	{
		resume();
	}

	return 0;
}

void Thread::resume()
{
	if (TS_SLEEP != m_status)
	{
		return;
	}
#if defined(_WIN32)
	ResumeThread(m_handle);
#endif
	m_status = TS_RUN;
}

void Thread::wait()
{
	if (m_Id == 0)
	{
		return;
	}

#if defined(_WIN32)
	DWORD exitCode;
	while (true)
	{
		if (GetExitCodeThread(m_handle, &exitCode) != 0)
		{
			if (exitCode != STILL_ACTIVE)
			{
				break;
			}
			else
			{
				WaitForSingleObject(m_handle,INFINITE);
			}
		}
		else
		{
			// log something here
			break;
		}
	}

	CloseHandle(m_handle);
	m_handle = NULL;

#elif defined(_LINUX64)
	void* exitCode;
	if (m_Id != pthread_self())
	{
		int r = pthread_join(m_Id, &exitCode);
		if (r != 0)
		{
			std::cerr << "pthread_join() returned " << r << std::endl;
		}
	}
#endif

	m_Id = 0;
}

} // namespace sysUtil
