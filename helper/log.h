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

//�����쳣������־
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
    static const int	MAX_SINGLE_LOG_LEN	= 1024*4;	 //������־��󳤶�

public:
    //��ʼ����־ϵͳ
	bool	init(int cachesize);
	//������־��ֱ��д����
	bool	saveLogImmediately(LOG_FILE logid, char* msg, ...);
	//������־��д�뻺��
	void	cacheLog(int logid, char* msg, ...);
	//������д���ļ�
	void	flushLog(int logid) ;
	void	flushAll(LOG_FILE level) ;
	//ȡ����־����
	bool	getLogName(int logid, char* buffer, int len) ;
	
private:
	char* _logCache[LOG_FILE_NUMBER] ;	//��־�ڴ���
	int _logPos[LOG_FILE_NUMBER] ;		//��־��ǰ��Ч����λ��
	int	 _cacheSize ;					//����ߴ磨��ʼ����̶���
	char* _pLogSaveDisk ;		//���ڴ洢�����̵�����
    MLock _logLock[LOG_FILE_NUMBER] ;	//��־��д��
};


#endif