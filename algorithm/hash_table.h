////////////////////////////////////////////////////////////////////////////////////
// 
// @file: hash_table.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:06
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

#include <iostream>
#include "assert.h"
#include "type.h"
#include "strUtil.h"

namespace algorithm
{
	enum 
	{
		INVALID_VALUE = -1,
		PRIME_NUM = 28,
	};

	//素数表
	static uint m_primeList[PRIME_NUM] = 
	{
		53u,         97u,         193u,       389u,       769u,
		1543u,       3079u,       6151u,      12289u,     24593u,
		49157u,      98317u,      196613u,    393241u,    786433u,
		1572869u,    3145739u,    6291469u,   12582917u,  25165843u,
		50331653u,   100663319u,  201326611u, 402653189u, 805306457u, 
		1610612741u, 3221225473u, 4294967291u
	};

	//用于获取合适的素数做哈希表数组的大小
	static uint getPrime(uint size)
	{
		for (int i = 0; i < PRIME_NUM; ++i)
		{
			if (m_primeList[i] >= size)
			{
				return m_primeList[i];
			}
		}
		return INVALID_VALUE;
	}

	//哈希函数类, 用于获取哈希码
	class Hash
	{
	public:
		template<typename Key> 
		uint operator()(Key key) const
		{
			return key;
		}
		uint operator()(char* key)   const
		{
			uint hashCode = 0u;
			while (*key)
			{
				hashCode = hashCode*31 + *(uchar*)(key);
				++key;
			}
			return hashCode;
		}
		uint operator()(const char* key) const 
		{
			uint hashCode = 0u;
			while (*key)
			{
				hashCode = hashCode*31 + *(uchar*)(key);
				++key;
			}
			return hashCode;
		}
		uint operator()(uchar* key)	const
		{
			uint hashCode = 0u;
			while (*key)
			{
				hashCode = hashCode*31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(const uchar* key)  const
		{
			uint hashCode = 0u;
			while (*key)
			{
				hashCode = hashCode*31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(wchar_t* key) const
		{
			uint hashCode = 0u;
			while (*key)
			{
				hashCode = hashCode*31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(const wchar_t* key) const
		{
			uint hashCode = 0u;
			while (*key)
			{
				hashCode = hashCode*31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(uint64 key)	const
		{
			return (uint)(key^(key>>32));
		}
	};

	class Compare
	{
	public:
		template<typename Key> 
		bool operator()(Key key1, Key key2) const
		{
			return key1 == key2;
		}
		bool operator()(char* key1, char* key2) const
		{
			return !strUtil::Strcmp(key1, key2);
		}
		bool operator()(const char* key1, const char* key2) const
		{
			return !strUtil::Strcmp(const_cast<char*>(key1), key2);
		}
		bool operator()(const char* &key1, const char* &key2) const
		{
			return !strUtil::Strcmp(const_cast<char*>(key1), key2);
		}
	};

	//该哈希表用于快速查找，通过拉链法解决冲突。
	//实现了插入、查找、更新、删除等
	template<typename Key, typename Value, typename HashFunc = Hash, typename CompareFunc = Compare>
	class HashTable
	{
	public:
		//哈希表中的节点类
		struct HashNode
		{
			HashNode* m_pNext;
			Key m_key;
			Value m_pValue;
			HashNode(const Key& key, const Value& value) : m_key(key), m_pValue(value), m_pNext(NULL)
			{}
		public:
			HashNode(const HashNode& rhs)
			{
				this->m_pNext = rhs.m_pNext;
				this->m_key = rhs.m_key;
				this->m_pValue = rhs.m_pValue;
			}
			HashNode& operator=(const HashNode& rhs)
			{
				if(*this == rhs)return *this;
				this->m_pNext = rhs.m_pNext;
				this->m_key = rhs.m_key;
				this->m_pValue = rhs.m_pValue;
				return *this;
			}
		};

	public:
		explicit HashTable(int capacity);
		~HashTable();

		//添加元素
		bool Insert(const Key& key, const Value& value);
		//查找元素
		const Value* Find(const Key& key) const;
		//更新元素
		bool Update(const Key& key, const Value& value);
		//删除元素
		bool Erase(const Key& key);

		//元素个数
		uint Size() const {return m_elementsNumber;}
		//哈希表数组大小
		uint Capacity() const {return m_capacity;} 
		//调整数组大小： 通过创建一个新哈希表，然后将旧表的内容插入到新表，最后将新表的所有数据成员赋值给旧表
		bool Resize(uint size);

		//将使用状况记入日志
		bool Status() const;
		//将表内详细使用情况写入日志
		void Dump() const;
		//清空哈希表
		bool Clear();

	private:
		//防止拷贝构造、赋值
		HashTable(const HashTable& rhs);
		HashTable& operator=(const HashTable& rhs);

		//获取键所对应数组中的索引
		uint GetHash(const Key& key, const uint size) const;
		//在已知的索引处插入节点
		bool InsertNode(const uint index, HashNode* hashNode);
		//根据key插入节点
		bool InsertNode(const Key& key, HashNode* hashNode);
		//插入键值，不考虑哈希数组大小
		bool InsertNoResize(const Key& key, const Value& value);

	private:
		//哈希函数
		HashFunc m_hashFunc;
		//比较函数
		CompareFunc m_compareFunc;
		// 哈希表数组
		HashNode** m_pBuckets;
		//哈希表数组大小
		uint m_capacity;
		//元素个数
		uint m_elementsNumber;
		//数组中使用的个数
		uint m_usedCount;
	};

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	HashTable<Key, Value, HashFunc, CompareFunc>::HashTable(int capacity) : m_capacity(capacity), m_pBuckets(NULL), m_elementsNumber(0), m_usedCount(0)
	{
		m_capacity = (uint)getPrime(capacity); 
		Assert(m_capacity);
		m_pBuckets = new HashNode*[m_capacity];
		for (uint i=0; i<m_capacity; ++i)
		{
			m_pBuckets[i] = 0;
		}
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	HashTable<Key, Value, HashFunc, CompareFunc>::~HashTable()
	{
		bool ret = Clear();
		if (!ret)
		{
			char log[256] = {0};
			sprintf(log, "%s %d %s", __FILE__,__LINE__,__FUNCTION__);
			Assert(log && false);
			return;
		}
		delete[] m_pBuckets;
		m_pBuckets = NULL;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	uint HashTable<Key, Value, HashFunc, CompareFunc>::GetHash(const Key& key, const uint capacity) const
	{
		uint hashCode = m_hashFunc(key);
		Assert(hashCode != (uint)(INVALID_VALUE));
		return hashCode%capacity;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::InsertNode(const uint index, HashNode* hashNode)
	{
		if (m_pBuckets[index] == NULL)
		{
			m_pBuckets[index] = hashNode;
			++m_elementsNumber;
			++m_usedCount;
		}
		else
		{
			HashNode* current = m_pBuckets[index];
			HashNode* next = m_pBuckets[index]->m_pNext;
			while (next)
			{
				current = next;
				next = next->m_pNext;
			}
			current->m_pNext = hashNode;
			++m_elementsNumber;
		}
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::InsertNode(const Key& key, HashNode* hashNode)
	{
		uint index = GetHash(key, m_capacity);
		return InsertNode(index, hashNode);
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::InsertNoResize(const Key& key, const Value& value)
	{
		uint index = GetHash(key, m_capacity);
		Assert(index != (uint)(INVALID_VALUE));
		HashNode* hashNode = new HashNode(key, value);
		return InsertNode(index, hashNode);
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::Resize(uint size)
	{
		if (size<=m_capacity)
		{
			return true;
		}
		uint oldsize = m_capacity;
		HashNode** pOld = m_pBuckets;
		m_capacity = getPrime(size);	
		m_pBuckets = new HashNode*[m_capacity]();;
		m_elementsNumber = 0;
		m_usedCount = 0;
		for (uint i=0; i<oldsize; ++i)
		{
			HashNode* first = pOld[i];
			while (first)
			{
				HashNode* insertNode = first;
				first = first->m_pNext;
				insertNode->m_pNext = NULL;
				bool ret = InsertNode(insertNode->m_key,insertNode);
				if (!ret)
				{
					char log[256] = {0};
					sprintf(log, "%s %d %s", __FILE__,__LINE__,__FUNCTION__);
					Assert(log && false);
					return ret;
				}
			}
		}
		delete[] pOld;
		pOld = NULL;
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::Insert(const Key& key, const Value& value)
	{
		Resize(m_elementsNumber+1);
		return InsertNoResize(key, value);
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	const Value* HashTable<Key, Value, HashFunc, CompareFunc>::Find(const Key& key) const
	{
		uint index = GetHash(key, m_capacity);
		if (index == (uint)(INVALID_VALUE))
		{
			return NULL;
		}
		HashNode* node = m_pBuckets[index];
		while (node)
		{
			if(m_compareFunc(node->m_key, key))
			{
				return &(node->m_pValue);
			}
			node = node->m_pNext;
		}
		return NULL;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::Update(const Key& key, const Value& value)
	{
		Value* oldValue = const_cast<Value*>(Find(key));
		Assert(oldValue != NULL);
		*oldValue = value;
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::Erase(const Key& key)
	{
		uint index = GetHash(key, m_capacity);
		HashNode* first = m_pBuckets[index];
		HashNode* previous = NULL;
		while (first)
		{
			if(m_compareFunc(first->m_key, key))
			{
				if (previous == NULL)
				{
					if (first->m_pNext == NULL) --m_usedCount;
					m_pBuckets[index] = first->m_pNext;
					delete first;
					first = NULL;
					--m_elementsNumber;
					return true;
				}
				else
				{
					previous->m_pNext = first->m_pNext;
					delete first;
					first = NULL;
					--m_elementsNumber;
					return true;
				}
			}
			previous = first;
			first =	first->m_pNext;
		}
		return false;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::Status() const
	{
		//std::cout << "HashTable使用的个数为：%d；冲突的个数为：%d；元素个数为：%d；哈希表数组大小：%d", 
		//m_usedCount, (m_elementsNumber-m_usedCount), m_elementsNumber, m_capacity);
		if(m_elementsNumber != 0)
		{
			float rate = ((m_elementsNumber-m_usedCount)*1000/m_elementsNumber)*(0.001f);
			//Log::GetInstance()->SaveSystemLog( "冲突率为：", rate );
		}
		else
		{
			//Log::GetInstance()->SaveSystemLog( "冲突率为：", 0 );
		}
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	void HashTable<Key, Value, HashFunc, CompareFunc>::Dump() const
	{
		for (int i=0; i<m_capacity; ++i)
		{
			std::cout << "index is " << i << ": " ;
			if (m_pBuckets[i])
			{
				HashNode* first = m_pBuckets[i];
				while (first)
				{
					//if (first->m_pNext)
					//	std::cout << 1 <<" -->";
					//else
					//	std::cout << " " << 1;
					std::cout << first->m_key <<", ";
					first = first->m_pNext;
				}
				std::cout << endl;
			}
			else
			{
				cout << 0 << endl;
			}
		}
	}

	template<typename Key, typename Value, typename HashFunc, typename CompareFunc>
	bool HashTable<Key, Value, HashFunc, CompareFunc>::Clear()
	{
		for (uint i=0; i<m_capacity; ++i)
		{
			if (m_pBuckets[i] != NULL)
			{
				HashNode* first = m_pBuckets[i];
				while (first)
				{
					HashNode* tmp = first;
					first = first->m_pNext;
					delete tmp;
					tmp = NULL;
					--m_elementsNumber;
				}
				m_pBuckets[i] = NULL;
			}
		}
		m_usedCount = 0;
		return (m_elementsNumber == 0);
	}
} // namespace algorithm
using namespace algorithm;

#endif	// __HASH_TABLE_H_
