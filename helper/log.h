////////////////////////////////////////////////////////////////////////////////////
// 
// @file: log.h
// @author: by Mr.Chen
// @date: 2015/5/25	13:47
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _LOG_H
#define _LOG_H

#include "type.h"
#include "singleton.h"

//特殊异常捕获日志
#if defined _LINUX64
    #define MyExceptionLog()	(LogSystem::getSinglePtr()->saveLogImmediately(\
            LOG_FILE_EXCEPTION, "%s %d %s", __FILE__,__LINE__,__PRETTY_FUNCTION__ ))
#else
    #define MyExceptionLog()	(LogSystem::getSinglePtr()->saveLogImmediately(\
            LOG_FILE_EXCEPTION, "%s %d %s", __FILE__,__LINE__,__FUNCTION__ ))
#endif

#define DEFAULT_LOG_CACHE_SIZE (1024*1024*4) //4M

enum LOG_FILE
{
    LOG_FILE_DEBUG=0,
    LOG_FILE_WARNING,
    LOG_FILE_SYSTEM,
    LOG_FILE_EXCEPTION,
    LOG_FILE_ERROR,
	LOG_FILE_NUMBER ,
};

class LogSystem : public Singleton<LogSystem>
{
public:
    static const int	MAX_SINGLE_LOG_LEN	= 1024*4;	 //单条日志最大长度

public:
    //初始化日志系统
	bool	init(int cachesize);
	//保存日志，直接写磁盘
	bool	saveLogImmediately(LOG_FILE logid, char* msg, ...);
	//保存日志，写入缓存
	void	cacheLog(int logid, char* msg, ...);
	//将缓存写入文件
	void	flushLog(int logid) ;
	void	flushAll(LOG_FILE level) ;
	//取得日志名称
	bool	getLogName(int logid, char* buffer, int len) ;
	
private:
	char* _logCache[LOG_FILE_NUMBER] ;	//日志内存区
	int _logPos[LOG_FILE_NUMBER] ;		//日志当前有效数据位置
	int	 _cacheSize ;					//缓存尺寸（初始化后固定）
	char* _pLogSaveDisk ;		//用于存储到磁盘的数据
    MLock _logLock[LOG_FILE_NUMBER] ;	//日志读写锁
};


#endif
