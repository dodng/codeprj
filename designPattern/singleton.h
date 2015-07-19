////////////////////////////////////////////////////////////////////////////////////
// 
// File: singleton.h
// Author: by Mr.Chen
// CreateTime: 2014/5/1	2:32
// Desc: 单例模式模板类(线程安全), ref from design pattern.
//          
////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include "lock.h"

namespace SinglePattern {

//单例模板类
template<class T> 
class Singleton
{
public:
	static T* getSinglePtr()
	{
		static MLock s_lock;
		if (NULL==m_pSingleton)
		{
			// 加锁
			AutoMLock lock(s_lock);
			if(NULL==m_pSingleton)	
			{
				m_pSingleton = new T();
			}
		}
		return m_pSingleton;
	}

protected:
	Singleton() {}

private:
	static T* m_pSingleton;
};

//成员初始化宏
#define WX_IMPLEMENT_SINGLEON(className)\
	template<> className* Singleton<className>::m_pSingleton = NULL;

} //namespace SinglePattern
using namespace SinglePattern;

#endif	//__SINGLETON_H_
