#ifndef SELFIDENTITY_H
#define SELFIDENTITY_H

#include <string>
#include <vector>

#include "configurationobject.h"

class SelfIdentity
{
public:
    SelfIdentity();
    SelfIdentity(const SelfIdentity& other);
    SelfIdentity(ConfigurationObject *confObject, std::vector<std::string> &errorMessages);

    bool HasValidEmailData() const;

	std::string name;
	std::string email;
	std::string emailPassword;
	std::string emailSmtpServer;
	bool emailUseSsl;
	int emailSmtpPort;

private:
    std::string BuildUnhandledPropertyMessage(const std::string& property) const;
};

#endif // SELFIDENTITY_H
