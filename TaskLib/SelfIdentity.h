#ifndef SELFIDENTITY_H
#define SELFIDENTITY_H

#include <string>

class SelfIdentity
{
public:

	std::string name;
	std::string email;
	std::string emailPassword;
	std::string emailSmtpServer;
	bool emailUseSsl;
	int emailSmtpPort;
};

#endif // SELFIDENTITY_H
