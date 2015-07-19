////////////////////////////////////////////////////////////////////////////////////
// 
// File: cbfunc.h
// Author: by Mr.Chen
// CreateTime: 2014/5/1	0:27
// Desc: �ص�������װ(֧�����Ա������ȫ�ֺ���), ref from CEGUI.
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

	//�ص�����
	class SlotFunctorBase
	{
	public:
		virtual ~SlotFunctorBase() {}
		//ʹ�ò����������ú���
		virtual bool operator()(const EventArgs& args) = 0;
	};

	//ȫ�ֺ������ྲ̬��Ա�������ص�
	class FreeFunctionSlot : public SlotFunctorBase
	{
	public:
		//����ص������ĸ�ʽ
		typedef bool (SlotFunction)(const EventArgs&);  //����ָ��

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

	//����ͨ��Ա�������ص�
	template<typename T>
	class MemberFunctionSlot : public SlotFunctorBase
	{
	public:
		//��Ա�����Ļص���������
		typedef bool (T::*MemberFunctionType)(const EventArgs&);  //���Ա����ָ�������

	public:
		MemberFunctionSlot(MemberFunctionType func, T* obj) :
		d_function(func), d_object(obj)
		{}
		virtual bool operator()(const EventArgs& args)
		{
			return (d_object->*d_function)(args);  //������ĳ�Ա����
		}
	private:
		MemberFunctionType d_function;
		T* d_object;
	};

	//����������ʵ�ֵķ�װ
	class SubscriberSlot
	{
	public:
		//Ĭ�Ϲ��캯��
		SubscriberSlot() 
		{
			d_functor_impl = NULL;
		}
		//��׼��������
		~SubscriberSlot()
		{
			delete d_functor_impl;
			d_functor_impl = NULL;
		}

		//���ú�����()���أ������ɵ�һ��Functorʵ��
		bool operator()(const EventArgs& args) const
		{
			return (*d_functor_impl)(args);
		}
		//���س�Ա�Ƿ���Ч���Ƿ��Ѿ����ӵ�һ�������Functorʵ��
		bool connected() const
		{
			return d_functor_impl != NULL;
		}
		//FreeFunctionSlot�����ɺ����ķ�װ��
		SubscriberSlot(FreeFunctionSlot::SlotFunction* func) :
		d_functor_impl(new FreeFunctionSlot(func))
		{}

		// ģ�幹�캯�����Գ�Ա�����ķ�װΪ������MemberFunctionSlot��
		template<typename T>
		SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
		d_functor_impl(new MemberFunctionSlot<T>(function, obj))
		{}

	private:
		//�ڲ�����Functor��ָ�룬SlotFunctorBase����������������õ���
		SlotFunctorBase* d_functor_impl;
	};

	// �ص���װ�����
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

	// ע�ắ����
	//////////////////////////////////////////////////////////////
	// ע��ȫ�ֺ������ྲ̬��Ա������Ϊ�ص�����
	//////////////////////////////////////////////////////////////
	//#define  REGISTER_FUNC(id, subscribeSlot, func, q) \
	//	SubscriberSlot* subscribeSlot = new SubscriberSlot((FreeFunctionSlot::SlotFunction*)&func);\
	//	q->subscribe(id, subscribeSlot);

	#define  REGISTER_FUNC(id, func, q) \
		q->subscribe(id, new SubscriberSlot((FreeFunctionSlot::SlotFunction*)&func));

	//////////////////////////////////////////////////////////////
	// ע�����Ա������Ϊ�ص�����
	//////////////////////////////////////////////////////////////
	// @param id			: �¼�id
	// @param subscribeSlot	: ���һ��������
	// @param func			: Ҫע��Ļص��������������ͨ��Ա����
	// @param p				: func�������һ��ʵ��ָ��
	// @param q				: ע����(����ע�ắ������)��һ��ʵ��ָ��

	//#define  REGISTER_OBJFUNC(id, subscribeSlot, func, p, q) \
	//	SubscriberSlot* subscribeSlot = new SubscriberSlot(&func, p);\
	//	q->subscribe(id, subscribeSlot);

	#define  REGISTER_OBJFUNC(id, func, p, q) \
		q->subscribe(id, new SubscriberSlot(&func, p));
	
} // namespace sysUtil
using namespace sysUtil;

#endif	// __CB_FUNC_H_
