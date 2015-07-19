#include "PropertySet.h"

// -----------------------------------------------------------------------------------------------------
PropertySet::PropertySet(void)
{
}

// -----------------------------------------------------------------------------------------------------
PropertySet::~PropertySet(void)
{
}

// -----------------------------------------------------------------------------------------------------
void PropertySet::registerProperty(std::string const& name, int* value)
{
	Property* property = new Property(name, value);
	m_properties.insert(PropertyContainerValueType(name, property));
}

// -----------------------------------------------------------------------------------------------------
void PropertySet::registerProperty(std::string const& name, float* value)
{
	Property* property = new Property(name, value);
	m_properties.insert(PropertyContainerValueType(name, property));
}

// -----------------------------------------------------------------------------------------------------
void PropertySet::registerProperty(std::string const& name, std::string* value)
{
	Property* property = new Property(name, value);
	m_properties.insert(PropertyContainerValueType(name, property));
}

// -----------------------------------------------------------------------------------------------------
void PropertySet::registerProperty(std::string const& name, bool* value)
{
	Property* property = new Property(name, value);
	m_properties.insert(PropertyContainerValueType(name, property));
}

// -----------------------------------------------------------------------------------------------------
Property* PropertySet::lookUp(std::string const& name)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return NULL;
	}
	return itor->second;
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::setValue(std::string const& name, std::string& value)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return false;
	}
	(itor->second)->setUnknownValue(value);
	return true;
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::set(std::string const& name, int value)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return false;
	}
	(itor->second)->set(value);
	return true;
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::set(std::string const& name, float value)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return false;
	}
	(itor->second)->set(value);
	return true;
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::set(std::string const& name, std::string value)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return false;
	}
	(itor->second)->set(value);
	return true;
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::set(std::string const& name, bool value)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return false;
	}
	(itor->second)->set(value);
	return true;
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::set(std::string const& name, char* value)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		return false;
	}
	return (itor->second)->set(std::string(value));
}

// -----------------------------------------------------------------------------------------------------
int PropertySet::getInt(std::string const& name)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		Assert(!"can not find the int property.");
	}
	return (itor->second)->getInt();
}

// -----------------------------------------------------------------------------------------------------
float PropertySet::getFloat(std::string const& name)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		Assert(!"can not find the float property.");
	}
	return (itor->second)->getFloat();
}

// -----------------------------------------------------------------------------------------------------
std::string PropertySet::getString(std::string const& name)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		Assert(!"can not find the string property.");
	}
	return (itor->second)->getString();
}

// -----------------------------------------------------------------------------------------------------
bool PropertySet::getBool(std::string const& name)
{
	PropertyItor itor = m_properties.find(name);
	if (itor == m_properties.end())
	{
		Assert(!"can not find the bool property.");
	}
	return (itor->second)->getBool();
}

// -----------------------------------------------------------------------------------------------------
PropertyContainer PropertySet::getPropertyContainer() const
{
	return m_properties;
}

// -----------------------------------------------------------------------------------------------------
// END OF FILE