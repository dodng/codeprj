////////////////////////////////////////////////////////////////////////////////////
// 
// @file: timeSystem.h
// @author: by Mr.Chen
// @date: 2014/8/3	20:44
// @brief: ʱ���������
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __TIME_SYSTEM_H_
#define __TIME_SYSTEM_H_

#include <time.h>
#include <string>
#include "type.h"
#include "singleton.h"

#ifdef _LINUX64
#include <sys/utsname.h>
#include <sys/time.h>
#endif

namespace timeUtil {

class TimeSystem : public Singleton<TimeSystem>
{
public:
	void	setBeginTime();					//��ʱ
	int64 getThisTime();					//ȡ�ø�ʽ��ʱ��
	uint	getCurrRunTime();				//ȡ�÷���������ʱ��

	// ȡ�ý�����ϸʱ�䣨������ʱ�������ڣ�
	int	 getThisYear()			{ _setThisTime(); return (m_thisTMTime.tm_year + 1900); };	//��
	int getThisMonth()		    { _setThisTime(); return (m_thisTMTime.tm_mon + 1); };		//��
	int getThisDay()			{ _setThisTime(); return (m_thisTMTime.tm_mday); };			//��
	int getThisHour()			{ _setThisTime(); return (m_thisTMTime.tm_hour); };				//ʱ
	int getThisMinute()		{ _setThisTime(); return (m_thisTMTime.tm_min); };				//��
	int getThisSecond()		{ _setThisTime(); return (m_thisTMTime.tm_sec); };				//��
	int getThisWeek()		    { _setThisTime(); return (m_thisTMTime.tm_wday); };			//���ڣ�0��ʾ������

	int getFrameThisYear() const		{ return (m_thisTMTime.tm_year + 1900); };	//��
	int getFrameThisMonth()	const		{ return (m_thisTMTime.tm_mon + 1); };		//��
	int getFrameThisDay() const			{ return (m_thisTMTime.tm_mday); };			//��
	int getFrameThisHour() const		{ return (m_thisTMTime.tm_hour); };			//ʱ
	int getFrameThisMinute() const		{ return (m_thisTMTime.tm_min); };				//��
	int getFrameThisSecond() const	{ return (m_thisTMTime.tm_sec); };				//��
	int getFrameThisWeek() const		{ return (m_thisTMTime.tm_wday); };			//���ڣ�0��ʾ������

public:
	uint getToday();								//��ȡ��ǰ���ڣ���ȷ���죬���磺20120621 ��ʾ2012��6��21�ţ�
    uint getCurrHourMinute();				    //��ȡ��ǰ���ڣ���ȷ�����ӣ����磺1206211930 ��ʾ12��6��21��19��룩
    int getYearDay();								 //��õ�ǰ���ڣ���+��ǰ���������磺12001��ʾ2012��1��1�գ�
    int getYearDay(int year,int month,int day);//�����ڸ�ʽ�������գ�ת�������ڸ�ʽ����+��ǰ�����������磺20120101ת��12001

	uint getFrameToday();						 //��ȡ��ǰ���ڣ���ȷ���죬���磺20120621 ��ʾ2012��6��21�ţ�
	uint getFrameCurrHourMinute();		//��ȡ��ǰ���ڣ���ȷ�����ӣ����磺1206211930 ��ʾ12��6��21��19��룩
	int getFrameYearDay();						//��õ�ǰ���ڣ���+��ǰ���������磺12001��ʾ2012��1��1�գ�
	
	bool isLeap(int year) const;							//�Ƿ�����
	uint getYesterday();										//����������ڣ����磺20120620��
	int getBeforeNDays(int date,int n) const;	 //��ȡǰn�죨���磺20120621-2, ���� 20120619��
	int getAfterNDays(int date,int n) const;	 //��ȡ��n�죨���磺20120619+2, ���� 20120621��

	int getDiffDays_V1(int date1, int date2) const;  //����2�����ڲ����:20120606 - 20120605, ����1��
	int getDiffDays_V2(int x1, int x2) const; //���㣨��+��ǰ��������ʽ��ʾ����������֮���������

public:
    static uint64 getSystemTime();
    static uint64 getTimeMicroSec(); // get a 64 bit time
    static uint64 getTimeMs(); // in ms
    static uint64 getTimeSecs(); // in secs
    static uint32 getTimeSecs32();
    static void getTimestamp(char* datebuf, int size);
    static void getTimestamp(char* datebuf, int size, uint64 ms);

    static void sleep(uint64 ms);
    static std::string getCurrDayTimeStr(const time_t *timer);
    static std::string getCurrDayStr(const time_t *timer);
	
protected:
	void _setThisTime();				//�����ʽ��ʱ��

private:
	uint m_nStartTime;				//����ʱ��
	uint m_nCurrentTime;			//��ǰʱ��
	time_t m_thisTime;				//��ʽ��ʱ��
	tm m_thisTMTime;				//��ʽ��ʱ�䣨������Сʱ���룩

#ifdef _LINUX64
	//struct timeval _tstart, _tend;
	//struct timezone tz;
	struct timespec _tstart,_tend;
#endif
};

} // namespace timeUtil
using namespace timeUtil;

#endif // __TIME_SYSTEM_H_
