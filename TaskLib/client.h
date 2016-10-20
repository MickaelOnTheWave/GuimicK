#ifndef CLIENT_H
#define CLIENT_H

#include <map>
#include <string>

class Client
{
public:

	Client(const std::string& _name = "");

	 std::string GetName() const;
	 void SetName(const std::string _name);

    void AddProperty(const std::string& name,
                     const std::string& value);

    bool HasProperty(const std::string& name);

    std::string GetProperty(const std::string& name);

protected:
    std::string name;
    std::map<std::string, std::string> propertyMap;
};

#endif // CLIENT_H
