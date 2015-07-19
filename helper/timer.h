////////////////////////////////////////////////////////////////////////////////////
// 
// @file: timer.h
// @author: by Mr.Chen
// @date: 2014/8/3	18:39
// @brief: 定时器
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H_
#define __TIMER_H_

#include "type.h"

namespace timeUtil {

//计时器
class Timer
{
public:
	Timer();

	bool	isSet() const { return m_bSetTimer; }				//是否有效
	uint	getStartTime() const { return m_nStartTime; }	//获得开始时间
	void	setTerm(uint nTerm){ m_nTerm = nTerm; }		//设置有效时间
	uint	getTerm() const { return m_nTerm; }				//获得有效时间
		
	bool	beginTimer(uint nTerm, uint nNowTime);	//开始计时
	void	endTimer();											//结束计时
	bool	isReachTerm (uint	uNowTime) ;				//是否时间到

	Timer& operator=(const Timer& rhs)
	{
		m_nStartTime	= rhs.getStartTime();
		m_nTerm			= rhs.getTerm();
		m_bSetTimer		= rhs.isSet();
		return *this;
	};

private:
	uint	m_nStartTime;			//开始时间
	uint	m_nTerm;					//有效时间
	bool	m_bSetTimer;				//是否有效
};

} // namespace timeUtil
using namespace timeUtil;

#endif // __TIMER_H_
