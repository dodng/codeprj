////////////////////////////////////////////////////////////////////////////////////
// 
// @file: strUtil.h
// @author: by Mr.Chen
// @date: 2014/5/1	19:48
// @brief: 字符串处理类，ref from CYOU and PWRD.
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

	//字符串拷贝
	char* Strcpy(char* pDestination, const char*  pSource);		
	char* Strncpy(char* pDestination, const char* pSource, uint nLength);	
	// 注意！destSizeInByte为目的缓冲区的sizeof，而非strlen
	char* Strcpy_s(char* pDestination, const char* pSource, const int destSizeInByte);

	// 字符串组合
	char* Strcat(char* pDestination, const char* pSource);				
	char* Strncat(char* pDestination, const char* pSource, uint nLength);	

	// 字符串比较
	int Strcmp(const char* pSource1, const char* pSource2);								
	int Strncmp(const char* pSource1, const char*  pSource2, uint nLength);	

	// 计算字符串长度
	int Strlen(const char* pSource);	
	int Utf8len(const char* pSource);

	// 查找字符串
	const char* Strstr(const char* pHaystack, const char* pNeedle);

	//格式化字符串
	int Snprintf (char *buf, int maxlen, const char *format, ...);
	int Vsnprintf(char* buf, int maxlen, const char* format, va_list varList);

	// 分解字符串
	char* Strtok(char* pSource, const char* pDelimit);		
	char* Strtok_s(char*  pSource, const char* pDelimit, char** pContext);
	// 切分字符串
	int	 ConvertStringToVector(const char* strSource, vector<string>& vRet, 
						   const char* szKey="\\/\n", bool bOneOfKey=true, bool bIgnoreEmpty=true);
	// 从buffer中解析出一行
	const char* GetLineFromMemory(char* pStringBuf, int nBufSize, 
		const char* pMemStart, const char* pMemEnd);

	//字符串转整型
	int Strtoint(const char* pSource);
    //字符串转无符号整型
    uint Strtouint(const char*  pSource, char**  pEnd = NULL, int nBase = 10);
	//字符串转长整型
	long Strtol(const char*  pSource, char** pEnd=NULL, int nBase=10);
	//字符串转无符号长整型
	ulong Strtoul(const char*  pSource, char** pEnd=NULL, int nBase=10);
	//字符串转64位整型
	int64 Strtoint64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	//字符串转64位无符号整型
    uint64 Strtouint64(const char*  pSource, char** pEnd = NULL, int nBase = 10);
	// 字符串转浮点型
	float Strtof(const char* pSource);
	double Strtod(const char* pSource, char** pEnd);

	//整型转字符串
	char* Int32toStr(int nValue, char* pString, int nRadix);
	bool Uint64toStr(uint64 i, char* pDest, int base=10);
	bool FloattoStr(float i,char* pDest, int base=10);

	// 去除字符串中空格和tab
	int TrimRight(char* s);
	bool EraseStrLeftBlank(char* pSource);						
	bool EraseStrRightBlank(char* pSource);
	bool EraseStrBlank(char* pSource);

	//判断字符串是否数字
	bool IsInteger(const char* pSource);
	bool IsFloat(const char* pSource);

	//杂项
	void toUpper(std::string &str);
	void toLower(std::string &str);
	std::string bin2hex(const void *data, unsigned int len);
	bool hex2bin(std::string &hex_str, unsigned char *&data, unsigned int len);

    //编码转换
    bool GbkToUtf8(const char *src, string& result);
    bool Utf8ToGbk(const char *src, string& result);

} // namespace strUtil
using namespace strUtil;

#endif	// __STRUTIL_H_
