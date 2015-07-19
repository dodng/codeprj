////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBCSystem.h
// @author: by Mr.Chen
// @date: 2014/8/15	10:55
// @brief: �ڴ����ݿ�ϵͳ
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __DATABASE_H_
#define __DATABASE_H_

#include "DBCFile.h"
#include "singleton.h"
#include <vector>
#include <map>
#include <string>
using std::vector;
using std::map;
using std::string;

namespace tabReader {

//----------------------------------------------------------
//���ݿ�
class DBC
{
public:
	//�ֶ���������
	enum DBC_FIELD_TYPE
	{
		T_INT		= 0,	//����
		T_FLOAT	= 1,	//������
		T_STRING	= 2,	//�ַ���
	};

	//���ݶ�
	union DBC_FIELD
	{
		float		fValue;
		int			iValue;
		const char*	pString;	// Just for runtime!

		//Construct
		DBC_FIELD() {}
		DBC_FIELD(int value) { iValue = value; }
		DBC_FIELD(float value) { fValue = value; }
		DBC_FIELD(const char* value) { pString = value; }
	};

public:
	//������������
	const DBC_FIELD*	Search_Index_EQU(int iIndexValue) const;
	//������
	const DBC_FIELD*	Search_LineNum_EQU(int iLineNum) const;
	//����ĳ�е���ָ��ֵ�ĵ�һ��
	const DBC_FIELD*	Search_First_Column_Equ(int nColumnNum, const DBC_FIELD& value) const;
	//����λ��
	const DBC_FIELD* Search_Position(int nRecordLine, int nColumNum) const;

public:
	const DBCFile*GetDBCFile(void) const { return m_pFileDBC; }
	//ȡ��ID
	int					GetID(void) const;
	//ȡ������
	uint				GetFieldsNum(void) const;
	//ȡ�ü�¼������
	uint				GetRecordsNum(void) const;
	//���ı��ļ�������һ�����ݿ�
	bool				OpenFromTXT(const char* szFileName);

public:
	DBC(int id);
	virtual ~DBC();

protected:
	DBCFile*	m_pFileDBC;
};

//============================================================================
// ������Ҫ�򿪵����ݿⶨ��
struct _DATABASE_DEFINE
{
	int	 nFileID;					//�ļ���ID
	const char* pFileName;	//�ļ�����
};

class DBCSystem : public Singleton<DBCSystem>
{
public:
	//��ϵͳ���������ݿ�
	void			OpenAllDataBase(_DATABASE_DEFINE* dbFileList, int nFileListCount);
	//�ر�ϵͳ���������ݿ�
	void			CloseAllDataBase();

	//�õ�һ���Ѿ��򿪵����ݿ⣬�������ݿ�ָ��
	const DBC*	GetDataBase(int idDataBase) const;
	const DBC*	GetDataBase(std::string& tableName) const;

private:
	//���ݿ�
	std::map<int, DBC*> m_mapDataBaseBuf;
	std::map<std::string, int> m_mapNameToID;

};

} // namespace tabReader
using namespace tabReader;

#endif	// __DATABASE_H_

