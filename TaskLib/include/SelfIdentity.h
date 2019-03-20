#ifndef AGENT_H
#define AGENT_H

#include <fstream>
#include <string>
#include <vector>

#include "configurationobject.h"
#include "emaildata.h"

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

    EmailData GetEmailData() const;
    void SetEmailData(const EmailData& value);
    bool HasValidEmailData() const;

private:
    std::wstring BuildUnhandledPropertyMessage(const std::wstring& property) const;

    // TODO : change all this to a property map, like Client.
    std::wstring name;
    std::wstring reportFile;
    std::wstring reportFolder;
    EmailData emailData;
};

#endif // AGENT_H
