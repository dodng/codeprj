////////////////////////////////////////////////////////////////////////////////////
// 
// @file: timeSystem.h
// @author: by Mr.Chen
// @date: 2014/8/3	20:44
// @brief: 时间帮助函数
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
	void	setBeginTime();					//计时
	int64 getThisTime();					//取得格式化时间
	uint	getCurrRunTime();				//取得服务器运行时间

	// 取得今日详细时间（年月日时分秒星期）
	int	 getThisYear()			{ _setThisTime(); return (m_thisTMTime.tm_year + 1900); };	//年
	int getThisMonth()		    { _setThisTime(); return (m_thisTMTime.tm_mon + 1); };		//月
	int getThisDay()			{ _setThisTime(); return (m_thisTMTime.tm_mday); };			//日
	int getThisHour()			{ _setThisTime(); return (m_thisTMTime.tm_hour); };				//时
	int getThisMinute()		{ _setThisTime(); return (m_thisTMTime.tm_min); };				//分
	int getThisSecond()		{ _setThisTime(); return (m_thisTMTime.tm_sec); };				//秒
	int getThisWeek()		    { _setThisTime(); return (m_thisTMTime.tm_wday); };			//星期，0表示星期日

	int getFrameThisYear() const		{ return (m_thisTMTime.tm_year + 1900); };	//年
	int getFrameThisMonth()	const		{ return (m_thisTMTime.tm_mon + 1); };		//月
	int getFrameThisDay() const			{ return (m_thisTMTime.tm_mday); };			//日
	int getFrameThisHour() const		{ return (m_thisTMTime.tm_hour); };			//时
	int getFrameThisMinute() const		{ return (m_thisTMTime.tm_min); };				//分
	int getFrameThisSecond() const	{ return (m_thisTMTime.tm_sec); };				//秒
	int getFrameThisWeek() const		{ return (m_thisTMTime.tm_wday); };			//星期，0表示星期日

public:
	uint getToday();								//获取当前日期（精确到天，例如：20120621 表示2012年6月21号）
    uint getCurrHourMinute();				    //获取当前日期（精确到分钟，例如：1206211930 表示12年6月21号19点半）
    int getYearDay();								 //获得当前日期（年+当前天数，例如：12001表示2012年1月1日）
    int getYearDay(int year,int month,int day);//把日期格式（年月日）转换成日期格式（年+当前天数），例如：20120101转成12001

	uint getFrameToday();						 //获取当前日期（精确到天，例如：20120621 表示2012年6月21号）
	uint getFrameCurrHourMinute();		//获取当前日期（精确到分钟，例如：1206211930 表示12年6月21号19点半）
	int getFrameYearDay();						//获得当前日期（年+当前天数，例如：12001表示2012年1月1日）
	
	bool isLeap(int year) const;							//是否闰年
	uint getYesterday();										//获得昨天日期（例如：20120620）
	int getBeforeNDays(int date,int n) const;	 //获取前n天（例如：20120621-2, 返回 20120619）
	int getAfterNDays(int date,int n) const;	 //获取后n天（例如：20120619+2, 返回 20120621）

	int getDiffDays_V1(int date1, int date2) const;  //计算2个日期差（例如:20120606 - 20120605, 返回1）
	int getDiffDays_V2(int x1, int x2) const; //计算（年+当前天数）格式表示的两个日期之间相隔天数

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
	void _setThisTime();				//保存格式化时间

private:
	uint m_nStartTime;				//启动时间
	uint m_nCurrentTime;			//当前时间
	time_t m_thisTime;				//格式化时间
	tm m_thisTMTime;				//格式化时间（年月天小时分秒）

#ifdef _LINUX64
	//struct timeval _tstart, _tend;
	//struct timezone tz;
	struct timespec _tstart,_tend;
#endif
};

} // namespace timeUtil
using namespace timeUtil;

#endif // __TIME_SYSTEM_H_
