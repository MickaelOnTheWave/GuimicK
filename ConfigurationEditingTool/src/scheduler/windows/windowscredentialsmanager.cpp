#include "windowscredentialsmanager.h"

#include <comdef.h>
#include <wincred.h>
#include "stringtools.h"

namespace  {
    const char* needsAdminToRunOutOfSession = "In order to run the task list even when you are not "
                                              "logged, we need your credentials.\n"
                                              "If you choose to not provide them, the tasks will only run "
                                              "when you are logged.";
    const char* wrongCredentials = "Invalid credentials";

    CREDUI_INFO CreateCredentialsData(const bool firstTime)
    {
        const char* message = (firstTime) ? needsAdminToRunOutOfSession : wrongCredentials;
        CREDUI_INFO dataStructure;
        dataStructure.cbSize = sizeof(CREDUI_INFO);
        dataStructure.hwndParent = nullptr;
        dataStructure.pszMessageText = TEXT(message);
        dataStructure.pszCaptionText = TEXT("User credentials needed");
        dataStructure.hbmBanner = nullptr;
        return dataStructure;
    }
}

HRESULT WindowsCredentialsManager::AskUser()
{
    bool credentialsProvided = false;
    bool shouldAskCredentials = true;
    bool firstTimeAsking = true;
    while (shouldAskCredentials)
    {
        const DWORD result = AskUserOnce(firstTimeAsking);
        firstTimeAsking = false;
        credentialsProvided = (result == NO_ERROR);
        if (credentialsProvided)
            shouldAskCredentials = !AreCredentialsValid();
        else
            shouldAskCredentials = false;
    }

    return (credentialsProvided) ? 0 : -1;
}

Credentials WindowsCredentialsManager::GetCredentials() const
{
    return credentials;
}

DWORD WindowsCredentialsManager::AskUserOnce(const bool firstTime)
{
    CREDUI_INFO cui = CreateCredentialsData(firstTime);
    BOOL fSave = FALSE;
    DWORD result;

    PCSTR target = TEXT("");

    DWORD optionFlags =
        CREDUI_FLAGS_GENERIC_CREDENTIALS |
        CREDUI_FLAGS_ALWAYS_SHOW_UI |
        CREDUI_FLAGS_DO_NOT_PERSIST |
        CREDUI_FLAGS_REQUEST_ADMINISTRATOR |
        CREDUI_FLAGS_VALIDATE_USERNAME |
        CREDUI_FLAGS_INCORRECT_PASSWORD |
        CREDUI_FLAGS_EXPECT_CONFIRMATION;

    char domainAndUser[CREDUI_MAX_USERNAME_LENGTH] = "";
    char passwordBuffer[CREDUI_MAX_PASSWORD_LENGTH] = "";
    result = CredUIPromptForCredentials(
        &cui,                             //  CREDUI_INFO structure
        target,                         //  Target for credentials
        nullptr,                             //  Reserved
        0,                                //  Reason
        domainAndUser,                          //  User name
        CREDUI_MAX_USERNAME_LENGTH,       //  Max number for user name
        passwordBuffer,                           //  Password
        CREDUI_MAX_PASSWORD_LENGTH,       //  Max number for password
        &fSave,                           //  State of save check box
        optionFlags
    );

    if (result == NO_ERROR)
        FillCredentials(domainAndUser, passwordBuffer);
    return result;
}

void WindowsCredentialsManager::FillCredentials(const char* domainAndUser,
                                                const char* password)
{
    const std::string strComposedName(domainAndUser);

    std::vector<std::string> tokens;
    StringTools::Tokenize(strComposedName, '\\', tokens);

    credentials.username = tokens.back();
    credentials.domain = tokens.front();
    credentials.password = password;
}

bool WindowsCredentialsManager::AreCredentialsValid()
{
    HANDLE token;
    const BOOL result = LogonUserA(
                credentials.username.c_str(),
                credentials.domain.c_str(),
                credentials.password.c_str(),
                 LOGON32_LOGON_INTERACTIVE,
                 LOGON32_PROVIDER_DEFAULT,
                 &token);
    return (result == TRUE);
}
