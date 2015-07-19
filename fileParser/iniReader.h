////////////////////////////////////////////////////////////////////////////////////
// 
// @file: iniReader.h
// @author: by Mr.Chen
// @date: 2014/8/21	10:24
// @brief: INI�����ļ�
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __INI_READER_H_
#define __INI_READER_H_

#include "type.h"

namespace FileReader {

////////////////////////////////////////////////////////////////////////////////
//	class IniReader
////////////////////////////////////////////////////////////////////////////////
class IniReader
{
	enum
	{
		INI_SECT_LEN	= 256,		//�γ�
		INI_VALUE_LEN	= 1024,		//ֵ��
		INI_KEY_LEN		= 256,		//����
		INI_LINE_LEN	= 2048,		//�г�
		MAX_FILE_NAME_LENGTH = 1024, 
	};
public:
	IniReader();
	IniReader(const char* pFileName);
	~IniReader();
public:
	// ���ļ�
	bool			open(const char* pFileName);
	// �����ļ�
	bool			save();
	
	// ��ȡ�������ɹ����ؼ�ֵ��ʧ�ܷ���-1
	int				readInt(const char* pSect, const char* pKey) const;
	// ��ȡ���������ɹ����ؼ�ֵ��ʧ�ܷ���-1
	float			readFloat(const char* pSect, const char* pKey) const;
	// ��ȡ�ַ�������ֵͨ������pOut��ȡ������ֵ��ʾʧ�ܻ�ɹ�
	bool			readString(const char* pSect, const char* pKey, char* pOut, int Len) const;

	// д������
	bool			writeInt(const char* pSect, const char* pKey, int iValue);
	// д�븡����
	bool			writeFloat(const char* pSect, const char* pKey, float fValue);
	// д���ַ���
	bool			writeString(const char* pSect, const char* pKey, char* pValue);
private:
	/************************************************************************/
	/* ��ʼ�����������ö�ָ�������Աָ����ε��ڴ�ƫ��λ��
	/************************************************************************/
	void			_initSectIndex();

	/************************************************************************/
	/* ���Ҷε�ƫ��λ�ã�ʧ�ܷ���-1
	/************************************************************************/
	int				_findSect(const char* pSect) const;

	/************************************************************************/
	/* ���Ҽ���ƫ��λ�ã�ʧ�ܷ���-1
	/************************************************************************/
	int				_findKey(int Begin, const char* pKey) const;

	/************************************************************************/
	/* �ҵ���һ�е���ʼλ��
	/************************************************************************/
	int				_findNextLineBegin(int CurPos) const;

	/************************************************************************/
	/* ��ȡ����Ӧ��ֵ
	/************************************************************************/
	bool			_getValue(int Begin, char* pValue, int Len) const;

	/************************************************************************/
	/* ͨ��λ�û�����ڶ�λ��������±�
	/************************************************************************/
	int				_getIndexByPos(int Pos) const;

	/************************************************************************/
	/* ��ȡһ�У�������һ�е���ʼλ��
	/************************************************************************/
	int				_getLine(char* pBuff, int BuffLen, int BeginPos) const;

public:
	/************************************************************************/
	/* ��Ӷ�
	/************************************************************************/
	void			addSect(const char* pSect);

	/************************************************************************/
	/* ��Ӽ�ֵ��
	/************************************************************************/
	bool			addKeyValuePair(const char* pSect, const char* pKey, const char* pValue);

	/************************************************************************/
	/* �޸ļ�ֵ��
	/************************************************************************/
	bool			modifyKeyValuePair(const char* pSect, const char* pKey, const char* pValue);

private:
	//�ļ���
	char			m_FileName[MAX_FILE_NAME_LENGTH];
	//�ļ����ݳ���
	int				m_DataLen;
	//�ļ�����
	char*			m_pData;
	//�����ڴ����ʼλ������
	int*			m_aSectIndex;
	//������
	int				m_SectCount;
};

} //namespace FileReader
using namespace FileReader;

#endif //__INI_READER_H_

