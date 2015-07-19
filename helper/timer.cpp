#include "timer.h"
#include "assert.h"

namespace timeUtil {

Timer::Timer()
{
	m_bSetTimer		= false;
	m_nStartTime		= 0;
	m_nTerm			= 0;
}

bool	Timer::beginTimer(uint nTerm, uint nNowTime)
{
	__ENTER_FUNCTION

	m_bSetTimer = true;
	m_nStartTime = nNowTime;
	m_nTerm		= nTerm;

	return true;
	__LEAVE_FUNCTION
	return false;
}

void	Timer::endTimer()
{
	__ENTER_FUNCTION
	
	m_bSetTimer		= false;
	m_nStartTime		= 0;
	m_nTerm			= 0;
	
	__LEAVE_FUNCTION
}

bool Timer::isReachTerm(uint uNowTime)
{
	__ENTER_FUNCTION
	Assert(m_bSetTimer);

	uint nElapsedTime = 0;
	if (uNowTime >= m_nStartTime)
	{
		nElapsedTime = uNowTime - m_nStartTime;
	}
	else
	{
		nElapsedTime = (uint(-1) - m_nStartTime) + uNowTime;
	}

	if (nElapsedTime < m_nTerm)
	{
		return false;
	}

	m_nStartTime = uNowTime;

	return true;
	__LEAVE_FUNCTION
	return false;
}

} // namespace timeUtil
