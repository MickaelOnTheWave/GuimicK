#ifndef WINDOWSCREDENTIALSMANAGER_H
#define WINDOWSCREDENTIALSMANAGER_H

#include <comutil.h>
#include "credentials.h"

class WindowsCredentialsManager
{
public:
    WindowsCredentialsManager() = default;
    virtual ~WindowsCredentialsManager() = default;

    HRESULT AskUser();

    Credentials GetCredentials() const;

private:
    DWORD AskUserOnce(const bool firstTime);

    void FillCredentials(const char* domainAndUser,
                         const char *password);

    bool AreCredentialsValid();

    Credentials credentials;
};

#endif // WINDOWSCREDENTIALSMANAGER_H
