#include "client.h"

Client::Client(const std::string &_name)
    : name(_name)
{
}

std::string Client::GetName() const
{
	return name;
}

void Client::SetName(const std::string _name)
{
	name = _name;
}

void Client::AddProperty(const std::string &name, const std::string &value)
{
    propertyMap[name] = value;
}

bool Client::HasProperty(const std::string &name)
{
    return (propertyMap.count(name) > 0);
}

std::string Client::GetProperty(const std::string &name)
{
    return propertyMap[name];
}
