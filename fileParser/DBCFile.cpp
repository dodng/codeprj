#include "DBCFile.h"
#include <string>
#include <map>
#include <exception>
#include "strUtil.h"
#include "macroDef.h"
#include "assert.h"
#include <stdio.h>
#include <string.h>

namespace tabReader
{
template < DBCFile::FIELD_TYPE T>
bool DBCFile::isFieldEqual(const FIELD& a, const FIELD& b)
{
	if(T == T_INT)
	{
		return a.iValue==b.iValue;
	}
	else if(T == T_FLOAT)
	{
		return a.fValue==b.fValue;
	}
	else
	{
		try
		{
			return (strUtil::Strcmp(a.pString, b.pString)==0);
		}
		catch(...)
		{
			return false;
		}
	}
}

DBCFile::DBCFile(uint id)
{
	m_ID = id;
	m_pStringBuf = NULL;
	m_nIndexColum = -1;
}

DBCFile::~DBCFile()
{
	SAFE_DELETE_ARRAY(m_pStringBuf);
}

bool DBCFile::OpenFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	Assert(pMemory && pDeadEnd);
	//----------------------------------------------------
	//判断是否是二进制格式
	if(pDeadEnd - pMemory >=sizeof(FILE_HEAD) && *((uint*)pMemory)==0XDDBBCC00)
	{
		return OpenFromMemoryImpl_Binary(pMemory, pDeadEnd, szFileName);
	}
	else
	{
		return OpenFromMemoryImpl_Text(pMemory, pDeadEnd, szFileName);
	}
}

bool DBCFile::OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	register const char* pMem = pMemory;

	//----------------------------------------------------
	//Read Head
	FILE_HEAD theHead;
	memcpy(&theHead, pMem, sizeof(FILE_HEAD));
	if(theHead.m_Identify != 0XDDBBCC00 )
	{
		return false;
	}
	//check memory size
	if(	sizeof(FILE_HEAD) + 
		sizeof(uint)*theHead.m_nFieldsNum + 
		sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum +
		theHead.m_nStringBlockSize > (uint16)(pDeadEnd-pMemory))
	{
		return false;
	}

	pMem += sizeof(FILE_HEAD);

	//----------------------------------------------------
	//Init 
	m_nRecordsNum = theHead.m_nRecordsNum;
	m_nFieldsNum  = theHead.m_nFieldsNum;
	m_nStringBufSize = theHead.m_nStringBlockSize;

	//---------------------------------------------
	//Create String Blok
	m_pStringBuf = new char[theHead.m_nStringBlockSize];
	if(!m_pStringBuf) return false;

	//------------------------------------------------------
	// Read Field Types
	vector< uint > vFieldType;
	vFieldType.resize(theHead.m_nFieldsNum);
	memcpy(&(vFieldType[0]), pMem, sizeof(uint)*theHead.m_nFieldsNum);
	pMem += sizeof(uint)*theHead.m_nFieldsNum;

	//Check it!
	m_theType.resize(theHead.m_nFieldsNum);
	for(int i=0; i<(int)theHead.m_nFieldsNum; i++)
	{
		switch(vFieldType[i])
		{
		case T_INT:
		case T_FLOAT:
		case T_STRING:
			m_theType[i] = (FIELD_TYPE)vFieldType[i];
			break;

		default:
			delete[] (m_pStringBuf);
			return false;
		}
	}

	//------------------------------------------------------
	// Read All Field
	m_vDataBuf.resize(theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	memcpy(&(m_vDataBuf[0]), pMem, sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum);
	pMem += sizeof(FIELD)*theHead.m_nRecordsNum * theHead.m_nFieldsNum;

	//------------------------------------------------------
	// Read String Block
	memcpy(m_pStringBuf, pMem, m_nStringBufSize);
	m_pStringBuf[m_nStringBufSize-1]=0;

	//------------------------------------------------------
	// TO runtime address
	for(int j=0; j<(int)theHead.m_nFieldsNum; j++)
	{
		if(vFieldType[j] != T_STRING) continue;
		for(int i=0; i<(int)theHead.m_nRecordsNum; i++)
		{
			m_vDataBuf[i*GetFieldsNum()+j].pString += reinterpret_cast<unsigned long long>(m_pStringBuf);
		}
	}

	//------------------------------------------------------
	//生成索引列
	CreateIndex(0, szFileName);

	return true;
}

bool DBCFile::OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd, const char* szFileName)
{
	char szLine[1024*10] = {0};
	//读第一行
	register const char* pMem = pMemory;
	pMem = strUtil::GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return false;

	//分析列数和类型
	pMem = strUtil::GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
	if(!pMem) return false;

	//分解
	vector< std::string > vRet;
	strUtil::ConvertStringToVector(szLine, vRet, "\t", true, true);
	if(vRet.empty()) return false;

	//生成Field Types
	FILEDS_TYPE vFieldsType;
	vFieldsType.resize(vRet.size());

	for(int i=0; i<(int)vRet.size(); i++) 
	{
		if(vRet[i] == "INT") vFieldsType[i] = T_INT;
		else if(vRet[i] == "FLOAT") vFieldsType[i] = T_FLOAT;
		else if(vRet[i] == "STRING") vFieldsType[i] = T_STRING;
		else if(vRet[i] == "STRING_INT") vFieldsType[i] = T_STRING; //TODO
		else
		{
			return false;
		}
	}

	//--------------------------------------------------------------
	//初始化
	int nRecordsNum	= 0;
	int nFieldsNum	= (int)vFieldsType.size();

	//临时字符串区
	vector< std::pair< std::string, int > >	vStringBuf;
	//检索表
	std::map< std::string, int >					mapStringBuf;

	//--------------------------------------------------------------
	//开始读取
	int nStringBufSize = 0;
	do
	{
		//读取一行
		pMem = strUtil::GetLineFromMemory(szLine, 1024*10, pMem, pDeadEnd);
		if(!pMem) break;;

		//是否是注释行
		if(szLine[0] == '#') continue;

		//分解
		strUtil::ConvertStringToVector(szLine, vRet, "\t", true, false);

		//列数不对
		if(vRet.empty()) continue;
        if(vRet.size() != nFieldsNum) 
		{
			//补上空格
			if((int)vRet.size() < nFieldsNum)
			{
				int nSubNum = nFieldsNum-(int)vRet.size();
				for(int i=0; i<nSubNum; i++)
				{
					vRet.push_back("");
				}
			}
		}

		//第一列不能为空
		if(vRet[0].empty()) continue;

		for(register int i=0; i<nFieldsNum; i++)
		{
			FIELD newField;
			switch(vFieldsType[i])
			{
			case T_INT:
				newField.iValue = strUtil::Strtoint(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_FLOAT:
				newField.fValue = strUtil::Strtof(vRet[i].c_str());
				m_vDataBuf.push_back(newField);
				break;

			case T_STRING:
				if(vRet[i].empty())
				{
					newField.iValue = 0;
				}
				else
				{
					//const char * p = vRet[i].c_str();
					std::map< std::string, int >::iterator it = mapStringBuf.find(vRet[i]);
					if(it == mapStringBuf.end())
					{
						vStringBuf.push_back(std::make_pair(vRet[i], nStringBufSize));
						mapStringBuf.insert(std::make_pair(vRet[i], (int)vStringBuf.size()-1));
						newField.iValue = nStringBufSize + 1; // first CHAR is '\0' for blank string
	
						nStringBufSize += strUtil::Strlen(vRet[i].c_str()) + 1;
					}
					else
					{
						newField.iValue = vStringBuf[it->second].second + 1;
					}
				}

				m_vDataBuf.push_back(newField);
				break;
			}
		}

		nRecordsNum++;
	}while(true);

	//--------------------------------------------------------
	//生成正式数据库
	m_nRecordsNum = nRecordsNum;
	m_nFieldsNum  = nFieldsNum;
	m_nStringBufSize = nStringBufSize+1;

	//Create String Blok
	m_pStringBuf = new char[m_nStringBufSize];
	if(!m_pStringBuf) return false;

	//------------------------------------------------------
	// Create Field Types
	m_theType = vFieldsType;

	//------------------------------------------------------
	// Create String Block
	uchar byBlank = '\0';
	m_pStringBuf[0] = '\0';

	register char* p = m_pStringBuf + 1;
	for(int i=0; i<(int)vStringBuf.size(); i++)
	{
		memcpy(p, vStringBuf[i].first.c_str(), vStringBuf[i].first.size());	
		p += vStringBuf[i].first.size();

		*(p++) = '\0';
	}

	//------------------------------------------------------
	// Relocate String Block
	for(register int i=0; i<nFieldsNum; i++)
	{
		if(vFieldsType[i] != T_STRING) continue;
		for(register int j=0; j<nRecordsNum; j++)
		{
			FIELD& theField = m_vDataBuf[j*nFieldsNum+i];
			theField.pString = m_pStringBuf + theField.iValue;
		}
	}

	//------------------------------------------------------
	//生成索引列
	CreateIndex(0, szFileName);

	return true;
}

bool DBCFile::OpenFromTXT(const char* szFileName)
{
	Assert(szFileName);
	//----------------------------------------------------
	//打开文件
	FILE* fp = fopen(szFileName, "rb");
	if(NULL == fp) return false;

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	//读入内存
	char* pMemory = new char[nFileSize+1];
	fread(pMemory, 1, nFileSize, fp);
	pMemory[nFileSize] = 0;

	bool bRet = OpenFromMemory(pMemory, pMemory+nFileSize+1, szFileName);
	SAFE_DELETE_ARRAY(pMemory);
	return bRet;
}

void DBCFile::CreateIndex(int nColum, const char* szFileName)
{
	if(nColum <0 || nColum >= m_nFieldsNum || m_nIndexColum==nColum) return;

	m_hashIndex.clear();

	for(int i=0; i<m_nRecordsNum; i++)
	{
		FIELD* p = &(m_vDataBuf[i*m_nFieldsNum]);

		FIELD_HASHMAP::iterator itFind = m_hashIndex.find(p->iValue);
		if(itFind != m_hashIndex.end())
		{
			char szTemp[260];
			strUtil::Snprintf(szTemp, 260, "[%s]Multi index at line %d(SAME:value=%d)", szFileName, i+1, p->iValue);
#ifdef GAME_CLIENT
			throw std::string(szTemp);
#else
			AssertEx(false,szTemp);
#endif
			return;
		}
		m_hashIndex.insert(std::make_pair(p->iValue, p));
	}
}

const DBCFile::FIELD* DBCFile::Search_Index_EQU(int iIndex) const
{
	FIELD_HASHMAP::const_iterator itFind = m_hashIndex.find(iIndex);
	if(itFind == m_hashIndex.end()) return NULL;

	return itFind->second;
}

const DBCFile::FIELD* DBCFile::Search_Posistion(int nRecordLine, int nColumNum) const
{
	int nPosition = nRecordLine*GetFieldsNum() + nColumNum;

	if(nPosition <0 || nPosition >= (int)m_vDataBuf.size()) 
	{
		char szTemp[260];
		strUtil::Snprintf(szTemp, 260, "DBC:Invalid search request[rec=%d, col=%d]", nRecordLine, nColumNum);
#ifdef GAME_CLIENT
		throw std::string(szTemp);
#else
		AssertEx(false, szTemp);
#endif
		return NULL;
	}

	return &(m_vDataBuf[nPosition]);
}

//查找某列等于指定值的第一行
const DBCFile::FIELD* DBCFile::Search_First_Column_Equ(int nColumnNum, const FIELD& value) const
{
	if(nColumnNum < 0 || nColumnNum >= m_nFieldsNum) return 0;
	FIELD_TYPE type = m_theType[nColumnNum];

	for(register int i=0; i<m_nRecordsNum; i++)
	{
		const FIELD& theField = m_vDataBuf[m_nFieldsNum*i + nColumnNum];
		
		bool bRet;
		if(T_INT == type) 
			bRet = isFieldEqual<T_INT>(theField, value);
		else if(T_FLOAT == type)
			bRet = isFieldEqual<T_FLOAT>(theField, value);
		else
			bRet = isFieldEqual<T_STRING>(theField, value);

		if(bRet) return &(m_vDataBuf[m_nFieldsNum*i]);
	}

	return 0;
}

} // namespace DBC
