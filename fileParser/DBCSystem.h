////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBCSystem.h
// @author: by Mr.Chen
// @date: 2014/8/15	10:55
// @brief: 内存数据库系统
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
//数据库
class DBC
{
public:
	//字段数据类型
	enum DBC_FIELD_TYPE
	{
		T_INT		= 0,	//整数
		T_FLOAT	= 1,	//浮点数
		T_STRING	= 2,	//字符串
	};

	//数据段
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
	//按照索引查找
	const DBC_FIELD*	Search_Index_EQU(int iIndexValue) const;
	//按照行
	const DBC_FIELD*	Search_LineNum_EQU(int iLineNum) const;
	//查找某列等于指定值的第一行
	const DBC_FIELD*	Search_First_Column_Equ(int nColumnNum, const DBC_FIELD& value) const;
	//按照位置
	const DBC_FIELD* Search_Position(int nRecordLine, int nColumNum) const;

public:
	const DBCFile*GetDBCFile(void) const { return m_pFileDBC; }
	//取得ID
	int					GetID(void) const;
	//取得列数
	uint				GetFieldsNum(void) const;
	//取得记录的条数
	uint				GetRecordsNum(void) const;
	//打开文本文件，生成一个数据库
	bool				OpenFromTXT(const char* szFileName);

public:
	DBC(int id);
	virtual ~DBC();

protected:
	DBCFile*	m_pFileDBC;
};

//============================================================================
// 所有需要打开的数据库定义
struct _DATABASE_DEFINE
{
	int	 nFileID;					//文件的ID
	const char* pFileName;	//文件名字
};

class DBCSystem : public Singleton<DBCSystem>
{
public:
	//打开系统中所有数据库
	void			OpenAllDataBase(_DATABASE_DEFINE* dbFileList, int nFileListCount);
	//关闭系统中所有数据库
	void			CloseAllDataBase();

	//得到一个已经打开的数据库，返回数据库指针
	const DBC*	GetDataBase(int idDataBase) const;
	const DBC*	GetDataBase(std::string& tableName) const;

private:
	//数据库
	std::map<int, DBC*> m_mapDataBaseBuf;
	std::map<std::string, int> m_mapNameToID;

};

} // namespace tabReader
using namespace tabReader;

#endif	// __DATABASE_H_

