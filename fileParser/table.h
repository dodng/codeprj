////////////////////////////////////////////////////////////////////////////////////
// 
// @file: table.h
// @author: by Mr.Chen
// @date: 2014/5/15	17:09
// @brief: tab�ļ���ȡ��
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TABLE_H_
#define __TABLE_H_

#include "type.h"
#include "assert.h"
#include "macroDef.h"
#include "tableSerializer.h"

namespace tabReader {

template<typename T>
class Table
{
public:
	Table()		{ m_Rows = NULL; m_Count = 0; m_LastIndex = 0; }
	~Table()	{ SAFE_DELETE_ARRAY(m_Rows); }

	//���ر��
	bool		load(const char* fileName);

	//���ݱ���ID������Ӧ�С�����ID��ָ����е�һ�������ֵ����һ����0��1��ʼ������ĳ��Ʒ����Ǵ�13010001��ʼ��
	bool		isContainsRow(const int ID);	
	const T&	operator[](const int ID) const { return getRow(ID); } //ע�⣡�����operator[]���ص��Ǹ���ID���ҵ���Row�������Ǹ���Index���ҵ���Row��
	
	//���ݱ���row�����±��������,����ĳЩ����±������ű��
	const T&	getRowByIndex(const int index) const { Assert(index >= 0 && index < m_Count); return m_Rows[index]; }

	int		rowCount() const { return m_Count; }

private:
	T*	m_Rows;
	int		m_Count;
	mutable int	m_LastIndex;
	int		binarySearch(const T* const p, const int startIndex, const int endIndex, const int key) const;
	const T&	getRow(const int ID) const;
};

template<typename T>
const T& Table<T>::getRow(const int ID) const
{  
	if (m_LastIndex >= 0 
		&& m_LastIndex < m_Count 
		&& m_Rows[m_LastIndex] == ID)
	{
		return m_Rows[m_LastIndex];
	}
	int foundIndex = binarySearch(m_Rows, 0, m_Count - 1, ID);
	if (foundIndex < 0)
	{
		static T s_emptyRow = T();
		return s_emptyRow;
		//throw 1;
	}
	m_LastIndex = foundIndex;
	return m_Rows[foundIndex];
}

template<typename T>
bool Table<T>::isContainsRow(const int ID)
{
	int foundIndex = binarySearch(m_Rows, 0, m_Count - 1, ID);
	if (foundIndex >= 0)
	{
		m_LastIndex = foundIndex;
	}
	return foundIndex >= 0;
}

template<typename T>
bool Table<T>::load(const char* fileName)
{
	TableSerializer s;
	s.setCheckColumn(true);
	s.openRead(fileName);
	s.skipHeader();
	const int TITLE_ROW_COUNT = 2;
	//TODO maxRowCount��û���ٳ���"#"ע�͵�����
	const int maxRowCount = s.lineCount() - TITLE_ROW_COUNT;
	if(m_Rows)
	{
		SAFE_DELETE_ARRAY(m_Rows);
	}
	m_Rows = new T[maxRowCount];
	m_Count = 0;
	while (s.nextLine())
	{
		m_Rows[m_Count].MapData(s);
		if (m_Count > 0 && m_Rows[m_Count] <= m_Rows[m_Count - 1])
		{
			//���ID�����в���˳�򣨿��ܴ����ظ���ID�����޷���֤���ֲ���
			//Tools::Log( "���ID�����в���˳�򣨿��ܴ����ظ���ID��, index = %d", m_Count);
		}
		if (m_Count == 0)
		{
			s.checkColumnCount();
		}
		++m_Count;
	}
	//Tools::Log("Load table %s OK!", fileName);
	s.close();	
	return true;
}

template<typename T>
int Table<T>::binarySearch(const T* const p, const int startIndex, const int endIndex, const int key) const
{
	int start	= startIndex;
	int end		= endIndex;
	int middle  = (end + start) / 2;
	while (start < end && !(p[middle] == key))
	{
		if (p[middle] < key)
		{
			start = middle + 1;
		}
		else// if (p[middle] < key)
		{
			end = middle - 1;
		}
		middle  = (end + start) / 2;
	}
	return p[middle] == key ? middle : -1;
}

} // namespace tabReader

#endif // __TABLE_H_

