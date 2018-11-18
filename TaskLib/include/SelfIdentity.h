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

    std::string GetReportFile() const;
    std::string GetReportFolder() const;

    EmailData GetEmailData() const;
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
