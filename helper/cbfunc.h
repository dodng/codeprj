////////////////////////////////////////////////////////////////////////////////////
// 
// File: cbfunc.h
// Author: by Mr.Chen
// CreateTime: 2014/5/1	0:27
// Desc: 回调函数封装(支持类成员函数与全局函数), ref from CEGUI.
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __CB_FUNC_H_
#define __CB_FUNC_H_

#include "type.h"
#include <map>
using std::map;
using std::make_pair;

namespace sysUtil {

	typedef int EventArgs;
	typedef int EventID;

	//回调基类
	class SlotFunctorBase
	{
	public:
		virtual ~SlotFunctorBase() {}
		//使用操作符来调用函数
		virtual bool operator()(const EventArgs& args) = 0;
	};

	//全局函数或类静态成员函数做回调
	class FreeFunctionSlot : public SlotFunctorBase
	{
	public:
		//定义回调函数的格式
		typedef bool (SlotFunction)(const EventArgs&);  //函数指针

	public:
		FreeFunctionSlot(SlotFunction* func) : d_function(func)
		{}
		virtual bool operator()(const EventArgs& args)
		{
			return d_function(args);
		}
	private:
		SlotFunction* d_function;
	};

	//类普通成员函数做回调
	template<typename T>
	class MemberFunctionSlot : public SlotFunctorBase
	{
	public:
		//成员变量的回调函数定义
		typedef bool (T::*MemberFunctionType)(const EventArgs&);  //类成员函数指针的类型

	public:
		MemberFunctionSlot(MemberFunctionType func, T* obj) :
		d_function(func), d_object(obj)
		{}
		virtual bool operator()(const EventArgs& args)
		{
			return (d_object->*d_function)(args);  //调用类的成员函数
		}
	private:
		MemberFunctionType d_function;
		T* d_object;
	};

	//对上面两种实现的封装
	class SubscriberSlot
	{
	public:
		//默认构造函数
		SubscriberSlot() 
		{
			d_functor_impl = NULL;
		}
		//标准析构函数
		~SubscriberSlot()
		{
			delete d_functor_impl;
			d_functor_impl = NULL;
		}

		//调用函数的()重载，具体由第一类Functor实现
		bool operator()(const EventArgs& args) const
		{
			return (*d_functor_impl)(args);
		}
		//返回成员是否有效，是否已经连接到一个具体的Functor实现
		bool connected() const
		{
			return d_functor_impl != NULL;
		}
		//FreeFunctionSlot，自由函数的封装类
		SubscriberSlot(FreeFunctionSlot::SlotFunction* func) :
		d_functor_impl(new FreeFunctionSlot(func))
		{}

		// 模板构造函数，以成员函数的封装为参数，MemberFunctionSlot。
		template<typename T>
		SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
		d_functor_impl(new MemberFunctionSlot<T>(function, obj))
		{}

	private:
		//内部基本Functor的指针，SlotFunctorBase基类的优势在这里用到了
		SlotFunctorBase* d_functor_impl;
	};

	// 回调封装类基类
	class CbfuncHelper
	{
	public:
		typedef map<EventID, SubscriberSlot*> EventMap;
		
	public:
		CbfuncHelper() {eventList.clear();}
		~CbfuncHelper() {eventList.clear();}

		bool subscribe(EventID id, SubscriberSlot* subscriberSlot)
		{
			if (NULL == subscriberSlot)
				return false;
			EventMap::iterator iter = eventList.find(id);
			if (iter != eventList.end())
				return false;
			eventList.insert(make_pair(id, subscriberSlot));
			return true;
		}

		bool evoke(EventID id)
		{
			EventMap::iterator iter = eventList.find(id);
			if(iter == eventList.end())
				return false;
			SubscriberSlot* subscribeSlot = eventList[id];
			if (subscribeSlot)
			{
				EventArgs args = 0;
				(*subscribeSlot)(args);
				return true;
			}
			return false;
		}

    public:
        void registerEvent(int eventId, void* func)
        {
            subscribe(eventId, new SubscriberSlot((FreeFunctionSlot::SlotFunction*)func));
        }

        template<typename T>
        void registerEvent(int eventId, bool (T::*func)(const EventArgs&), T* obj)
        {
            subscribe(eventId, new SubscriberSlot(func, obj));
        }

        bool fireEvent(int eventId)
        {
            return evoke(eventId);
        }

	private:
		EventMap eventList;
	};

	// 注册函数宏
	//////////////////////////////////////////////////////////////
	// 注册全局函数、类静态成员函数作为回调函数
	//////////////////////////////////////////////////////////////
	//#define  REGISTER_FUNC(id, subscribeSlot, func, q) \
	//	SubscriberSlot* subscribeSlot = new SubscriberSlot((FreeFunctionSlot::SlotFunction*)&func);\
	//	q->subscribe(id, subscribeSlot);

	#define  REGISTER_FUNC(id, func, q) \
		q->subscribe(id, new SubscriberSlot((FreeFunctionSlot::SlotFunction*)&func));

	//////////////////////////////////////////////////////////////
	// 注册类成员函数作为回调函数
	//////////////////////////////////////////////////////////////
	// @param id			: 事件id
	// @param subscribeSlot	: 随便一个变量名
	// @param func			: 要注册的回调函数，即类的普通成员函数
	// @param p				: func所在类的一个实例指针
	// @param q				: 注册类(调用注册函数的类)的一个实例指针

	//#define  REGISTER_OBJFUNC(id, subscribeSlot, func, p, q) \
	//	SubscriberSlot* subscribeSlot = new SubscriberSlot(&func, p);\
	//	q->subscribe(id, subscribeSlot);

	#define  REGISTER_OBJFUNC(id, func, p, q) \
		q->subscribe(id, new SubscriberSlot(&func, p));
	
} // namespace sysUtil
using namespace sysUtil;

#endif	// __CB_FUNC_H_
