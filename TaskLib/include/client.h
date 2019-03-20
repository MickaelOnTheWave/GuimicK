#ifndef CLIENT_H
#define CLIENT_H

#include <list>
#include <map>
#include <string>

class AbstractJob;

class Client
{
public:
    typedef std::map<std::wstring, std::wstring> PropertyMap;

    Client(const std::wstring& _name = L"");
    Client(const Client& other);
    ~Client();

    Client* Clone() const;

    std::wstring GetName() const;
    void SetName(const std::wstring& _name);

    void AddProperty(const std::wstring& name,
                     const std::wstring& value);

    bool HasProperty(const std::wstring& name);
    std::wstring GetProperty(const std::wstring& name);

    PropertyMap::const_iterator PropertyBegin() const;
    PropertyMap::const_iterator PropertyEnd() const;


    void GetJobList(std::list<AbstractJob*>& _jobList) const;
    void SetJobList(const std::list<AbstractJob*>& _jobList);
    void ClearJobList();

    void AddJob(AbstractJob* job);


protected:
    std::wstring name;
    PropertyMap propertyMap;
    std::list<AbstractJob*> jobList;
};

#endif // CLIENT_H
