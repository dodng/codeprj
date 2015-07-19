#include "varSystem.h"
#include "strUtil.h"
#include "assert.h"

WX_IMPLEMENT_SINGLEON(CVariableSystem);

//������������ֵ
void CVariableSystem::setIntValue(const string& szKey, int iValue)
{
	__ENTER_FUNCTION

	char temp[10]={0};
	strUtil::Int32toStr(iValue,temp,10);
	string szValue = temp;
	_setVariable(szKey, szValue);

	__LEAVE_FUNCTION
}

void CVariableSystem::setIntValue(const char* szKey, int iValue)
{
	string key = szKey;
	setIntValue(key, iValue);
}

//�õ���������ֵ
int CVariableSystem::getIntValue(const string& szKey)
{
	return strUtil::Strtoint(_getVariable(szKey).c_str());
}

int CVariableSystem::getIntValue(const char* szKey)
{
	string key = szKey;
	return getIntValue(key);
}

//���ø�������ֵ
void CVariableSystem::setFloatValue(const string& szKey, float fValue)
{
	__ENTER_FUNCTION

	char temp[10]={0};
	strUtil::FloattoStr(fValue,temp);
	string szValue = temp;
	_setVariable(szKey, szValue);

	__LEAVE_FUNCTION
}

void CVariableSystem::setFloatValue( const char* szKey, float fValue )
{
	string key = szKey;
	setFloatValue(key, fValue);
}

//�õ���������ֵ
float CVariableSystem::getFloatValue(const string& szKey)
{
	return Strtof(_getVariable(szKey).c_str());
}

float CVariableSystem::getFloatValue( const char* szKey )
{
	string key = szKey;
	return Strtof(_getVariable(key).c_str());

}

//�����ַ�������ֵ
void CVariableSystem::setStringValue(const string& szKey, const string& szValue)
{
	_setVariable(szKey, szValue);
}

//�õ��ַ�������ֵ
string CVariableSystem::getStringValue(const string& szKey)
{
	return _getVariable(szKey);
}

string CVariableSystem::getStringValue(const char* szKey)
{
	string key = szKey;
	return _getVariable(key);
}

//ɾ������
void CVariableSystem::delVariable(const string& szKey)
{
	std::map<string, string>::iterator it;
	it = m_VariablesMap.find(szKey);

	if(it != m_VariablesMap.end())
		m_VariablesMap.erase(it);
}

//���ñ���
void CVariableSystem::_setVariable(const string& szKey, const string& szValue)
{
	__ENTER_FUNCTION
	m_VariablesMap[szKey] = szValue;
	__LEAVE_FUNCTION
}

//�õ�����
string CVariableSystem::_getVariable(const string& szKey)
{
	std::map<string, string>::iterator it;
	it = m_VariablesMap.find(szKey);
	if(it != m_VariablesMap.end())
		return it->second;
	return "";
}
