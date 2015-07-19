#include "property.h"
#include "assert.h"
//using std::cout;
//using std::endl;

// -----------------------------------------------------------------------------------------------------
Property::Property()
{
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name)
{
	this->m_name = name;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, bool *value)
{
	this->m_name = name;
	this->m_type = BOOL;
	this->m_data.m_bool = value;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, float *value)
{
	this->m_name = name;
	this->m_type = FLOAT;
	this->m_data.m_float = value;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, int *value)
{
	this->m_name = name;
	this->m_type = INT;
	this->m_data.m_int = value;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, std::string *value)
{
	this->m_name = name;
	this->m_type = STRING;
	this->m_data.m_string = value;
}

// -----------------------------------------------------------------------------------------------------
bool Property::setUnknownValue(std::string const& value)
{
	bool return_value = true;
	switch (m_type)
	{
	case INT:
		*m_data.m_int = atoi(value.c_str());
		break;
	case FLOAT:
		*m_data.m_float = static_cast<float>(atof(value.c_str()));
		break;
	case STRING:
		*m_data.m_string = value;
		break;
	case BOOL:
		*m_data.m_bool = (value.compare("true") == 0) || (value.compare("TRUE") == 0 || (value.compare("1") == 0));
		break;
	default:
		return_value = false;
		break;
	}

	return return_value;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(bool value)
{
	if (m_type == BOOL)
	{
		*m_data.m_bool = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(float value)
{
	if (m_type == FLOAT)
	{
		*m_data.m_float = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(int value)
{
	if (m_type == INT)
	{
		*m_data.m_int = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(std::string value)
{
	if (m_type == STRING)
	{
		*m_data.m_string = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
void Property::setName(const std::string &name)
{
	m_name = name;
}

// -----------------------------------------------------------------------------------------------------
std::string Property::getName() const
{
	return m_name;
}

// -----------------------------------------------------------------------------------------------------
int Property::getInt()
{
	Assert(m_type == INT);
	return *m_data.m_int;
}

// -----------------------------------------------------------------------------------------------------
float Property::getFloat()
{
	Assert(m_type == FLOAT);
	return *m_data.m_float;
}

// -----------------------------------------------------------------------------------------------------
std::string Property::getString()
{
	Assert(m_type == STRING);
	return *m_data.m_string;
}

// -----------------------------------------------------------------------------------------------------
bool Property::getBool()
{
	Assert(m_type == BOOL);
	return *m_data.m_bool;
}

// -----------------------------------------------------------------------------------------------------
void Property::erase()
{
	m_type = EMPTY;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(int* value)
{
	m_type = INT;
	m_data.m_int = value;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(float* value)
{
	m_type = FLOAT;
	m_data.m_float = value;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(std::string* value)
{
	m_type = STRING;
	m_data.m_string = value;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(bool* value)
{
	m_type = BOOL;
	m_data.m_bool = value;
}
// -----------------------------------------------------------------------------------------------------
// END OF FILE
