#include "client.h"

Client::Client(const std::string &_name)
    : name(_name)
{
}

Client::Client(const Client &other)
    : name(other.name), propertyMap(other.propertyMap)
{
}

Client::~Client()
{
    propertyMap.clear();
}

Client *Client::Clone() const
{
    return new Client(*this);
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
