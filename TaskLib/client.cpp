#include "client.h"

#include "abstractjob.h"

using namespace std;

Client::Client(const std::string &_name)
    : name(_name)
{
}

Client::Client(const Client &other)
    : name(other.name), propertyMap(other.propertyMap)
{
   SetJobList(other.jobList);
}

Client::~Client()
{
    propertyMap.clear();
}

Client *Client::Clone() const
{
    return new Client(*this);
}

string Client::GetName() const
{
	return name;
}

void Client::SetName(const string& _name)
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

void Client::GetJobList(std::list<AbstractJob*>& _jobList) const
{
   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
      _jobList.push_back((*it)->Clone());
}

void Client::SetJobList(const std::list<AbstractJob*>& _jobList)
{
   FreeJobList();

   list<AbstractJob*>::const_iterator it = _jobList.begin();
   for (; it != _jobList.end(); ++it)
      jobList.push_back((*it)->Clone());
}

void Client::FreeJobList()
{
   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
      delete (*it);
   jobList.clear();
}
