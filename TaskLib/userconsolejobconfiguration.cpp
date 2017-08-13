#include "userconsolejobconfiguration.h"
#include "userconsolejob.h"

#include <sstream>

using namespace std;

UserConsoleJobConfiguration::UserConsoleJobConfiguration()
    : AbstractJobDefaultConfiguration("Console")
{
}

UserConsoleJobConfiguration::UserConsoleJobConfiguration(const string &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

AbstractJob *UserConsoleJobConfiguration::CreateJob()
{
    return new UserConsoleJob();
}

void UserConsoleJobConfiguration::ConfigureJob(AbstractJob *job, ConfigurationObject *confObject, std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const string title =          confObject->GetFirstProperty("title", "param0");
    const string command =        confObject->GetFirstProperty("command", "param1");
    const string rawReturnCode =  confObject->GetFirstProperty("returnCode","param2");
    const string expectedOutput = confObject->GetProperty("expectedOutput");
    const string outputFile     = confObject->GetProperty("outputFileName");
    const string parserCommand  = confObject->GetProperty("parserCommand");
    const string parserUsingBuffer = confObject->GetProperty("parserUsesBuffer");

    UserConsoleJob* castJob = static_cast<UserConsoleJob*>(job);
    castJob->SetTitle(title);
    castJob->Initialize(command);

    if (rawReturnCode != "")
    {
        int returnCode = 0;
        stringstream ss(rawReturnCode);
        ss >> returnCode;
        castJob->SetExpectedReturnCode(returnCode);
    }

    if (expectedOutput != "")
        castJob->SetExpectedOutput(expectedOutput);

    if (outputFile != "")
        castJob->SetOutputTofile(outputFile);
    else
        castJob->SetAttachOutput(true);

    if (parserCommand != "")
        castJob->SetMiniDescriptionParserCommand(parserCommand);

    if (parserUsingBuffer == "true")
        castJob->SetParsingUsingBuffer(true);
}

void UserConsoleJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    properties.push_back("title");
    properties.push_back("command");
    properties.push_back("returnCode");
    properties.push_back("expectedOutput");
    properties.push_back("outputFileName");
    properties.push_back("parserCommand");
    properties.push_back("parserUsesBuffer");
}
