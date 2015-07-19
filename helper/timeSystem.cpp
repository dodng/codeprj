#include "timeSystem.h"
#include "assert.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

WX_IMPLEMENT_SINGLEON(timeUtil::TimeSystem)

namespace timeUtil {

#define GETFULLTIME(year, month, day, hour) (year*1000000 + month*10000 + day*100 + hour)
#define GETYEAR(X) (X/1000000)					//获得年
#define GETMONTH(X) ((X%1000000)/10000)	//获得月
#define GETDAY(X) ((X%10000)/100)				//获得天
#define GETHOUR(X) (X%100)						//获得小时

void TimeSystem::setBeginTime()
{
	__ENTER_FUNCTION

#if defined(_WIN32)
	m_nStartTime	= GetTickCount();
	m_nCurrentTime	= GetTickCount();

#elif defined(_LINUX64)
	//gettimeofday(&_tstart,&tz);
	 clock_gettime(CLOCK_REALTIME, &_tstart);
#endif
	_setThisTime();

	__LEAVE_FUNCTION
}

void TimeSystem::_setThisTime()
{
	__ENTER_FUNCTION

	time_t stime;
	time(&stime);
	if (stime != (time_t)(-1))
	{
		m_thisTime = (int64)stime;
	}
#if defined(_WIN32)
	tm* pTMTime = localtime( &m_thisTime ) ;
	m_thisTMTime = *pTMTime ;

#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&m_thisTime,&newtm);
	if( ptm ) m_thisTMTime = newtm;
#endif

	__LEAVE_FUNCTION
}

int64  TimeSystem::getThisTime()
{
	__ENTER_FUNCTION

	_setThisTime();
	return m_thisTime;

	__LEAVE_FUNCTION
	return uint(-1);
}

uint	TimeSystem::getCurrRunTime()
{
	__ENTER_FUNCTION
	
#if defined(_WIN32)
	m_nCurrentTime	= GetTickCount();
	return (m_nCurrentTime-m_nStartTime);

#elif defined(_LINUX64)
	//gettimeofday(&_tend,&tz);
	struct timespec tend;
	clock_gettime(CLOCK_REALTIME, &tend);
	uint curRunTime=(tend.tv_sec-_tstart.tv_sec)*1000+(tend.tv_nsec-_tstart.tv_nsec)/1000000;
	return curRunTime;
#endif
	__LEAVE_FUNCTION
	return 0;
}

uint TimeSystem::getFrameToday()
{
	__ENTER_FUNCTION

	int year = getFrameThisYear();
	int month = getFrameThisMonth();
	int day = getFrameThisDay();
	return GETFULLTIME(year, month, day, 0)/100;

	__LEAVE_FUNCTION
	return 0;
}

uint TimeSystem::getToday()
{
	__ENTER_FUNCTION
	
	_setThisTime();
	int year = getThisYear();
	int month = getThisMonth();
	int day = getThisDay();
	return GETFULLTIME(year, month, day, 0)/100;

	__LEAVE_FUNCTION
	return 0;
}

uint	TimeSystem::getFrameCurrHourMinute()
{
	__ENTER_FUNCTION

	int year = getFrameThisYear();
	year %= 100; //只要最后两位数字
	int month = getFrameThisMonth();
	int day = getFrameThisDay();
	int hour = getFrameThisHour();
	int minute = getFrameThisMinute();
	uint uRet = GETFULLTIME(year, month, day, hour);
	uRet = uRet*100 + minute;
	return uRet;

	__LEAVE_FUNCTION
	return 0;
}

uint	TimeSystem::getCurrHourMinute()
{
	__ENTER_FUNCTION
	
	_setThisTime();
	int year = getThisYear();
	year %= 100; //只要最后两位数字
	int month = getThisMonth();
	int day = getThisDay();
	int hour = getThisHour();
	int minute = getThisMinute();
	uint uRet = GETFULLTIME(year, month, day, hour);
	uRet = uRet*100 + minute;
	return uRet;

	__LEAVE_FUNCTION
	return 0;
}

bool TimeSystem::isLeap(int year) const
{
	if((year%4==0&&year%100!=0)||(year%400==0))
	{
	   return true;
	}
	return false;
}

int TimeSystem::getFrameYearDay()
{
	__ENTER_FUNCTION

	int year = getFrameThisYear();
	int month = getFrameThisMonth();
	int day = getFrameThisDay();

	return getYearDay(year,month,day);
	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getYearDay()
{
	__ENTER_FUNCTION

	_setThisTime();
	int year = getThisYear();
	int month = getThisMonth();
	int day = getThisDay();

	return getYearDay(year,month,day);
	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getYearDay(int year,int month,int day)
{
	__ENTER_FUNCTION

	int MonthDays[]={31,28,31,30,31,30,31,31,30,31,30,31};
	int LeapMonthDays[]={31,29,31,30,31,30,31,31,30,31,30,31};
	int sum = 0;

	if(isLeap(year))
	{
		for(int i=0;i<month-1;i++)
			sum+=LeapMonthDays[i];
	}
	else
	{
		for(int i=0;i<month-1;i++)
			sum+=MonthDays[i];
	}

	sum+=day;
	return (sum + (year - 2000)*1000);

	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getDiffDays_V2(int x1, int x2) const
{
	__ENTER_FUNCTION
	
	// 例如：x1 = 12001, x2 = 12101
	//		 x1 = 12360, x2 = 13001
	if (x2 < x1)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}
	
	int year1 = x1 / 1000;
	int year2 = x2 / 1000;
	if ( year1 == year2)
	{//如果同年
		return x2 - x1;
	}
	else
	{//如果不同年
		int nRet1 = 0;
		if (isLeap(year1+2000))
			nRet1 = 366 - x1 % 1000;
		else
			nRet1 = 365 - x1 % 1000;
		int nRet2 = x2 % 1000;
		return nRet1 + nRet2;
	}

	__LEAVE_FUNCTION
	return 0;
}

uint TimeSystem::getYesterday()
{
	__ENTER_FUNCTION

	_setThisTime();
	time_t yesterdayTime = m_thisTime - 24*3600;
	tm yesterTMTime;

#if defined(_WIN32)
	tm* pTMTime = localtime( &yesterdayTime ) ;
	yesterTMTime = *pTMTime ;

#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&yesterdayTime,&newtm);
	if( ptm ) yesterTMTime = newtm;
#endif

	int year = yesterTMTime.tm_year + 1900;
	int month = yesterTMTime.tm_mon + 1;
	int day = yesterTMTime.tm_mday;
	return GETFULLTIME(year, month, day, 0)/100;

	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getBeforeNDays(int date,int n) const
{
	__ENTER_FUNCTION

	if (n<1)
		return date;
	date*=100;
	struct tm time1, time2;
	time_t timeT1,timeT2;

	time1.tm_year=GETYEAR(date)-1900;
	time1.tm_mon=GETMONTH(date)-1;
	time1.tm_mday=GETDAY(date);
	time1.tm_hour=0;
	time1.tm_min=0;
	time1.tm_sec=0;
	time1.tm_isdst=-1;
	timeT1=mktime(&time1);
	
	timeT2 = timeT1-n*24*3600;
#if defined(_WIN32)
	tm* pTMTime = localtime( &timeT2 ) ;
	time2 = *pTMTime ;
#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&timeT2,&newtm);
	if( ptm ) time2 = newtm;
#endif
	int year = time2.tm_year + 1900;
	int month = time2.tm_mon + 1;
	int day = time2.tm_mday;
	return GETFULLTIME(year, month, day, 0)/100;

	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getAfterNDays(int date,int n) const
{
	__ENTER_FUNCTION

	if (n<1)
		return date;
	date*=100;
	struct tm time1, time2;
	time_t timeT1,timeT2;

	time1.tm_year=GETYEAR(date)-1900;
	time1.tm_mon=GETMONTH(date)-1;
	time1.tm_mday=GETDAY(date);
	time1.tm_hour=0;
	time1.tm_min=0;
	time1.tm_sec=0;
	time1.tm_isdst=-1;
	timeT1=mktime(&time1);

	timeT2 = timeT1+n*24*3600;
#if defined(_WIN32)
	tm* pTMTime = localtime( &timeT2 ) ;
	time2 = *pTMTime ;
#elif defined(_LINUX64)
	tm  newtm;
	tm* ptm = localtime_r(&timeT2,&newtm);
	if( ptm ) time2 = newtm;
#endif
	int year = time2.tm_year + 1900;
	int month = time2.tm_mon + 1;
	int day = time2.tm_mday;
	return GETFULLTIME(year, month, day, 0)/100;

	__LEAVE_FUNCTION
	return 0;
}

int TimeSystem::getDiffDays_V1(int date1,int date2 ) const
{
	__ENTER_FUNCTION

	date1*=100;
	date2*=100;
	struct tm time1, time2;
	time_t timeT1,timeT2;
	
	int days=0;

	time1.tm_year=GETYEAR(date1)-1900;
	time1.tm_mon=GETMONTH(date1)-1;
	time1.tm_mday=GETDAY(date1);
	time1.tm_hour=0;
	time1.tm_min=0;
	time1.tm_sec=0;
	time1.tm_isdst=-1;
	timeT1=mktime(&time1);

	time2.tm_year=GETYEAR(date2)-1900;
	time2.tm_mon=GETMONTH(date2)-1;
	time2.tm_mday=GETDAY(date2);
	time2.tm_hour=0;
	time2.tm_min=0;
	time2.tm_sec=0;
	time2.tm_isdst=-1;
	timeT2=mktime(&time2);

	days=static_cast<int>(difftime(timeT2,timeT1)/(24*60*60));
	return days;
	__LEAVE_FUNCTION
	return 0;
}

uint64
TimeSystem::getSystemTime()
{
    Assert( sizeof(uint64) == 64/8 );
    uint64 time=0;
#if defined(_WIN32)  
    FILETIME ft;
    GetSystemTimeAsFileTime( &ft);
    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    time = li.QuadPart/10;
#else
    struct timeval now;
    gettimeofday( &now , NULL );
    //assert( now );
    time = now.tv_sec;
    time = time*1000000;
    time += now.tv_usec;
#endif
    return time;
}

uint64 
TimeSystem::getTimeMicroSec()
{
    return getSystemTime();
}

uint64
TimeSystem::getTimeMs()
{
    return getSystemTime()/1000;
}

uint64
TimeSystem::getTimeSecs()
{
    return getSystemTime()/1000000;
}

uint32
TimeSystem::getTimeSecs32()
{
	time_t t;
	time(&t);
	return t;
}

void 
TimeSystem::getTimestamp(char* datebuf, int datebufSize)
{
#ifdef _WIN32 
   int result = 1; 
   SYSTEMTIME systemTime;
   struct { time_t tv_sec; int tv_usec; } tv = {0,0};
   time(&tv.tv_sec);
   GetLocalTime(&systemTime);
   tv.tv_usec = systemTime.wMilliseconds * 1000; 
#else 
   struct timeval tv; 
   int result = gettimeofday (&tv, NULL);
#endif   
   int outTextLen = 0;
   if (result == -1)
   {
      datebuf[0] = 0;
      return;
   }
   else
   {
      /* The tv_sec field represents the number of seconds passed since
         the Epoch, which is exactly the argument gettimeofday needs. */
      const time_t timeInSeconds = (time_t) tv.tv_sec;
      outTextLen = (int)strftime (datebuf,
                             datebufSize,
                             "%Y%m%d-%H%M%S", /* guaranteed to fit in 256 chars,
                                                 hence don't check return code */
                             localtime (&timeInSeconds));
   }
   
   char msbuf[5];
   /* Dividing (without remainder) by 1000 rounds the microseconds
      measure to the nearest millisecond. */
   int msLen = sprintf(msbuf, ".%3.3ld", long(tv.tv_usec / 1000));

   memcpy(datebuf + outTextLen, msbuf, msLen);
   
   datebuf[outTextLen + msLen] = '\0';
}

void TimeSystem::getTimestamp(char* datebuf, int datebufSize, uint64 ms)
{
   const time_t timeInSeconds = ms/1000;
   strftime (datebuf,
             datebufSize,
             "%Y-%m-%d %H:%M:%S", /* guaranteed to fit in 256 chars,
                                 hence don't check return code */
             localtime (&timeInSeconds));
}

void TimeSystem::sleep(uint64 ms)
{
#ifdef _WIN32
   Sleep((DWORD)ms);
#else
   usleep(1000 * ms);   
#endif
}


std::string TimeSystem::getCurrDayTimeStr(const time_t *timer)
{
	char buf[20];
	time_t long_time;
	if (timer == NULL)
		time( &long_time );                /* Get time as long integer. */
	else
		long_time = *timer;

	struct tm* curtime = localtime(&long_time);
	sprintf(buf,"%04d%02d%02d%02d%02d%02d",curtime->tm_year+1900,++curtime->tm_mon,curtime->tm_mday
		,curtime->tm_hour+1,curtime->tm_min,curtime->tm_sec);
    return std::string(buf);
}

std::string TimeSystem::getCurrDayStr(const time_t *timer)
{
	char buf[10];
	time_t long_time;
	if (timer == NULL)
		time( &long_time );                /* Get time as long integer. */
	else
		long_time = *timer;

	struct tm* curtime = localtime(&long_time);
	sprintf(buf,"%04d%02d%02d",curtime->tm_year+1900,++curtime->tm_mon,curtime->tm_mday);
    return std::string(buf);
}


} // namespace sysUtil
