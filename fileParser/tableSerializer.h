////////////////////////////////////////////////////////////////////////////////////
// 
// @file: tableSerializer.h
// @author: by Mr.Chen
// @date: 2014/5/15	18:45
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __TABLE_SERIALIZER_H_
#define __TABLE_SERIALIZER_H_

#include "ISerializer.h"
#include "assert.h"
#include "macroDef.h"
#include <stdio.h>

namespace tabReader {

const int MAX_COLUMN_COUNT = 512;
class TableSerializer : public ISerializer
{
public:
	TableSerializer();
	virtual ~TableSerializer();

	//read
	void	openRead(const char* fileName);
	void	skipHeader();	//该接口用于跳过第一行(列名称)和第二行(列类型)
	bool	nextLine();
	int 	lineCount() const;
	bool	checkColumnCount() const;
	
	static void	reload()
	{
		m_StringPool.reload();
		m_IntArrayPool.reload();
		m_FloatArrayPool.reload();
	}

	void	close()
	{
		if(m_pFile)
		{
			fclose(m_pFile);
		}
	}
	
	TableSerializer& operator<<(int&			value)		{ set(value); return *this; }
	TableSerializer& operator<<(uint&		value)		{ set(value); return *this; }
	TableSerializer& operator<<(int64&		value)		{ set(value); return *this; }
	TableSerializer& operator<<(uint64&		value)		{ set(value); return *this; }
	TableSerializer& operator<<(float&		value)		{ set(value); return *this; }
	TableSerializer& operator<<(char*&		value)		{ set(value); return *this; }
	TableSerializer& operator<<(IntArray&		value)		{ set(value); return *this; }
	TableSerializer& operator<<(FloatArray&	value)		{ set(value); return *this; }

	void skipField();
	void setCheckColumn(bool isCheck) { m_IsCheckColumn = isCheck; }

private:
	void	set(int&		value);
	void	set(uint&		value);
	void	set(int64&	value);
	void	set(uint64&	value);
	void	set(float& 	value);
	void	set(char*& 	value);
	void	set(IntArray&	value);
	void	set(FloatArray& value);

	bool	isSeparator(char c);
	void	clear();
	void	prepareRead();

	char*beginParseLine();
	void	endParseLine();

private:
	FILE*	m_pFile;
	char*	m_pCurrent;
	
	//列定义信息（从.tab文件中读取的列定义信息，用于与TableRow校对）
	enum EM_TYPE_COLUMN
	{
		EM_TYPE_COLUMN_INVALID=-1,
		EM_TYPE_COLUMN_INT,
		EM_TYPE_COLUMN_UINT,
		EM_TYPE_COLUMN_INT64,
		EM_TYPE_COLUMN_UINT64,
		EM_TYPE_COLUMN_FLOAT,
		EM_TYPE_COLUMN_STRING,
		EM_TYPE_COLUMN_INT_ARRAY,
		EM_TYPE_COLUMN_FLOAT_ARRAY,
		EM_TYPE_COLUMN_NUMBER,
	};

	EM_TYPE_COLUMN	_parseColumnType(const char* columnType);
	void			_checkColumnType(EM_TYPE_COLUMN columnType);

	EM_TYPE_COLUMN	m_Columns[MAX_COLUMN_COUNT];
	int			m_ColumnCount;
	int			m_ColumnOfCurrentLine;	//当前行第几列
	static char	s_ansiFileName[260];

	//读取缓冲区
	bool	m_HasReadBuffer;
	uchar*m_pReadBuffer;
	char*	m_pFileEnd;
	char*	m_pLineEnd;
	char	m_TempReplaceChar;
	bool	m_IsCheckColumn;

	class StringPool
	{
	public:
		static const int MAX_STRING_POOL_COUNT = 128*1024;
		char**m_pString;
		int	m_nCount;
		StringPool() 
		{ 
			m_pString = NULL;
			m_nCount = 0;
		}
		~StringPool() 
		{ 
			for (int i = 0; i < m_nCount; ++i)
			{
				SAFE_DELETE_ARRAY(m_pString[i]);
			}
			SAFE_DELETE_ARRAY(m_pString);
		}
		void reload()
		{
			for (int i = 0; i < m_nCount; ++i)
			{
				if(m_pString[i])
					SAFE_DELETE_ARRAY(m_pString[i]);
			}
			if(m_pString)
				SAFE_DELETE_ARRAY(m_pString);
			m_pString = new char*[MAX_STRING_POOL_COUNT]; 
			m_nCount = 0;
		}
		void add(char* pString) 
		{ 
			Assert(m_nCount < MAX_STRING_POOL_COUNT); 
			m_pString[m_nCount++] = pString; 
		}
	};

	class IntArrayPool
	{
	public:
		static const int MAX_INT_ARRAY_POOL_COUNT = 128*1024;
		int**	m_pIntArrays;
		int	m_nCount;
		IntArrayPool()
		{ 
			m_pIntArrays = NULL;
			m_nCount = 0;
			/*m_pIntArrays = new int*[MAX_INT_ARRAY_POOL_COUNT]; m_nCount = 0; */
		}
		~IntArrayPool() 
		{ 
			for (int i = 0; i < m_nCount; ++i)
			{
				SAFE_DELETE(m_pIntArrays[i]);
			}
			SAFE_DELETE(m_pIntArrays);
		}
		void reload()
		{
			for (int i = 0; i < m_nCount; ++i)
			{
				if(m_pIntArrays[i])
					SAFE_DELETE_ARRAY(m_pIntArrays[i]);
			}
			if(m_pIntArrays)
				SAFE_DELETE_ARRAY(m_pIntArrays);
			m_pIntArrays = new int*[MAX_INT_ARRAY_POOL_COUNT]; 
			m_nCount = 0;
		}
		void add(int* pIntArray) 
		{ 
			Assert(m_nCount < MAX_INT_ARRAY_POOL_COUNT); 
			m_pIntArrays[m_nCount++] = pIntArray; 
		}
	};

	class FloatArrayPool
	{
	public:
		static const int MAX_FLOAT_ARRAY_POOL_COUNT = 128*1024;
		float** m_pFloatArrays;
		int	  m_nCount;
		FloatArrayPool()
		{ 
			m_pFloatArrays = NULL;
			m_nCount = 0;
			/*m_pFloatArrays = new float*[MAX_FLOAT_ARRAY_POOL_COUNT]; m_nCount = 0;*/ 
		}
		~FloatArrayPool() 
		{ 
			for (int i = 0; i < m_nCount; ++i)
				SAFE_DELETE_ARRAY(m_pFloatArrays[i]);
			SAFE_DELETE_ARRAY(m_pFloatArrays);
		}
		void reload()
		{
			for (int i = 0; i < m_nCount; ++i)
			{
				if(m_pFloatArrays[i])
					SAFE_DELETE_ARRAY(m_pFloatArrays[i]);
			}
			if(m_pFloatArrays)
				SAFE_DELETE_ARRAY(m_pFloatArrays);
			m_pFloatArrays = new float*[MAX_FLOAT_ARRAY_POOL_COUNT]; 
			m_nCount = 0;
		}
		void add(float* pFloatArray) 
		{ 
			Assert(m_nCount < MAX_FLOAT_ARRAY_POOL_COUNT); 
			m_pFloatArrays[m_nCount++] = pFloatArray; 
		}
	};

	static StringPool		m_StringPool;
	static IntArrayPool	    m_IntArrayPool;
	static FloatArrayPool	m_FloatArrayPool;
};

} // namespace tabReader

#endif //__TABLE_SERIALIZER_H_

