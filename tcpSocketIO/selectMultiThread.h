// SelectMultiThread.cpp : �������̨Ӧ�ó������ڵ㡣
//
// ѡ��ģ�� +  ���߳�

#ifndef _MULTI_SELECT_SERVER_
#define _MULTI_SELECT_SERVER_

#if defined (_WIN32)

#include "type.h"
#include "macroDef.h"
#include "socketdef.h"
#include "thread.h"

namespace MultiThreadSelectServer
{

// ���ٵ�����߳���
const int MAX_THREADS_NUM = 2;
// ÿ���̴߳���Ŀͻ�����
const int MAX_CLIENTS_NUM_PER_THREAD = 2;

// �����̺߳���
void* WorkerThread(void* lpParameter);

int startSrv();

} //namespace MultiThreadSelectServer
using namespace MultiThreadSelectServer;

#endif //(_WIN32)

#endif  //_MULTI_SELECT_SERVER_
