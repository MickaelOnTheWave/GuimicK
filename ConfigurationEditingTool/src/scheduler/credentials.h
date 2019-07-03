#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <string>

class Credentials
{
public:

    std::string GetDomainUsername() const
    {
        if (username != "")
            return domain + "\\" + username;
        else
            return "";
    }

    std::string username;
    std::string password;
    std::string domain;
};

#endif // CREDENTIALS_H
