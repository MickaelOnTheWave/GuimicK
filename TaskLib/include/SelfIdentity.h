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
    Agent(ConfigurationObject *confObject, std::vector<std::string> &errorMessages);

    void SaveToOpenedFile(std::ofstream& fileStream);

    std::string GetName() const;
    void SetName(const std::string& value);

    std::string GetReportFile() const;
    void SetReportFile(const std::string& value);

    std::string GetReportFolder() const;
    void SetReportFolder(const std::string& value);

    EmailData GetEmailData() const;
    void SetEmailData(const EmailData& value);
    bool HasValidEmailData() const;

private:
    std::string BuildUnhandledPropertyMessage(const std::string& property) const;

    // TODO : change all this to a property map, like Client.
    std::string name;
    std::string reportFile;
    std::string reportFolder;
    EmailData emailData;
};

#endif // AGENT_H
