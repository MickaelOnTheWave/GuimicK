#ifndef AGENT_H
#define AGENT_H

#include <fstream>
#include <string>
#include <vector>

#include "configurationobject.h"
#include "emailaccountdata.h"

class Agent
{
public:
   static const int BOTMODE_NO = 0;
   static const int BOTMODE_TELEGRAM = 1;

    Agent();
    Agent(const Agent& other);
    Agent(ConfigurationObject *confObject, std::vector<std::wstring> &errorMessages);

    void SaveToOpenedFile(std::wofstream& fileStream);

    std::wstring GetName() const;
    void SetName(const std::wstring& value);

    std::wstring GetReportFile() const;
    void SetReportFile(const std::wstring& value);

    std::wstring GetReportFolder() const;
    void SetReportFolder(const std::wstring& value);

    EmailAccountData GetEmailData() const;
    void SetEmailData(const EmailAccountData& value);
    bool HasValidEmailData() const;

    int GetOutputDebugInformation() const;

    bool IsDispatcherVerbose() const;

    int GetBotMode() const;

    std::wstring GetBotToken() const;

    std::string GetAuthorizedUserToken() const;

private:
    void LoadSubobjects(
        ConfigurationObject *confObject,
        std::vector<std::wstring> &errorMessages
    );

    void LoadProperties(
        ConfigurationObject *confObject,
        std::vector<std::wstring> &errorMessages
    );

    void LoadProperty(
        const std::pair<std::wstring, std::wstring>& property,
        std::vector<std::wstring> &errorMessages
    );

    void SaveToFile(
       const EmailAccountData& emailData,
       std::wofstream& fileStream
    );

    static int ParseBotMode(const std::wstring& desc);
    static std::wstring GetBotModeName(const int value);

    // TODO : change all this to a property map, like Client.
    std::wstring name;
    std::wstring reportFile;
    std::wstring reportFolder;
    EmailAccountData emailData;
    bool isDispatcherVerbose;
    int outputDispatcherDebugInformation;
    int botMode;
    std::wstring botToken;
    std::wstring authorizedUserToken;
};

#endif // AGENT_H
