#ifndef CLIENT_H
#define CLIENT_H

#include <list>
#include <map>
#include <string>

class AbstractJob;

class Client
{
public:
    Client(const std::string& _name = "");
    Client(const Client& other);
    ~Client();

    Client* Clone() const;

	 std::string GetName() const;
    void SetName(const std::string& _name);

    void AddProperty(const std::string& name,
                     const std::string& value);

    bool HasProperty(const std::string& name);
    std::string GetProperty(const std::string& name);

    void GetJobList(std::list<AbstractJob*>& _jobList) const;
    void SetJobList(const std::list<AbstractJob*>& _jobList);

protected:
    void FreeJobList();

    std::string name;
    std::map<std::string, std::string> propertyMap;
    std::list<AbstractJob*> jobList;
};

#endif // CLIENT_H
