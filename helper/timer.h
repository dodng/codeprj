////////////////////////////////////////////////////////////////////////////////////
// 
// @file: timer.h
// @author: by Mr.Chen
// @date: 2014/8/3	18:39
// @brief: ��ʱ��
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H_
#define __TIMER_H_

#include "type.h"

namespace timeUtil {

//��ʱ��
class Timer
{
public:
	Timer();

	bool	isSet() const { return m_bSetTimer; }				//�Ƿ���Ч
	uint	getStartTime() const { return m_nStartTime; }	//��ÿ�ʼʱ��
	void	setTerm(uint nTerm){ m_nTerm = nTerm; }		//������Чʱ��
	uint	getTerm() const { return m_nTerm; }				//�����Чʱ��
		
	bool	beginTimer(uint nTerm, uint nNowTime);	//��ʼ��ʱ
	void	endTimer();											//������ʱ
	bool	isReachTerm (uint	uNowTime) ;				//�Ƿ�ʱ�䵽

	Timer& operator=(const Timer& rhs)
	{
		m_nStartTime	= rhs.getStartTime();
		m_nTerm			= rhs.getTerm();
		m_bSetTimer		= rhs.isSet();
		return *this;
	};

private:
	uint	m_nStartTime;			//��ʼʱ��
	uint	m_nTerm;					//��Чʱ��
	bool	m_bSetTimer;				//�Ƿ���Ч
};

} // namespace timeUtil
using namespace timeUtil;

#endif // __TIMER_H_
