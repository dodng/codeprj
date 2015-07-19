////////////////////////////////////////////////////////////////////////////////////
// 
// @file: lock.h
// @author: by Mr.Chen
// @date: 2014/5/1	11:43
// @brief: 线程锁, ref from BAIDU.
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef  __LOCK_H_
#define  __LOCK_H_

namespace sysUtil {

	//自动锁模板类
	template <typename Lock>
	class AutoLock
	{
	public:
		inline AutoLock(Lock &l) : m_lock(l) {
			l.lock();
		}
		inline ~AutoLock() {
			m_lock.unlock();
		}
	private:
		Lock &m_lock;
	};

	#if defined(_WIN32)
	#include <Windows.h>

	class MLock
	{
	public:
		MLock() {
			InitializeCriticalSection(&m_lock);
		}
		~MLock() {
			DeleteCriticalSection(&m_lock);
		}
		void lock() {
			EnterCriticalSection(&m_lock);
		}
		void unlock() {
			LeaveCriticalSection(&m_lock);
		}
		int trylock() { 
			return TryEnterCriticalSection(&m_lock); 
		}
	private:
		CRITICAL_SECTION m_lock; //共享锁变量
	};

	typedef AutoLock<MLock> AutoMLock;

	#elif defined(_LINUX64)
	#include <sys/time.h>
	#include <pthread.h>

	//互斥对象锁
	class MLock
	{
	public:
		inline MLock() {
			pthread_mutex_init(&m_lock, NULL);
		}
		inline ~MLock() {
			pthread_mutex_destroy(&m_lock);
		}
		inline int lock() {
			return pthread_mutex_lock(&m_lock);
		}
		inline int unlock() {
			return pthread_mutex_unlock(&m_lock);
		}
		inline int trylock() {
			return pthread_mutex_trylock(&m_lock);
		}

	private:
		pthread_mutex_t m_lock;
		friend class MCondition;
	};

	//读写锁
	class RWLock
	{
	public:
		inline RWLock() {
			pthread_rwlock_init(&m_lock,NULL);
		}
		inline ~RWLock() {
			pthread_rwlock_destroy(&m_lock);
		}
		inline int RdLock() {
			return pthread_rwlock_rdlock(&m_lock);
		}
		inline int tryRdLock() {
			return pthread_rwlock_tryrdlock(&m_lock);
		}
		inline int RdLock_ms(const struct timespec *abs_time) {
			return pthread_rwlock_timedrdlock(&m_lock, abs_time);
		}
		inline int WrLock() {
			return pthread_rwlock_wrlock(&m_lock);
		}
		inline int tryWrLock() {
			return pthread_rwlock_trywrlock(&m_lock);
		}
		inline int Unlock() {
			return pthread_rwlock_unlock(&m_lock);
		}
	private:
		pthread_rwlock_t m_lock;
	};

	//自旋锁
	class SpinLock
	{
	public:
		inline SpinLock() {
			pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
		}
		inline ~SpinLock() {
			pthread_spin_destroy(&m_lock);
		}
		inline int lock() {
			return pthread_spin_lock(&m_lock);
		}
		inline int unlock() {
			return pthread_spin_unlock(&m_lock);
		}
		inline int trylock() {
			return pthread_spin_trylock(&m_lock);
		}

	private:
		pthread_spinlock_t m_lock;
	};

	//条件锁
	class MCondition
	{
	public:
		MCondition(MLock &l) : m_lock(l.m_lock), m_waits(0) {
			pthread_cond_init(&m_cond, NULL);
		}
		MCondition(pthread_mutex_t &l) : m_lock(l), m_waits(0) {
			pthread_cond_init(&m_cond, NULL);
		}
		~MCondition() {
			pthread_cond_destroy(&m_cond);
		}
		inline int wait(const timeval *tv) {
			++ m_waits;
			int ret = 0;
			if (tv) {
				timespec ts;
				ts.tv_sec = tv->tv_sec;
				ts.tv_nsec = tv->tv_usec * 1000;
				ret = pthread_cond_timedwait(&m_cond, &m_lock, &ts);
				--m_waits;
				return ret;
			}
			ret = pthread_cond_wait(&m_cond, &m_lock);
			-- m_waits;
			return ret;
		}
		inline void signal() {
			pthread_cond_signal(&m_cond);
		}
		inline void signalAll() {
			pthread_cond_broadcast(&m_cond);
		}
		inline size_t waits() { return m_waits; }
	private:
		pthread_cond_t m_cond;
		pthread_mutex_t &m_lock;
		size_t m_waits;
	};

	typedef AutoLock<MLock> AutoMLock;
	typedef AutoLock<SpinLock> AutoSLock;

	//自动读锁
	class AutoRdLock
	{
	private:
		RWLock &_lock;
	public:
		inline AutoRdLock(RWLock &l): _lock(l) {   
			_lock.RdLock(); 
		}
		inline ~AutoRdLock() { 
			_lock.Unlock(); 
		}
	};

	//自动写锁
	class AutoWrLock
	{
	private:
		RWLock &_lock;
	public:
		inline AutoWrLock(RWLock &l): _lock(l) {   
			_lock.WrLock(); 
		}
		inline ~AutoWrLock() { 
			_lock.Unlock(); 
		}
	};

	#endif	//_LINUX64

} // namespace sysUtil
using namespace sysUtil;

#endif  //__LOCK_H_
