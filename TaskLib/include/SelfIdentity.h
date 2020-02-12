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

    bool ShouldOutputDebugInformation() const;

    bool IsDispatcherVerbose() const;

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

    // TODO : change all this to a property map, like Client.
    std::wstring name;
    std::wstring reportFile;
    std::wstring reportFolder;
    EmailAccountData emailData;
    bool isDispatcherVerbose;
    bool shouldOutputDispatcherDebugInformation;
};

#endif // AGENT_H
