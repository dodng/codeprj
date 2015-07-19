////////////////////////////////////////////////////////////////////////////////////
// 
// @file: strUtil.h
// @author: by Mr.Chen
// @date: 2014/5/1	19:48
// @brief: �ַ��������࣬ref from CYOU and PWRD.
//          
////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __STRUTIL_H_
#define __STRUTIL_H_

#include "type.h"
#include <vector>
#include <string>
#include <stdarg.h>

using std::string;
using std::vector;

namespace strUtil {

	//�ַ�������
	char* Strcpy(char* pDestination, const char*  pSource);		
	char* Strncpy(char* pDestination, const char* pSource, uint nLength);	
	// ע�⣡destSizeInByteΪĿ�Ļ�������sizeof������strlen
	char* Strcpy_s(char* pDestination, const char* pSource, const int destSizeInByte);

	// �ַ������
	char* Strcat(char* pDestination, const char* pSource);				
	char* Strncat(char* pDestination, const char* pSource, uint nLength);	

	// �ַ����Ƚ�
	int Strcmp(const char* pSource1, const char* pSource2);								
	int Strncmp(const char* pSource1, const char*  pSource2, uint nLength);	

	// �����ַ�������
	int Strlen(const char* pSource);	
	int Utf8len(const char* pSource);

	// �����ַ���
	const char* Strstr(const char* pHaystack, const char* pNeedle);

	//��ʽ���ַ���
	int Snprintf (char *buf, int maxlen, const char *format, ...);
	int Vsnprintf(char* buf, int maxlen, const char* format, va_list varList);

	// �ֽ��ַ���
	char* Strtok(char* pSource, const char* pDelimit);		
	char* Strtok_s(char*  pSource, const char* pDelimit, char** pContext);
	// �з��ַ���
	int	 ConvertStringToVector(const char* strSource, vector<string>& vRet, 
						   const char* szKey="\\/\n", bool bOneOfKey=true, bool bIgnoreEmpty=true);
	// ��buffer�н�����һ��
	const char* GetLineFromMemory(char* pStringBuf, int nBufSize, 
		const char* pMemStart, const char* pMemEnd);

	//�ַ���ת����
	int Strtoint(const char* pSource);
    //�ַ���ת�޷�������
    uint Strtouint(const char*  pSource, char**  pEnd = NULL, int nBase = 10);
	//�ַ���ת������
	long Strtol(const char*  pSource, char** pEnd=NULL, int nBase=10);
	//�ַ���ת�޷��ų�����
	ulong Strtoul(const char*  pSource, char** pEnd=NULL, int nBase=10);
	//�ַ���ת64λ����
	int64 Strtoint64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//�ַ���ת64λ�޷�������
    uint64 Strtouint64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	// �ַ���ת������
	float Strtof(const char* pSource);
	double Strtod(const char* pSource, char** pEnd);

	//����ת�ַ���
	char* Int32toStr(int nValue, char* pString, int nRadix);
	bool Uint64toStr(uint64 i, char* pDest, int base=10);
	bool FloattoStr(float i,char* pDest, int base=10);

	// ȥ���ַ����пո��tab
	int TrimRight(char* s);
	bool EraseStrLeftBlank(char* pSource);						
	bool EraseStrRightBlank(char* pSource);
	bool EraseStrBlank(char* pSource);

	//�ж��ַ����Ƿ�����
	bool IsInteger(const char* pSource);
	bool IsFloat(const char* pSource);

	//����
	void toUpper(std::string &str);
	void toLower(std::string &str);
	std::string bin2hex(const void *data, unsigned int len);
	bool hex2bin(std::string &hex_str, unsigned char *&data, unsigned int len);

    //����ת��
    bool GbkToUtf8(const char *src, string& result);
    bool Utf8ToGbk(const char *src, string& result);

} // namespace strUtil
using namespace strUtil;

#endif	// __STRUTIL_H_
