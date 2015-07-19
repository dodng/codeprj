////////////////////////////////////////////////////////////////////////////////////
// 
// @file: anyObj.h
// @author: by Mr.Chen
// @date: 2014/10/10/ 20:08
// @brief: 任意对象类型
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __ANY_OBJ_H_
#define __ANY_OBJ_H_

#include <typeinfo>
#include "assert.h"
#include <iostream>
#include <ostream>
#include <string.h>

namespace sysUtil {

	class Any 
	{
	public: // constructors

		Any() : mContent(0)
		{
		}

		template<typename ValueType>
		explicit Any(const ValueType & value)
			: mContent(new holder<ValueType>(value))
		{
		}

		Any(const Any & other)
			: mContent(other.mContent ? other.mContent->clone() : 0)
		{
		}

		virtual ~Any()
		{
			destroy();
		}

	public: // modifiers

		Any& swap(Any & rhs)
		{
			std::swap(mContent, rhs.mContent);
			return *this;
		}

		template<typename ValueType>
		Any& operator=(const ValueType & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

		Any & operator=(const Any & rhs)
		{
			Any(rhs).swap(*this);
			return *this;
		}

	public: // queries

		bool isEmpty() const
		{
			return !mContent;
		}

		const std::type_info& getType() const
		{
			return mContent ? mContent->getType() : typeid(void);
		}

		inline friend std::ostream& operator <<
			( std::ostream& o, const Any& v )
		{
			if (v.mContent)
				v.mContent->writeToStream(o);
			return o;
		}

		void destroy()
		{
			delete mContent;
			mContent = NULL;
		}

	protected: // types

		class placeholder 
		{
		public: // structors

			virtual ~placeholder()
			{
			}

		public: // queries

			virtual const std::type_info& getType() const = 0;

			virtual placeholder * clone() const = 0;

			virtual void writeToStream(std::ostream& o) = 0;

		};

		template<typename ValueType>
		class holder : public placeholder
		{
		public: // structors

			holder(const ValueType & value)
				: held(value)
			{
			}

		public: // queries

			virtual const std::type_info & getType() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return new holder(held);
			}

			virtual void writeToStream(std::ostream& o)
			{
				//o << held;
			}

		public: // representation
			ValueType held;

		};


	protected: // representation
		placeholder * mContent;

		template<typename ValueType>
		friend ValueType * any_cast(Any *);


	public:

		template<typename ValueType>
		ValueType operator()() const
		{
			if (!mContent) 
			{
				Assert("Bad cast from uninitialised Any");
			}
			else if (getType() == typeid(ValueType))
			{
				return static_cast<Any::holder<ValueType> *>(mContent)->held;
			}
			else
			{
				Assert("Bad cast from uninitialised Any");
			}
		}

		template <typename ValueType>
		ValueType get(void) const
		{
			if (!mContent) 
			{
				Assert("Bad cast from uninitialised Any");
			}
			else if (getType() == typeid(ValueType))
			{
				return static_cast<Any::holder<ValueType> *>(mContent)->held;
			}
			else
			{
				Assert("Bad cast from uninitialised Any");
			}
		}

	};


	template<typename ValueType>
	ValueType * any_cast(Any * operand)
	{
		return operand && (strcmp(operand->getType().name(), typeid(ValueType).name()) == 0)
			? &static_cast<Any::holder<ValueType> *>(operand->mContent)->held
			: 0;
	}

	template<typename ValueType>
	const ValueType * any_cast(const Any * operand)
	{
		return any_cast<ValueType>(const_cast<Any *>(operand));
	}

	template<typename ValueType>
	ValueType any_cast(const Any & operand)
	{
		const ValueType * result = any_cast<ValueType>(&operand);
		if (!result)
		{
			Assert("Bad cast from type");
		}
		return *result;
	}

}// namespace sysUtil

#endif // __any_obj_h_
