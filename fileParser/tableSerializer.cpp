#include <sys/stat.h>
#include <stdio.h>
#include "tableSerializer.h"
#include "strUtil.h"
#include <string.h> //memset

namespace tabReader { 

TableSerializer::StringPool		TableSerializer::m_StringPool;
TableSerializer::IntArrayPool		TableSerializer::m_IntArrayPool;
TableSerializer::FloatArrayPool	TableSerializer::m_FloatArrayPool;

char TableSerializer::s_ansiFileName[260] = {0};

char* g_ColumnTypeToString[] = 
{
	"INVALID",
	"INT",
	"UINT",
	"INT64",
	"UINT64",
	"FLOAT",
	"STRING",
	"STRING_INT",
	"STRING_FLOAT",
	"INT",
};

char* ColumnIndexToChar(const int index);

TableSerializer::TableSerializer()
{
	clear();
}

TableSerializer::~TableSerializer() 
{
	if (NULL != m_pReadBuffer)
		SAFE_DELETE_ARRAY(m_pReadBuffer);
	clear();
}

void TableSerializer::clear()
{
	m_HasReadBuffer = false;
	m_pCurrent		= NULL;
	m_pReadBuffer	= NULL;
	m_pFileEnd 		= NULL;
	m_pLineEnd 		= NULL;
	m_pFile			= NULL;
	m_ColumnCount	= 0;
	for (int i = 0; i < MAX_COLUMN_COUNT; ++i)
	{
		m_Columns[i] = EM_TYPE_COLUMN_INVALID;
	}
	m_TempReplaceChar	= 0;
	m_ColumnOfCurrentLine = 0;
	m_IsCheckColumn = false;
}

void TableSerializer::openRead(const char* fileName)
{
	Strcpy_s(s_ansiFileName, fileName, sizeof(s_ansiFileName));
	m_pFile = ::fopen(s_ansiFileName, "rb");
	if (NULL == m_pFile)
	{
		return;
	}
	fseek(m_pFile, 0, SEEK_SET);
	uchar bom[2] = { 0 };
	fread(bom, sizeof(uchar), 2, m_pFile);
	if ((bom[0] == 0xFF && bom[1] == 0xFE) || (bom[0] == 0xFE && bom[1] == 0xFF))
	{
		//return true;
	}
	else
	{
		fseek(m_pFile, 0, SEEK_SET);
	}
}

void TableSerializer::set(int& value)
{	
	_checkColumnType(EM_TYPE_COLUMN_INT);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	value = strUtil::Strtol(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(uint& value)
{	
	_checkColumnType(EM_TYPE_COLUMN_UINT);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	value = strUtil::Strtoul(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(int64& value)
{
	_checkColumnType(EM_TYPE_COLUMN_INT64);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	value = strUtil::Strtoint64(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(uint64& value)
{
	_checkColumnType(EM_TYPE_COLUMN_UINT64);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	value = strUtil::Strtouint64(pBegin, NULL, 10);
	endParseLine();
}

void TableSerializer::set(float& value)
{
	_checkColumnType(EM_TYPE_COLUMN_FLOAT);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	value = static_cast<float>(strUtil::Strtod(pBegin, NULL));
	endParseLine();
}

void TableSerializer::set(char*& value)
{
	_checkColumnType(EM_TYPE_COLUMN_STRING);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;

	//生成临时的新字符串
	const int64 maxSize = m_pCurrent - pBegin + 1;
	value = new char[maxSize];
	strUtil::Strncpy(value, pBegin, maxSize - 1);
	value[maxSize - 1] = 0;
	m_StringPool.add(value);
	endParseLine();
}

void TableSerializer::set(IntArray& value)
{
	_checkColumnType(EM_TYPE_COLUMN_INT_ARRAY);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	if (true == value.isValid())
	{
		Assert(0);
		return;
	}
	//生成临时的新字符串
	const int64 maxSize = m_pCurrent - pBegin + 1;
	if (maxSize <= 0)
	{
		Assert(0);
		return;
	}
	char* wholeString = new char[maxSize];
	memset(wholeString, 0, sizeof(char)*maxSize);
	strUtil::Strncpy(wholeString, pBegin, maxSize-1);
	//先判断是否是-1，-1就不处理啦
	if (strUtil::Strcmp(wholeString, "-1") == 0)
	{
		SAFE_DELETE_ARRAY(wholeString);
		endParseLine();
		return;
	}
	//统计分割符数量
	int seperatorCount = 0;
	for (int i = 0; i < maxSize; ++i)
	{
		if (wholeString[i] == '|')
			++seperatorCount;
	}
	int *pArray = value.init(seperatorCount+1);
	m_IntArrayPool.add(pArray);
	char	seperator[]		= "|";
	char*	token			= NULL;
	char*	token_context	= NULL;

	token = strUtil::Strtok_s(wholeString, seperator, &token_context);
	int tokenCount = 0;
	while (token != NULL)
	{
		Assert(tokenCount < value.m_Count);
		pArray[tokenCount] = strUtil::Strtol(token, NULL, 10);
		token = strUtil::Strtok_s(NULL, seperator, &token_context);
		tokenCount++;
	}
	SAFE_DELETE_ARRAY(wholeString);
	endParseLine();
}

void TableSerializer::set(FloatArray& value)
{
	_checkColumnType(EM_TYPE_COLUMN_FLOAT_ARRAY);
	char* pBegin = beginParseLine();
	if (NULL == pBegin)
		return;
	if (value.m_Array != NULL)
	{
		Assert(0);
		return;
	}
	//生成临时的新字符串
	const int64 maxSize = m_pCurrent-pBegin+1;
	if (maxSize <= 0)
	{
		Assert(0);
		return;
	}
	char* wholeString = new char[maxSize];
	memset(wholeString, 0, sizeof(char)*maxSize);
	strUtil::Strncpy(wholeString, pBegin, maxSize - 1);
	//统计分割符数量
	int seperatorCount = 0;
	for (int i = 0; i < maxSize; ++i)
	{
		if (wholeString[i] == '|')
			++seperatorCount;
	}
	value.m_Array = new float[seperatorCount+1];
	m_FloatArrayPool.add(value.m_Array);
	value.m_Count = seperatorCount + 1;
	char	seperator[]		= "|";
	char*	token			= NULL;
	char*	token_context	= NULL;
	
	token = strUtil::Strtok_s(wholeString, seperator, &token_context);
	int tokenCount = 0;
	while (token != NULL)
	{
		Assert(tokenCount < value.m_Count);
		value.m_Array[tokenCount] = static_cast<float>(strUtil::Strtod(token, NULL));
		token = strUtil::Strtok_s(NULL, seperator, &token_context);
		tokenCount++;
	}
	SAFE_DELETE_ARRAY(wholeString);
	endParseLine();
}

bool TableSerializer::isSeparator(char c)
{
	return (c == '\t') || (c == '\r') || (c == '\n') || c == 0;
}

char* ColumnIndexToChar(const int index)
{
	static char c[3] = { 0 };
	memset(c, 0, sizeof(c));
	if (index >= 26)
	{
		c[0] = 'A' + index / 26 - 1;
		c[1] = 'A' + index % 26;
	}
	else
	{
		c[0] = 'A' + index;
	}
	return c;
}

int FileLineCount(FILE* file);

int TableSerializer::lineCount() const
{
	return FileLineCount(m_pFile);
}

void TableSerializer::skipHeader()
{
	prepareRead();

	//指向第一行，列的名称
	if (!nextLine())
	{
		Assert(false);
		return;
	}
	//指向第二行，列的类型
	if (!nextLine())
	{
		Assert(false);
		return;
	}
	//m_IsFirstColumn = true;

	//统计列的信息和数量
	//TODO 应该在读取第一行真实数据的时候统计columnCount
	const int MAX_TYPE_STRING_LIST_LENGTH = MAX_COLUMN_COUNT * sizeof("STRING_INT");
	char typeString[MAX_TYPE_STRING_LIST_LENGTH] = { 0 };
	strUtil::Strncpy(typeString, m_pCurrent, m_pLineEnd - m_pCurrent + 1);
	
	char seperator[]		= "\t";
	char* token			= NULL;
	char* token_context	= NULL;
	token = strUtil::Strtok_s(typeString, seperator, &token_context);
	m_ColumnCount = 0;
	while (token != NULL)
	{
		TrimRight(token);
		m_Columns[m_ColumnCount] = _parseColumnType(token);
		token = strUtil::Strtok_s(NULL, seperator, &token_context);
		m_ColumnCount++;
	}
}

char* FindLineEnd(const char* pString, const char* rightMost);

bool TableSerializer::nextLine()
{
	prepareRead();
	m_ColumnOfCurrentLine = 0;
	
	if (NULL == m_pLineEnd)
	{
		m_pLineEnd = FindLineEnd(m_pCurrent, m_pFileEnd);
	}
	else
	{
		if (m_pLineEnd < m_pFileEnd)
		{
			if (*(m_pLineEnd + 1) == '\r')
			{
				m_pCurrent = m_pLineEnd + 2 + 1; // +2 是跳过"\r\n", +1 是跳过最后一个字符，即跳到下一行开始处
			}
			else
			{
				m_pCurrent = m_pLineEnd + 1 + 1; // +2 是跳过"\r\n", +1 是跳过最后一个字符，即跳到下一行开始处
			}
			m_pLineEnd = FindLineEnd(m_pCurrent, m_pFileEnd);
		}
		else
		{
			return false;
		}
	}

	if (*m_pCurrent == '#' || *m_pCurrent == '\r' || *m_pCurrent == '\n')
	{
		nextLine();
	}

	//m_IsFirstColumn = false;

	if (m_pCurrent >= m_pLineEnd)
		return false;

	if  (*m_pCurrent == '\t' || *m_pCurrent == 0)
	{
		char msg[512] = { 0 };
		strUtil::Snprintf(msg, 512, "\nthere's empty row in table[%s]!", s_ansiFileName);
		//Tools::Log(msg);
		return false;
	}
	return true;
}

int FileSize(FILE* file)
{
	if (!file)
		return 0;

	int m_size = 0;
	int oldPosition = ftell(file);
	//将缓冲区内容写入文件，以便能够获取fileStat
	fflush(file);
	struct stat fileStat;
#ifdef _WIN32
	int result = fstat(_fileno(file), &fileStat);
#endif
#ifdef _LINUX64
	int result = fstat(fileno(file), &fileStat);
#endif
	if( result != 0 )
	{
		Assert(false);
		return 0;
	}
	m_size = fileStat.st_size;
	fseek(file, oldPosition, SEEK_SET);
	Assert(m_size >= 0);
	return m_size;
}

//使用全局对象，避免占用局部栈空间
const int MAX_LINE_COUNT_TEMP_STRING_LENGTH = 16*1024;
char lineCountTempString[MAX_LINE_COUNT_TEMP_STRING_LENGTH];

int FileLineCount(FILE* file)
{
	if (!file)
		return 0;
	int m_lineCount = 0;

	int oldPosition = ftell(file);
	//TODO 这样做会导致单行数据超过16k的时候，统计结果错误。
	//TODO 统计行数可以采用更好的算法
	fseek(file, 0, SEEK_SET);
	m_lineCount = 0;
	while (fgets(lineCountTempString, MAX_LINE_COUNT_TEMP_STRING_LENGTH, file))
	{
		m_lineCount++;
	}
	fseek(file, oldPosition, SEEK_SET);

	return m_lineCount;
}

void TableSerializer::prepareRead()
{
	if (m_HasReadBuffer)
		return;
	if (NULL == m_pFile)
	{
		Assert(false);
		return;
	}
	if (NULL != m_pReadBuffer)
	{
		SAFE_DELETE_ARRAY(m_pReadBuffer);
		m_pReadBuffer = NULL;
	}
	//根据文件大小分配缓冲区
	int fileSize = FileSize(m_pFile);
	m_pReadBuffer = new uchar[fileSize+4];
	memset(m_pReadBuffer, 0, fileSize + 4);

	//读取整个文件到缓冲区
	int q = ftell(m_pFile);
	size_t readSize = fread(m_pReadBuffer, 1, fileSize, m_pFile);
	m_pCurrent = reinterpret_cast<char*>(m_pReadBuffer);
	m_pFileEnd = reinterpret_cast<char*>(m_pReadBuffer + readSize - 1);
	m_HasReadBuffer = true;
}

////////////////////
//FindLineEnd
//返回找的第一个换行符的指针pNewLine的前一个字符
//整个行的字符串内容为m_string到pNewLine - 1
////////////////////
char* FindLineEnd(const char* pString, const char* rightMost)
{
	const char* pNewLine	= pString;
	const char* pEnd		= rightMost;
	while (pNewLine <= pEnd)
	{
		if (	(*pNewLine == '\r' && *(pNewLine + 1) == '\n' && pNewLine < pEnd)
			||	(*pNewLine == '\n' && pNewLine < pEnd) )
		{
			return const_cast<char*>(pNewLine - 1);
		}
		++pNewLine;
	}
	return const_cast<char*>(pEnd);
}

char*	TableSerializer::beginParseLine()
{
	//增加当前计数
	m_ColumnOfCurrentLine++;

	if (m_pCurrent > m_pLineEnd)
	{
		return NULL;
	}
	char* pBegin = m_pCurrent;
	while (!isSeparator(*m_pCurrent))
	{
		m_pCurrent++;
	}
	m_TempReplaceChar = *m_pCurrent;
	*m_pCurrent = 0;
	return pBegin;
}

void TableSerializer::endParseLine()
{
	*m_pCurrent = m_TempReplaceChar;
	//跳过tab分隔符
	m_pCurrent++;
}

void TableSerializer::skipField()
{
	if (NULL == beginParseLine())
		return;
	endParseLine();
}

TableSerializer::EM_TYPE_COLUMN TableSerializer::_parseColumnType(const char* columnType)
{
	if (strUtil::Strcmp("INT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT;
	}
	else if (strUtil::Strcmp("UINT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_UINT;
	}
	else if (strUtil::Strcmp("INT64", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT64;
	}
	else if (strUtil::Strcmp("UINT64", columnType) == 0)
	{
		return EM_TYPE_COLUMN_UINT64;
	}
	else if (strUtil::Strcmp("FLOAT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_FLOAT;
	}
	else if (strUtil::Strcmp("STRING", columnType) == 0)
	{
		return EM_TYPE_COLUMN_STRING;
	}
	else if (strUtil::Strcmp("STRING_INT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT_ARRAY;
	}
	else if (strUtil::Strcmp("STRING_FLOAT", columnType) == 0)
	{
		return EM_TYPE_COLUMN_FLOAT_ARRAY;
	}
	else if (strUtil::Strcmp("MONEY", columnType) == 0)
	{
		return EM_TYPE_COLUMN_INT;
	}
	else if (strUtil::Strcmp("KEY_VALUE", columnType) == 0)
	{
		return EM_TYPE_COLUMN_STRING;
	}
	else
	{
		//Tools::Log("table[%s]invalid column type = %s", s_ansiFileName, columnType);
		return EM_TYPE_COLUMN_INVALID;
	}
}

void TableSerializer::_checkColumnType(EM_TYPE_COLUMN columnType)
{
	if (m_IsCheckColumn)
	{
		Assert(m_Columns[m_ColumnOfCurrentLine] == columnType);
		if (m_Columns[m_ColumnOfCurrentLine] != columnType)
		{
			/*Tools::Log("\ntable [%s], column [%d] (column[%s] in excel) isn't match!\n"
				"in struct TableRow : %s\n"
				"in .tab file : %s\n",
				s_ansiFileName, 
				m_ColumnOfCurrentLine, 
				ColumnIndexToChar(m_ColumnOfCurrentLine),
				g_ColumnTypeToString[columnType],
				g_ColumnTypeToString[m_Columns[m_ColumnOfCurrentLine]]);*/
		}
	}
}

bool TableSerializer::checkColumnCount() const
{
	if (m_IsCheckColumn && m_ColumnOfCurrentLine != m_ColumnCount)
	{
		char msg[512] = {0};
		strUtil::Snprintf(
			msg, 512, 
			"\nin table[%s], column count isn't match!\ncolumn count:\nTableRow = %d\n *.tab File = %d", 
			s_ansiFileName, m_ColumnOfCurrentLine, m_ColumnCount);
		//Tools::Log(msg);
	}
	return true;
}

} // namespace tabReader

