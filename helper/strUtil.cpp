#include "strUtil.h"
#include <string.h>
#include <stdarg.h>
#include <algorithm>
#include "assert.h"
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#elif _LINUX64
#endif

namespace strUtil {

char* Strcpy(char* pDestination, const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strcpy(pDestination, pSource);
	__LEAVE_FUNCTION
	return NULL;
}

char* Strncpy(char* pDestination, const char* pSource, uint nLength)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strncpy(pDestination, pSource, nLength);
	__LEAVE_FUNCTION
	return NULL;
}

char* Strcpy_s(char* pDestination, const char* pSource, const int destSizeInByte)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	if (Strlen(pSource) >= destSizeInByte)
	{
		Assert(false);
	}

#ifdef _WIN32
	::strcpy_s(pDestination, destSizeInByte / sizeof(char), pSource);
#endif

#ifdef _LINUX64
	::strncpy(pDestination, pSource, destSizeInByte / sizeof(char));
#endif

	pDestination[destSizeInByte / sizeof(char) - 1] = 0;
	return pDestination;
	__LEAVE_FUNCTION
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strcat, Strncat
//	- ��pSource��ָ����NULL��β���ַ�����ӵ�pDestination��ָ���ڴ�����
//	- ����ԭ����'/0'�����'/0'
//	- pDestination��pSource��ָ�����򲻿����ص�
//	- ����ָ��pDestination��ָ��
/////////////////////////////////////////////////////////////////////////////
char* Strcat(char* pDestination, const char* pSource)	
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strcat(pDestination, pSource);
	__LEAVE_FUNCTION
	return NULL;
}

char* Strncat(char* pDestination, const char* pSource, uint nLength)
{
	__ENTER_FUNCTION
	Assert(pDestination);
	Assert(pSource);
	return ::strncat(pDestination, pSource, nLength);
	__LEAVE_FUNCTION
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strcmp, Strncmp
//	- �Ƚ��ַ���pSource1���ַ���pSource2
//	- ��� (pSource1 < pSource2) ����ֵ <0
//	- ��� (pSource2 = pSource2) ����ֵ =0
//	- ��� (pSource1 > pSource2) ����ֵ >0
/////////////////////////////////////////////////////////////////////////////
int Strcmp(const char* pSource1, const char* pSource2)	
{
	__ENTER_FUNCTION
	Assert(pSource1);
	Assert(pSource2);
	return ::strcmp(pSource1, pSource2);
	__LEAVE_FUNCTION
	return -1;
}

int Strncmp(const char* pSource1, const char* pSource2, uint nLength)	
{
	__ENTER_FUNCTION
	Assert(pSource1);
	Assert(pSource2);
	return ::strncmp(pSource1, pSource2, nLength);
	__LEAVE_FUNCTION
	return -1;
}

int Strlen(const char* pSource)			
{
	__ENTER_FUNCTION
	Assert(pSource);
	return (int)::strlen(pSource);
	__LEAVE_FUNCTION
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Utf8len
//	- �����ַ���pSource�ĳ���
//	- �����ַ����ĳ���, ������������
/////////////////////////////////////////////////////////////////////////////
int	 Utf8len(const char* s)
{
	__ENTER_FUNCTION
	Assert(s);
	uchar tcp;
	const uchar* buf = reinterpret_cast<const uchar*>(s);
	int count = 0;
	int len = Strlen(s);
	while ((len) > 0)
	{
		--len;
		tcp = *buf++;
		++count;

		if (tcp < 0x80)
		{
		}
		else if (tcp < 0xE0)
		{
			--len;
			++buf;
		}
		else if (tcp < 0xF0)
		{
			len -= 2;
			buf += 2;
		}
		else
		{
			len -= 3;
			buf += 3;
		}
	}
	if(len < 0)
	{
		//�ַ�������,Ĭ�ϳ���Ϊ0
		count = 0;
	}
	return count;
	__LEAVE_FUNCTION
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strstr
//	- ��pHaystack�ַ�����Ѱ��pNeedle�ַ�����һ�γ��ֵ�λ��
//	- ���ȽϽ�����
//	- ���ص�һ�γ���pNeedleλ�õ�ָ��, ���û���ҵ�����NULL
/////////////////////////////////////////////////////////////////////////////
const char* Strstr(const char* pHaystack, const char* pNeedle)
{
	__ENTER_FUNCTION
	Assert(pHaystack);
	Assert(pNeedle);
	return ::strstr(pHaystack, pNeedle);
	__LEAVE_FUNCTION
	return NULL;
}

int Snprintf(char *pBuffer, int maxlen, const char *pFormat, ...)
{
	__ENTER_FUNCTION
	Assert(pBuffer);
	Assert(pFormat);

	int result;
	va_list argptr ;
	va_start(argptr, pFormat);

//#if defined (_LINUX64)
//	result = vsnprintf(pBuffer, maxlen, pFormat, argptr);
//#elif defined (_WIN32)
//	result = _vsnprintf(pBuffer, maxlen, pFormat, argptr);
//#endif	
	result = Vsnprintf(pBuffer, maxlen, pFormat, argptr);
	va_end(argptr);
	return result;
	__LEAVE_FUNCTION
	return -1;
}

int Vsnprintf(char* buf, int maxlen, const char* format, va_list varList)
{
	__ENTER_FUNCTION
	Assert(buf);
	Assert(format);

	int result = 0;
#if defined (_LINUX64)
	result = vsnprintf(buf, maxlen, format, varList);
#elif defined (_WIN32)
	result = _vsnprintf(buf, maxlen, format, varList);
#endif

	return result;
	__LEAVE_FUNCTION
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtok
//	- �ֽ��ַ���Ϊһ���Ǵ�, pSourceΪ�ֽ���ַ���, pDelimitΪ�ָ���
//	- �״ε���ʱ, pSource����ָ��Ҫ�ֽ���ַ���, 
//	- ֮��ĵ���Ҫ��pSource���ó�NULL
/////////////////////////////////////////////////////////////////////////////
char* Strtok(char* pSource, const char* pDelimit)
{
	__ENTER_FUNCTION
	Assert(pDelimit);
	return ::strtok(pSource, pDelimit);
	__LEAVE_FUNCTION
	return NULL;
}

char* Strtok_s(char* pSource, const char* pDelimit, char** pContext)
{
	__ENTER_FUNCTION
		
	Assert(pDelimit);
#ifdef _WIN32
	return ::strtok_s(pSource, pDelimit, pContext);
#endif

#ifdef _LINUX64
	return ::strtok_r(pSource, pDelimit, pContext);
#endif

	__LEAVE_FUNCTION
	return NULL;
}

int	ConvertStringToVector(const char* strSource, vector<string>& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty)
{
	vRet.clear();

	//------------------------------------------------------------
	//�Ϸ���
	if(!strSource || strSource[0] == '\0') return 0;
	if(!szKey || szKey[0] == '\0') return 0;

	string strSrc = strSource;

	//------------------------------------------------------------
	//���ҵ�һ���ָ��
	string::size_type nLeft = 0;
	string::size_type nRight;
	if(bOneOfKey)
	{
		nRight = strSrc.find_first_of(szKey);
	}
	else
	{
		nRight = strSrc.find(szKey);
	}

	if(nRight == string::npos)
	{
		nRight = strSrc.length();
	}

	while(true)
	{
		string strItem = strSrc.substr(nLeft, nRight-nLeft);
		if(strItem.length() > 0 || !bIgnoreEmpty)
		{
			vRet.push_back(strItem);
		}

		if(nRight == strSrc.length())
		{
			break;
		}

		nLeft = nRight + (bOneOfKey ? 1 : Strlen(szKey));

		if(bOneOfKey)
		{
			string strTemp = strSrc.substr(nLeft);
			nRight = strTemp.find_first_of(szKey);
			if(nRight != string::npos) nRight += nLeft;
		}
		else
		{
			nRight = strSrc.find(szKey, nLeft);
		}

		if(nRight == string::npos)
		{
			nRight = strSrc.length();
		}
	}

	return (int)vRet.size();
}

const char* GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemStart, const char* pMemEnd)
{
	__ENTER_FUNCTION

	//Ѱ����һ�����з�
	const char* pMem = pMemStart;
	if(pMem>=pMemEnd || *pMem==NULL)
		return NULL;

	while(pMem<pMemEnd && pMem-pMemStart+1<nBufSize && *pMem != 0 && *pMem != '\r' && *pMem != '\n') 
		*(pStringBuf++) = *(pMem++);

	//��ӽ�����
	*pStringBuf = 0;

	//�����������������'\r'��'\n'���ҵ���һ����ʼ��λ��
	while(pMem<pMemEnd && *pMem != 0 && (*pMem=='\r' || *pMem=='\n')) 
		pMem++;

	return pMem;

	__LEAVE_FUNCTION
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//	- Atoi
//	- ��pSourceָ����ַ���ת��Ϊ����
//	- ����pSourceǰ��Ŀո��ַ�, ֱ���������ֻ��������ſ�ʼת��
//	- ���������ֻ����ַ�������������ת��
/////////////////////////////////////////////////////////////////////////////
int Strtoint(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::atoi(pSource);
	__LEAVE_FUNCTION
	return -1;
}

uint Strtouint(const char*  pSource, char**  pEnd, int nBase)
{
    __ENTER_FUNCTION
    Assert(pSource);
    return ::strtoul(pSource, pEnd, nBase);
    __LEAVE_FUNCTION
     return (uint)-1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtol
//	- ��pSourceָ����ַ���ת��Ϊ������, nBase����Ľ��Ʒ�ʽ
/////////////////////////////////////////////////////////////////////////////
long Strtol(const char* pSource, char**  pEnd, int nBase)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::strtol(pSource, pEnd, nBase);
	__LEAVE_FUNCTION
	return -1;
}

ulong Strtoul(const char*  pSource, char**  pEnd, int nBase)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::strtoul(pSource, pEnd, nBase);
	__LEAVE_FUNCTION
	return (ulong)-1;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtoint64
//	- ��pSourceָ����ַ���ת��Ϊ64λ����, nBase����Ľ��Ʒ�ʽ
//////////////////////////////////////////////////////////////////////
int64 Strtoint64(const char*  pSource, char**  pEnd, int nBase)
{
#ifdef _WIN32
	return ::_strtoi64(pSource, pEnd, nBase);
#else
	return ::strtoll(pSource, pEnd, nBase);
#endif
}

/////////////////////////////////////////////////////////////////////////////
//	- StrToUInt64
//	- ��pSourceָ����ַ���ת��Ϊ64λ�޷�������, nBase����Ľ��Ʒ�ʽ
//////////////////////////////////////////////////////////////////////
uint64 Strtouint64(const char*  pSource, char**  pEnd, int nBase)
{
#ifdef _WIN32
	return ::_strtoui64(pSource, pEnd, nBase);
#else
	return ::strtoull(pSource, pEnd, nBase);
#endif
}

/////////////////////////////////////////////////////////////////////////////
//	- Atof
//	- ��pSourceָ����ַ���ת��Ϊ������
/////////////////////////////////////////////////////////////////////////////
float Strtof(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::atof(pSource);
	__LEAVE_FUNCTION
	return -1.0f;
}

/////////////////////////////////////////////////////////////////////////////
//	- Strtod
//	- ��pSourceָ����ַ���ת��Ϊ������
/////////////////////////////////////////////////////////////////////////////
double Strtod(const char*  pSource, char**  pEnd)
{
	__ENTER_FUNCTION
	Assert(pSource);
	return ::strtod(pSource, pEnd);
	__LEAVE_FUNCTION
	return -1.0f;
}

char* Int32toStr(int nValue, char* pString, int nRadix)
{
	__ENTER_FUNCTION
	Assert(pString);

#if defined (_WIN32)
	return ::itoa(nValue, pString, nRadix);

#elif defined (_LINUX64)
	char* pEnd = pString;
	char* pStart = pString;
	//��ת����Ϊ0
	if (0 == nValue)
	{
		pString[0] = '0';
		pString[1] = 0;
		return pString;
	}
	//��ת����Ϊ����������10
	if ((nValue < 0)&&( 10 == nRadix))
	{
		//���÷���
		pString[0] = '-';
		pEnd++;
		pStart++;
		nValue = -nValue;
	}
	//����nRadix����ת�����֣������ַ���
	while (nValue != 0)
	{
		int nMode = nValue % nRadix;
		nValue = nValue / nRadix;
		*pEnd++ = (nMode < 10) ? '0' + nMode:'a' + nMode - 10;
	}
	*pEnd-- = 0;
	//��ת�ַ���
	char cTemp;
	while (pEnd > pStart)
	{
		cTemp = *pEnd;
		*pEnd = *pStart;
		*pStart = cTemp;
		pEnd--;
		pStart++;
	}
	return pString;
#endif
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

bool Uint64toStr(uint64 i, char* pDest, int base)
{
__ENTER_FUNCTION
	if (base==16)
	{
		sprintf(pDest, "%llx", i);
	}
	else
	{
		sprintf(pDest, "%llu", i);
	}
//#ifdef __WINDOWS_
//	_ui64toa(i, pDest, base);
//#else
//	
//#endif
	return true;
__LEAVE_FUNCTION
	return false;
}

bool FloattoStr(float i,char* pDest, int base/*=10*/)
{
	if (base==16)
	{
		sprintf(pDest, "%lx", i);
	}
	else
	{
		sprintf(pDest, "%lf", i);
	}
	return true;
}

int TrimRight( char* s )
{
	int stringLength = ::strlen(s);
	int deleteEndCount = 0;
	//�����ĩβ�Ŀ��ַ�
	for (int i = stringLength - 1; i > 0; --i)
	{
		if (!::isspace(s[i]))
			break;
		s[i] = 0;
		deleteEndCount++;
	}
	return deleteEndCount;
}

/////////////////////////////////////////////////////////////////////////////
//	- EraseStrLeftBlank
//	- ɾ���ַ������Ŀո��ַ�
/////////////////////////////////////////////////////////////////////////////
bool EraseStrLeftBlank(char *pSrc)
{
	__ENTER_FUNCTION
	Assert(pSrc);
	if (pSrc == NULL)
	{
		return false;
	}
	while (*pSrc==' ' || *pSrc=='\t')
	{
		memmove(pSrc, pSrc+1, strUtil::Strlen(pSrc));
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//	- EraseStrRightBlank
//	- ɾ���ַ����Ҳ�Ŀո��ַ�
/////////////////////////////////////////////////////////////////////////////
bool EraseStrRightBlank(char *pSrc)
{
	__ENTER_FUNCTION
	Assert(pSrc);

	if (pSrc == NULL)
	{
		return false;
	}
	char* pTail = pSrc+strUtil::Strlen(pSrc)-1;
	while (*pTail==' ' || *pTail=='\t')
	{
		*pTail = '\0';
		pTail = pSrc+strUtil::Strlen(pSrc)-1;
	}
	return true;
	__LEAVE_FUNCTION
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//	- EraseStrBlank
//	- ɾ���ַ����еĿո��ַ�
/////////////////////////////////////////////////////////////////////////////
bool EraseStrBlank(char *pSrc)
{
	__ENTER_FUNCTION
	Assert(pSrc);

	bool bErase1 = EraseStrLeftBlank(pSrc);
	bool bErase2 = EraseStrRightBlank(pSrc);
	return bErase1&&bErase2;

	__LEAVE_FUNCTION
	return false;
}

bool IsInteger(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);

	if (pSource[0] == '-')
	{
		if (Strlen(pSource) == 1)
		{
			return false;
		}
		++pSource;
	}
	while(*pSource != '\0')
	{
		if((*pSource < '0') || (*pSource > '9'))
		{
			return false;
		}
		++pSource;
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool IsFloat(const char* pSource)
{
	__ENTER_FUNCTION
	Assert(pSource);

	if (pSource[0] == '-')
	{
		if (Strlen(pSource) == 1)
		{
			return false;
		}
		++pSource;
	}

	while(*pSource != '\0')
	{
		if(((*pSource >= '0') && (*pSource <= '9'))||(*pSource == '.'))
		{
			++pSource;
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
	__LEAVE_FUNCTION
	return false;
}

void toUpper(std::string &str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void toLower(std::string &str)
{
	transform(str.begin(),str.end(), str.begin(), ::tolower);
}

std::string bin2hex(const void *data, unsigned int len)
{
	uint8 *bytes = (uint8*)data;
	std::string ret;
	ret.reserve(len*2+1);

	char tmp[8];
	for(unsigned int i=0; i<len; ++i)
	{
		Snprintf(tmp,sizeof(tmp), "%02X", bytes[i]);
		ret += tmp;
	}
	return ret;
}

bool hex2bin(std::string &hex_str, uint8*&data, unsigned int len)
{
	toUpper(hex_str);

	if(hex_str.size() < len * 2) 
	{
		return false;
	}

	char tmp[16]={0};
	unsigned int ch;
	unsigned int idx=0;
	for(unsigned int i = 0; i < len*2;) 
	{
		::memcpy(tmp, hex_str.substr(i, 2).c_str(),2);
		::sscanf(tmp, "%02X", &ch);
		data[idx++] = (uint8)ch;
		i+=2;
	}
	return true;
}

bool GbkToUtf8(const char *src, string& result)
{
#ifdef _WIN32
 
    //TODO: check 
    int len = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_UTF8,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);
    if (strTemp.find('?') != std::string::npos)
    {
        strTemp.assign(src);
    }

    result = strTemp;
    delete[]szGBK;
    delete[]wszGBK;

    return true;

#elif _LINUX64

    if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//����ת��Ϊunicodeǰ����,��ǰΪgbk����    
    {
        return false;
    }    
    int unicodeLen=mbstowcs(NULL,src,0);//����ת����ĳ���
    if(unicodeLen<=0)    
    {
        return false;    
    }
    wchar_t *unicodestr = new wchar_t[unicodeLen+1];
    mbstowcs(unicodestr,src,strUtil::Strlen(src));//��gbkת��Ϊunicode
    if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//����unicodeת�������,��ǰΪutf8    
    {
        return false;    
    }   
    int len = wcstombs(NULL,unicodestr,0);//����ת����ĳ���    
    if(len <= 0)    
    {
        return false;    
    }
    char* dest = new char[len+1];
    wcstombs(dest,unicodestr,len);
    dest[len]=0;//��ӽ�����
    result = dest;

    delete[] unicodestr;
    delete[] dest;
    return true;
#endif
}

bool Utf8ToGbk(const char *src, string& result)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);
    if (strTemp.find('?') != std::string::npos)
    {
        strTemp.assign(src);
    }

    result = strTemp;
    delete[]szGBK;
    delete[]wszGBK;
    
    return true;

#elif _LINUX64

    if(NULL== setlocale(LC_ALL,"zh_CN.utf8"))//����ת��Ϊunicodeǰ����,��ǰΪutf8����    
	{
		return false;
	}    
	int unicodeLen = mbstowcs(NULL,src,0);//����ת����ĳ���
	if(unicodeLen <= 0)    
	{
		return false;    
	}
    wchar_t *unicodestr = new wchar_t[unicodeLen+1];
    mbstowcs(unicodestr,src,strUtil::Strlen(src));//��utf8ת��Ϊunicode
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//����unicodeת�������,��ǰΪgbk    
	{
		return false;    
	}   
	int len = wcstombs(NULL,unicodestr,0);//����ת����ĳ���    
	if(len<=0)    
	{
		return false;    
	}
    char* dest = new char[len+1];
	wcstombs(dest,unicodestr,len);
	dest[len]=0;//��ӽ�����

    result = dest;
    delete[] unicodestr;
    delete[] dest;
    
    return true;

#endif
}

} // namespace strUtil
