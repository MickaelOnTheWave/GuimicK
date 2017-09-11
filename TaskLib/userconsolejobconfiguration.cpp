#include "userconsolejobconfiguration.h"
#include "userconsolejob.h"

#include <sstream>

using namespace std;

const std::string UserConsoleJobConfiguration::TitleProperty = "title";
const std::string UserConsoleJobConfiguration::CommandProperty = "command";
const std::string UserConsoleJobConfiguration::ParameterProperty = "params";
const std::string UserConsoleJobConfiguration::ReturnCodeProperty = "returnCode";
const std::string UserConsoleJobConfiguration::ExpectedOutputProperty = "expectedOutput";
const std::string UserConsoleJobConfiguration::OutputFilenameProperty = "outputFileName";
const std::string UserConsoleJobConfiguration::ParserCommandProperty = "parserCommand";
const std::string UserConsoleJobConfiguration::ParserUsesBufferProperty = "parserUsesBuffer";

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

    const string title =          confObject->GetFirstProperty(TitleProperty, "param0");
    const string command =        confObject->GetFirstProperty(CommandProperty, "param1");
    const string parameters =     confObject->GetFirstProperty(ParameterProperty, "param1");
    const string rawReturnCode =  confObject->GetFirstProperty(ReturnCodeProperty,"param2");
    const string expectedOutput = confObject->GetProperty(ExpectedOutputProperty);
    const string outputFile     = confObject->GetProperty(OutputFilenameProperty);
    const string parserCommand  = confObject->GetProperty(ParserCommandProperty);
    const string parserUsingBuffer = confObject->GetProperty(ParserUsesBufferProperty);

    UserConsoleJob* castJob = static_cast<UserConsoleJob*>(job);
    castJob->SetTitle(title);
    castJob->Initialize(command);
    castJob->SetCommandParameters(parameters);

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
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);

    properties.push_back(TitleProperty);
    properties.push_back(CommandProperty);
    properties.push_back(ParameterProperty);
    properties.push_back(ReturnCodeProperty);
    properties.push_back(ExpectedOutputProperty);
    properties.push_back(OutputFilenameProperty);
    properties.push_back(ParserCommandProperty);
    properties.push_back(ParserUsesBufferProperty);
}
