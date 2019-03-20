#include "client.h"

#include "abstractjob.h"

using namespace std;

Client::Client(const wstring& _name)
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

wstring Client::GetName() const
{
	return name;
}

void Client::SetName(const wstring& _name)
{
	name = _name;
}

void Client::AddProperty(const std::string &propertyName, const std::wstring &value)
{
    propertyMap[propertyName] = value;
}

bool Client::HasProperty(const std::string &propertyName)
{
    return (propertyMap.count(propertyName) > 0);
}

std::wstring Client::GetProperty(const std::string &propertyName)
{
   return propertyMap[propertyName];
}

Client::PropertyMap::const_iterator Client::PropertyBegin() const
{
   return propertyMap.begin();
}

Client::PropertyMap::const_iterator Client::PropertyEnd() const
{
   return propertyMap.end();
}

void Client::GetJobList(std::list<AbstractJob*>& _jobList) const
{
   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
      _jobList.push_back((*it)->Clone());
}

void Client::SetJobList(const std::list<AbstractJob*>& _jobList)
{
   ClearJobList();
   list<AbstractJob*>::const_iterator it = _jobList.begin();
   for (; it != _jobList.end(); ++it)
      jobList.push_back((*it)->Clone());
}

void Client::ClearJobList()
{
   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
      delete (*it);
   jobList.clear();
}

void Client::AddJob(AbstractJob* job)
{
   jobList.push_back(job);
}
