#include "iniReader.h"
#include "assert.h"
#include "strUtil.h"
#include "macroDef.h"
#include <stdio.h>

#if defined(_LINUX64)
#include <string.h>          // memset
#endif

namespace FileReader {

IniReader::IniReader()
{
	__ENTER_FUNCTION
	memset(m_FileName, 0, sizeof(m_FileName));
	m_DataLen = 0;
	m_pData = NULL;
	m_aSectIndex = NULL;
	m_SectCount = 0;
	__LEAVE_FUNCTION
}

IniReader::IniReader(const char *pFileName)
{
	__ENTER_FUNCTION
	memset(m_FileName, 0, sizeof(m_FileName));
	strUtil::Strncpy(m_FileName, pFileName, sizeof(m_FileName)-1);
	m_DataLen = 0;
	m_pData = NULL;
	m_aSectIndex = NULL;
	m_SectCount = 0;
	bool ret = open(pFileName);
	if (!ret)
	{
		Assert(0);
	}
	__LEAVE_FUNCTION
}

IniReader::~IniReader()
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(m_pData);
	SAFE_DELETE_ARRAY(m_aSectIndex);
	__LEAVE_FUNCTION
}

bool IniReader::open(const char* pFileName)
{
	__ENTER_FUNCTION

	//�ж��ļ����Ƿ���Ч
	Assert(pFileName!=NULL);
	if (m_FileName[0] == '\0')
	{
		strUtil::Strncpy(m_FileName, pFileName, sizeof(m_FileName)-1);
	}
	
	//open file
	FILE*	 fp = ::fopen(m_FileName, "r+b");
	if (fp == NULL)
	{
		AssertSpecial(0, "Can't Open File!, FileName = %s ", m_FileName);
	}
	//get file length
	::fseek(fp, 0, SEEK_END);
	m_DataLen = (int)::ftell(fp);

	//read file
	::fseek(fp, 0, SEEK_SET);
	m_pData = new char[m_DataLen+1];
	Assert(m_pData);
	::fread(m_pData, 1, m_DataLen, fp);
	m_pData[m_DataLen] = '\0';

	//close file
	::fclose(fp);
	fp = NULL;

	uint16 uFlag = 0;
	memcpy(&uFlag, m_pData, sizeof(uFlag));
	if((uFlag == 0xFFFE) || (uFlag == 0xFEFF))
	{
		AssertSpecial(0, "Can't Read Unicode File!, FileName = %s ", m_FileName);
	}

	//��ʼ������ʼλ������
	_initSectIndex();

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::save()
{
	__ENTER_FUNCTION
	Assert(m_FileName[0]!='\0');

	FILE* pFile = ::fopen(m_FileName, "wb");
	Assert(pFile!=NULL);
	
	::fseek(pFile, 0, SEEK_SET);
	::fwrite(m_pData, m_DataLen, 1, pFile);
	::fclose(pFile);

	return true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::readString(const char* pSect, const char* pKey, char* pOut, int Len) const
{
	__ENTER_FUNCTION
	Assert(pSect != NULL);
	Assert(pKey != NULL);

	//�ҵ��ε���ʼƫ��
	int Pos = _findSect(pSect);
	if (Pos == -1)
	{
		return false;
	}

	//������һ�У���ֵ���У�
	Pos = _findNextLineBegin(Pos);
	if (Pos == -1)
	{
		return false;
	}

	//���Ҽ�,û�ҵ�
	Pos = _findKey(Pos, pKey);
	if (Pos == -1)
	{
		return false;
	}

	//��ȡֵ
	return _getValue(Pos, pOut, Len);
	__LEAVE_FUNCTION
	return false;
}

int IniReader::readInt(const char* pSect, const char* pKey) const
{
	__ENTER_FUNCTION

	char Buff[INI_VALUE_LEN] = {0};
	memset(Buff, 0, sizeof(Buff));
	if (!readString(pSect, pKey, Buff, INI_VALUE_LEN))
	{
		return -1;
	}
	if (!strUtil::IsInteger(Buff))
	{
		return -1;
	}
	return strUtil::Strtoint(Buff);

	__LEAVE_FUNCTION
	return -1;
}

float IniReader::readFloat(const char* pSect, const char* pKey) const
{
	__ENTER_FUNCTION

	char Buff[INI_VALUE_LEN] = {0};
	memset(Buff, 0, sizeof(Buff));
	if (!readString(pSect, pKey, Buff, INI_VALUE_LEN))
	{
		return -1;
	}
	if (!strUtil::IsFloat(Buff))
	{
		return -1;
	}
	return strUtil::Strtof(Buff);

	__LEAVE_FUNCTION
	return -1;
}

bool IniReader::writeInt(const char* pSect, const char* pKey, int iValue)
{
	__ENTER_FUNCTION
	char Buff[INI_KEY_LEN+INI_VALUE_LEN];
	memset(Buff, 0, sizeof(Buff));
	sprintf(Buff, "%d", iValue);
	return writeString(pSect, pKey, Buff);
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::writeFloat(const char* pSect, const char* pKey, float fValue)
{
	__ENTER_FUNCTION
	char Buff[INI_KEY_LEN+INI_VALUE_LEN];
	memset(Buff, 0, sizeof(Buff));
	sprintf(Buff, "%f", fValue);
	return writeString(pSect, pKey, Buff);
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::writeString(const char* pSect, const char* pKey, char* pValue)
{
	__ENTER_FUNCTION
	Assert(pSect!=NULL);
	Assert(pKey!=NULL);
	Assert(pValue!=NULL);

	//���Ҷ�
	int iFind = _findSect(pSect);
	//���������½�
	if (iFind == -1)
	{
		addSect(pSect);
		iFind = _findSect(pSect);
		Assert(iFind != -1);
	}

	//������һ�У���ֵ���У�
	iFind = _findNextLineBegin(iFind);
	if (iFind == -1)
	{
		return addKeyValuePair(pSect, pKey, pValue);
	}

	//���Ҽ�,û�ҵ������
	iFind = _findKey(iFind, pKey);
	if (iFind == -1)
	{
		return addKeyValuePair(pSect, pKey, pValue);
	}
	//�ҵ����޸�
	return modifyKeyValuePair(pSect, pKey, pValue);

	__LEAVE_FUNCTION
	return false;
}

bool IniReader::addKeyValuePair(const char* pSect, const char* pKey, const char* pValue)
{
	__ENTER_FUNCTION

	int iFind1 = _findSect(pSect);
	if (iFind1 == -1)
	{
		return false;
	}
	
	int iFind2 = _findKey(iFind1, pKey);
	if (iFind2 != -1)
	{
		return false;
	}
	
	//����λ��
	int InsertPos = -1;
	int Index = _getIndexByPos(iFind1);
	if (Index < m_SectCount-1)
	{
		InsertPos = m_aSectIndex[Index+1]-1;
	}
	else
	{
		InsertPos = m_DataLen;
	}
	
	char KeyValuePair[INI_KEY_LEN+INI_VALUE_LEN];
	memset(KeyValuePair, 0, sizeof(KeyValuePair));

	int KeyLen = strUtil::Strlen(pKey);
	int ValueLen = strUtil::Strlen(pValue);

	//��ӻ���
	KeyValuePair[0] = '\r';
	KeyValuePair[1] = '\n';

	strUtil::Strncpy(&KeyValuePair[2], pKey, KeyLen);
	KeyValuePair[KeyLen+2] = '=';
	strUtil::Strncpy(&KeyValuePair[KeyLen+3], pValue, ValueLen);

	int IncSize = 2+KeyLen+1+ValueLen;
	
	char* pNewData = new char[m_DataLen+IncSize];
	Assert(pNewData!=NULL);

	//����ԭ�������ݵ�ǰ��
	if (InsertPos > 0)
	{
		memcpy(pNewData, m_pData, InsertPos);
	}
	
	//�����ֵ��
	memcpy(&pNewData[InsertPos], KeyValuePair, IncSize);
	
	//����ԭ���ݵĺ��
	if (m_DataLen-InsertPos > 0)
	{
		memcpy(&pNewData[InsertPos+IncSize], &pNewData[InsertPos], m_DataLen-InsertPos);
	}

	//�ɵ�ԭ����
	SAFE_DELETE_ARRAY(m_pData);
	m_pData = pNewData;
	m_DataLen += IncSize;

	//�ع�����ʼλ������
	_initSectIndex();

	return  true;
	__LEAVE_FUNCTION
	return false;
}

bool IniReader::modifyKeyValuePair(const char* pSect, const char* pKey, const char* pValue)
{
	__ENTER_FUNCTION
	//���Ҷ�
	int iFind = _findSect(pSect);
	Assert(iFind != -1);

	//������һ�У���ֵ���У�
	iFind = _findNextLineBegin(iFind);
	Assert(iFind != -1);

	//���Ҽ�,û�ҵ������
	iFind = _findKey(iFind, pKey);
	Assert(iFind != -1);

	int OldSize = -1;
	int EndPos = -1;
	for (int i=iFind; i<m_DataLen; ++i)
	{
		if (m_pData[i] == '\n')
		{
			OldSize = i-iFind-1;
			EndPos = i-1;
			break;
		}

		//���һ��
		if (i == m_DataLen-1)
		{
			OldSize = m_DataLen-iFind;
			EndPos = m_DataLen;
		}
	}

	char KeyValuePair[INI_KEY_LEN+INI_VALUE_LEN];
	memset(KeyValuePair, 0, sizeof(KeyValuePair));

	int KeyLen = strUtil::Strlen(pKey);
	int ValueLen = strUtil::Strlen(pValue);

	strUtil::Strncpy(&KeyValuePair[0], pKey, KeyLen);
	KeyValuePair[KeyLen] = '=';
	strUtil::Strncpy(&KeyValuePair[KeyLen+1], pValue, ValueLen);

	int NewSize = KeyLen+1+ValueLen;
	int IncSize = NewSize-OldSize;

	char* pNewData = new char[m_DataLen+IncSize];
	Assert(pNewData!=NULL);

	//����ԭ�������ݵ�ǰ��
	if (iFind > 0)
	{
		memcpy(pNewData, m_pData, iFind);
	}

	//�����¼�ֵ��
	memcpy(&pNewData[iFind], KeyValuePair, NewSize);

	//����ԭ���ݵĺ��
	if (m_DataLen-EndPos > 0)
	{
		memcpy(&pNewData[iFind+NewSize], &m_pData[EndPos], m_DataLen-EndPos);
	}

	//�ɵ�ԭ����
	SAFE_DELETE_ARRAY(m_pData);
	m_pData = pNewData;
	m_DataLen += IncSize;

	//�ع�����ʼλ������
	_initSectIndex();

	return true;
	__LEAVE_FUNCTION
	return false;
}

int IniReader::_getIndexByPos(int Pos) const
{
	__ENTER_FUNCTION
	for (int i=0; i<m_SectCount; ++i)
	{
		if (m_aSectIndex[i] == Pos)
		{
			return i;
		}
	}

	return -1;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_findSect(const char* pSect) const
{
	__ENTER_FUNCTION
	Assert(pSect!=NULL);

	char Buf[INI_VALUE_LEN] = {0};
	memset(Buf, 0, sizeof(Buf));

	for (int i=0; i<m_SectCount; ++i)
	{
		int os = m_aSectIndex[i];
		while (m_pData[os] != ']' && os<m_DataLen)
		{
			++os;
		}
		memset(Buf, 0, sizeof(Buf));
		strUtil::Strncpy(Buf, &m_pData[m_aSectIndex[i]], os-m_aSectIndex[i]);
		strUtil::EraseStrBlank(Buf);
		if (strUtil::Strcmp(Buf, pSect) == 0)
		{
			return m_aSectIndex[i];
		}
	}

	//û�ҵ�
	return -1;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_getLine(char* pBuff, int BuffLen, int BeginPos) const
{
	__ENTER_FUNCTION
	Assert(pBuff != 0);
	Assert(BuffLen > 0);
	Assert(BeginPos>=0 && BeginPos<m_DataLen);

	int Idx = 0;
	while (BeginPos<m_DataLen && Idx<BuffLen-1)
	{
		if (m_pData[BeginPos]=='\n' || m_pData[BeginPos]=='\r')
		{
			break;
		}
		else
		{
			pBuff[Idx++] = m_pData[BeginPos++];
		}
	}

	pBuff[Idx] = '\0';
	
	//��������
	while (BeginPos<m_DataLen)
	{
		if (m_pData[BeginPos]=='\r' || m_pData[BeginPos]=='\n')
		{
			++BeginPos;
		}
		else
		{
			return BeginPos;
		}
	}

	return m_DataLen;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_findKey(int Begin, const char* pKey) const
{
	__ENTER_FUNCTION
	Assert(pKey != NULL);

	char LineBuff[INI_LINE_LEN] = {0};
	memset(LineBuff, 0, sizeof(LineBuff));
	char KeyBuff[INI_KEY_LEN] = {0};
	memset(KeyBuff, 0, sizeof(KeyBuff));	

	while (Begin<m_DataLen)
	{
		//��ȡһ��
		memset(LineBuff, 0, sizeof(LineBuff));
		int NextLineBegin = _getLine(LineBuff, INI_LINE_LEN, Begin);
		
		int LineLen = (int)strUtil::Strlen(LineBuff);
		//��������
		if (LineLen == 0)
		{
			Begin = NextLineBegin;
			continue;
		}

		//����ע����
		if (LineBuff[0] == '#')
		{
			Begin = NextLineBegin;
			continue;
		}
		
		memset(KeyBuff, 0, sizeof(KeyBuff));
		for (int i=0; i<LineLen; ++i)
		{
			//���ҵ���һ���ˣ�ʧ��
			if (LineBuff[i] == '[')
			{
				if (i==0 || LineBuff[i-1]=='\n')
				{
					return -1;
				}		 
			}
			if (LineBuff[i] != '=')
			{
				KeyBuff[i] = LineBuff[i];
			}
			else
			{
				//ɾ���ո�
				strUtil::EraseStrBlank(KeyBuff);
				//�ҵ�Key
				if (strUtil::Strcmp(KeyBuff, pKey) == 0)
				{
					//����Key�����е���ʼλ��
					return Begin;
				}
				else
				{
					break;
				}
			}
		}

		Begin = NextLineBegin;
	}

	return -1;
	__LEAVE_FUNCTION
	return -1;
}

int IniReader::_findNextLineBegin(int CurPos) const
{
	__ENTER_FUNCTION
	for (int i=CurPos; i<m_DataLen; ++i)
	{
		if (m_pData[i] == '\n')
		{
			return ++i;
		}
	}
	return -1;
	__LEAVE_FUNCTION
	return -1;
}

void IniReader::addSect(const char* pSect)
{
	__ENTER_FUNCTION

	//���
	if (NULL==pSect)
	{
		AssertEx(0, "sect error!");
	}
	if (strUtil::Strlen(pSect) > INI_SECT_LEN-8)
	{
		AssertEx(0, "sect too long!");
		return;
	}
	//�δ�����ֱ�ӷ���
	if (_findSect(pSect) != -1)
	{
		return;
	}
	
	char SectBuff[INI_SECT_LEN] = {0};
	memset(SectBuff, 0, sizeof(SectBuff));
	SectBuff[0] = '[';
	strUtil::Strncpy(&SectBuff[1], pSect, INI_SECT_LEN-2);
	SectBuff[strUtil::Strlen(pSect)+1] = ']';
	
	int IncLen = (int)strUtil::Strlen(SectBuff);
	char* pNewData = NULL;
	//�ļ���βû�п�������ӿ���
	if (m_pData[m_DataLen-1] != '\n')
	{
		pNewData = new char[m_DataLen+IncLen+2];
		Assert(pNewData != NULL);
		
		//����ԭ��������
		memcpy(pNewData, m_pData, m_DataLen);
		
		//��ӻس�����
		pNewData[m_DataLen] = '\r';
		pNewData[m_DataLen+1] = '\n';

		//����¶�
		memcpy(&pNewData[m_DataLen+2], SectBuff, IncLen);

		//�޸��ļ�����
		m_DataLen += IncLen+2;
	}
	else
	{
		pNewData = new char[m_DataLen+IncLen+2];
		Assert(pNewData != NULL);

		//����ԭ��������
		memcpy(pNewData, m_pData, m_DataLen);

		//����¶�
		memcpy(&pNewData[m_DataLen], SectBuff, IncLen);

		//�޸��ļ�����
		m_DataLen += IncLen;
	}

	//�ɵ�ԭ�����ڴ��
	SAFE_DELETE_ARRAY(m_pData);
	m_pData = pNewData;

	//���¹���������
	_initSectIndex();
	__LEAVE_FUNCTION
}

bool IniReader::_getValue(int Begin, char* pValue, int Len) const
{
	__ENTER_FUNCTION
	Assert(Begin>0 && Begin<m_DataLen);
	Assert(pValue != NULL);

	bool bFlag = false;
	memset(pValue, 0, sizeof(char)*Len);
	int os = 0;

	for (int i=Begin; i<m_DataLen; ++i)
	{
		//�������Ⱥţ��ñ�ǣ����������Ҫ����
		if (m_pData[i] == '=')
		{
			bFlag = true;
			continue;
		}

		//���������з�����
		if (m_pData[i]=='\n' || m_pData[i]=='\r' || m_pData[i]==';')
		{
			break;
		}

		if (bFlag)
		{
			//����
			pValue[os] = m_pData[i];
			++os;
		}
	}

	//ȥ���ո�
	strUtil::EraseStrBlank(pValue);
	return strUtil::Strlen(pValue)>0;

	__LEAVE_FUNCTION
	return false;
}

void IniReader::_initSectIndex()
{
	__ENTER_FUNCTION
	Assert(m_DataLen>0);
	
	//�������Ŀ
	m_SectCount = 0;
	for (int i=0; i<m_DataLen; ++i)
	{
		if (m_pData[i] == '[')
		{
			if (i==0 || m_pData[i-1]=='\n')
			{
				++m_SectCount;
			}
		}
	}

	if (m_SectCount > 0)
	{
		//������ɵ�
		if (m_aSectIndex != NULL)
		{
			SAFE_DELETE_ARRAY(m_aSectIndex);
		}

		//������������ռ�
		m_aSectIndex = new int[m_SectCount];
		int os = 0;
		//��ʼ��
		for (int i=0; i<m_DataLen; ++i)
		{
			if (m_pData[i] == '[')
			{
				if (i==0 || m_pData[i-1]=='\n')
				{
					m_aSectIndex[os] = i+1;
					++os;
				}
			}
		}
	}
	__LEAVE_FUNCTION
}

} //namespace FileReader
