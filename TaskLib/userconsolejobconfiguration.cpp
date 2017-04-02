#include "userconsolejobconfiguration.h"
#include "userconsolejob.h"

#include <sstream>

using namespace std;

UserConsoleJobConfiguration::UserConsoleJobConfiguration()
    : AbstractJobConfiguration("Console")
{
}

UserConsoleJobConfiguration::UserConsoleJobConfiguration(const string &tag)
    : AbstractJobConfiguration(tag)
{
}

AbstractJob *UserConsoleJobConfiguration::CreateConfiguredJobAfterCheck(
                                            ConfigurationObject *confObject,
                                            std::vector<std::string> &)
{
    string title =          confObject->GetFirstProperty("title", "param0");
    string command =        confObject->GetFirstProperty("command", "param1");
    string rawReturnCode =  confObject->GetFirstProperty("returnCode","param2");
    string expectedOutput = confObject->propertyList["expectedOutput"];
    string outputFile     = confObject->propertyList["outputFileName"];
    string parserCommand  = confObject->propertyList["parserCommand"];

    UserConsoleJob* job = new UserConsoleJob();
    job->SetTitle(title);
    job->Initialize(command);

    if (confObject->propertyList["showDebugInformation"] != "")
        job->SetOutputDebugInformation(true);

    if (rawReturnCode != "")
    {
        int returnCode = 0;
        stringstream ss(rawReturnCode);
        ss >> returnCode;
        job->SetExpectedReturnCode(returnCode);
    }

    if (expectedOutput != "")
        job->SetExpectedOutput(expectedOutput);

    if (outputFile != "")
        job->SetOutputTofile(outputFile);
    else
        job->SetAttachOutput(true);

    if (parserCommand != "")
        job->SetMiniDescriptionParserCommand(parserCommand);

    return job;
}

void UserConsoleJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    properties.push_back("title");
    properties.push_back("command");
    properties.push_back("returnCode");
    properties.push_back("expectedOutput");
    properties.push_back("outputFileName");
    properties.push_back("parserCommand");
    properties.push_back("showDebugInformation");
}
