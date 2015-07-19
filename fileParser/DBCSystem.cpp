#include "DBCSystem.h"
#include "macroDef.h"
#include "assert.h"
#include <stdio.h>

WX_IMPLEMENT_SINGLEON(tabReader::DBCSystem);

namespace tabReader {

DBC::DBC(int id)
{
	m_pFileDBC = new DBCFile(id);
}

DBC::~DBC()
{
	SAFE_DELETE(m_pFileDBC);
}

const DBC::DBC_FIELD* DBC::Search_Index_EQU(int iIndexValue) const 
{
	return (DBC_FIELD*)m_pFileDBC->Search_Index_EQU(iIndexValue);
}

const DBC::DBC_FIELD* DBC::Search_LineNum_EQU(int iLineNum) const
{
	return  (DBC_FIELD*)m_pFileDBC->Search_Posistion(iLineNum, 0); 
}

//����ĳ�е���ָ��ֵ�ĵ�һ��
const DBC::DBC_FIELD* DBC::Search_First_Column_Equ(int nColumnNum, const DBC_FIELD& value) const
{
	return (DBC_FIELD*)m_pFileDBC->Search_First_Column_Equ(nColumnNum, value.iValue);
}

const DBC::DBC_FIELD* DBC::Search_Position(int nRecordLine, int nColumNum) const
{
	return (DBC_FIELD*)m_pFileDBC->Search_Posistion(nRecordLine,nColumNum);
}

//ȡ��ID
int DBC::GetID(void) const
{
	return m_pFileDBC->GetID(); 
}

//ȡ������
uint DBC::GetFieldsNum(void)const
{ 
	return m_pFileDBC->GetFieldsNum(); 
}

//ȡ�ü�¼������
uint DBC::GetRecordsNum(void)const
{ 
	return m_pFileDBC->GetRecordsNum(); 
}

bool DBC::OpenFromTXT(const char* szFileName)
{
	return m_pFileDBC->OpenFromTXT(szFileName);
}

//============================================================================

void DBCSystem::OpenAllDataBase(_DATABASE_DEFINE* dbFileList, int nFileListCount)
{
	__ENTER_FUNCTION

	// �����������ļ�
	for(int i=0; i<nFileListCount; i++)
	{
		//����DBC����
		DBC* pDataBase = new DBC(dbFileList[i].nFileID);
		if( !pDataBase )
		{
			printf("Create database memory failed! File name is %s!\n", dbFileList[i].pFileName);
		}
		//���������ļ�
		if(!(pDataBase->OpenFromTXT(dbFileList[i].pFileName)))
		{
			printf("Read database file %s is failed!\n", dbFileList[i].pFileName);
		}
		//����Ƿ��ظ�
		if(m_mapDataBaseBuf.find(dbFileList[i].nFileID) != m_mapDataBaseBuf.end())
		{
			printf("Database file %s is existed!\n", dbFileList[i].pFileName);
		}
		//���������ļ��б�
		m_mapDataBaseBuf.insert(std::make_pair(dbFileList[i].nFileID, pDataBase));
		std::string tableName = dbFileList[i].pFileName;
		m_mapNameToID.insert(std::make_pair(tableName, dbFileList[i].nFileID));
		printf("Create Database File %s OK!\n", dbFileList[i].pFileName);
	}

	__LEAVE_FUNCTION
}

void DBCSystem::CloseAllDataBase()
{
	__ENTER_FUNCTION

	std::map<int, DBC*>::iterator it;
	for(it = m_mapDataBaseBuf.begin(); it!=m_mapDataBaseBuf.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	m_mapDataBaseBuf.clear();

	__LEAVE_FUNCTION
}

const DBC* DBCSystem::GetDataBase(int idDataBase) const
{
	__ENTER_FUNCTION
	std::map<int, DBC* >::const_iterator it = m_mapDataBaseBuf.find(idDataBase);
	if( it != m_mapDataBaseBuf.end() )
		return (const DBC*)it->second;
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

const DBC* DBCSystem::GetDataBase(std::string& tableName) const
{
__ENTER_FUNCTION

	std::map<std::string, int>::const_iterator it = m_mapNameToID.find(tableName);
	if( it == m_mapNameToID.end() )
	{
		return NULL;
	}
	return GetDataBase(it->second);

__LEAVE_FUNCTION
	return NULL;
}

} // namespace tabReader
