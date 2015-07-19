#include "socketdef.h"
#include "test_mysql.h"
#include "macroDef.h"
#include "assert.h"
#include "strUtil.h"
#include "log.h"
#include <iostream>

using std::cout;
using std::endl;

IDBInterface::IDBInterface()
{
    __ENTER_FUNCTION
    _reconnTimer.endTimer();
    _pDataBase = new CDataBase();
    Assert(_pDataBase);
    __LEAVE_FUNCTION
}

IDBInterface::~IDBInterface()
{
    __ENTER_FUNCTION
    SAFE_DELETE(_pDataBase);
    _reconnTimer.endTimer();
    __LEAVE_FUNCTION
}

bool IDBInterface::init(const char* ip, uint port, const char* user, const char* pwd, const char* dbname, const char* encoding)
{
    __ENTER_FUNCTION

    strUtil::Strcpy_s(_ip,ip,sizeof(_ip));
    _port = port;
    strUtil::Strcpy_s(_user,user,sizeof(_user));
    strUtil::Strcpy_s(_pwd,pwd,sizeof(_pwd));
    strUtil::Strcpy_s(_dbname,dbname,sizeof(_dbname));
    strUtil::Strcpy_s(_encoding,encoding,sizeof(_encoding));
    //连接db
    if (!connect())
    {
        Assert(0);
    }
    return true ;
    __LEAVE_FUNCTION
    return false ;
}

bool IDBInterface::heartbeat(uint uTime)
{
    __ENTER_FUNCTION
    __MYTRY
    {
        // 重连DB
        if (!_reconnTimer.isSet())
        {
            _reconnTimer.beginTimer(30*1000, uTime);
        }
        else
        {
            if (_reconnTimer.isReachTerm(uTime))
            {
                reconnect();
            }
        }
    }
    __MYCATCH
    {
        MyExceptionLog( ) ;
    }
    return true ;
    __LEAVE_FUNCTION
     return false ;
}

bool IDBInterface::connect()
{
    __ENTER_FUNCTION
    bool bRet = _pDataBase->init(_ip,_user,_pwd,_dbname,_port,_encoding);
    Assert(bRet);
    LogSystem::getSinglePtr()->saveLogImmediately(LOG_FILE_SYSTEM,"connect db [%s] ok.", _dbname);
    return true;
    __LEAVE_FUNCTION
    return false;
}

bool IDBInterface::reconnect()
{
    __ENTER_FUNCTION
        if (!_pDataBase->isConnected())
        {
            return false;
        }
        int nRet = _pDataBase->reconnect();
        if(EM_RECONNECT_OK == nRet)
        {
            LogSystem::getSinglePtr()->saveLogImmediately(LOG_FILE_SYSTEM, "reconnect db [%s] ok", _dbname);
            return true;
        }
        else if(EM_RECONNECT_FAILED == nRet)
        {
            LogSystem::getSinglePtr()->cacheLog(LOG_FILE_ERROR, "reconnect db [%s] fail.", _dbname) ;
            return false;
        }
        else
        {
            return true;
        }
        __LEAVE_FUNCTION
        return false;
}

bool IDBInterface::disconnect()
{
    return _pDataBase->release();
}

bool test_mysql()
{
    IDBInterface* p = new IDBInterface();
    bool ret = p->init("127.0.0.1",3306,"root","123456","ns_video","utf8");
    return ret;
}
