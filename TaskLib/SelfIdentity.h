#ifndef SELFIDENTITY_H
#define SELFIDENTITY_H

#include <string>

class SelfIdentity
{
public:

    bool HasValidEmailData() const
    {
        if (email == "")
            return false;
        else if (emailPassword == "")
            return false;
        else if (emailSmtpPort == -1)
            return false;
        else if (emailSmtpServer == "")
            return false;
        else
            return true;
    }

	std::string name;
	std::string email;
	std::string emailPassword;
	std::string emailSmtpServer;
	bool emailUseSsl;
	int emailSmtpPort;
};

#endif // SELFIDENTITY_H
