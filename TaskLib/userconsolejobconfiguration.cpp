#include "userconsolejobconfiguration.h"

#include <sstream>

using namespace std;

const std::string UserConsoleJobConfiguration::CommandProperty = "command";
const std::string UserConsoleJobConfiguration::ParameterProperty = "params";
const std::string UserConsoleJobConfiguration::ReturnCodeProperty = "returnCode";
const std::string UserConsoleJobConfiguration::ExpectedOutputProperty = "expectedOutput";
const std::string UserConsoleJobConfiguration::OutputFilenameProperty = "outputFileName";
const std::string UserConsoleJobConfiguration::ParserCommandProperty = "parserCommand";
const std::string UserConsoleJobConfiguration::ParserUsesBufferProperty = "parserUsesBuffer";
const std::string UserConsoleJobConfiguration::UserAttachmentObject = "AttachFile";

UserConsoleJobConfiguration::UserConsoleJobConfiguration()
    : AbstractJobDefaultConfiguration("Console")
{
}

UserConsoleJobConfiguration::UserConsoleJobConfiguration(const string &tag)
    : AbstractJobDefaultConfiguration(tag)
{
}

bool UserConsoleJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<UserConsoleJob*>(job) != NULL);
}

ConfigurationObject* UserConsoleJobConfiguration::CreateConfigurationObject(AbstractJob* job)
{
   AbstractConsoleJob* consoleJob = dynamic_cast<AbstractConsoleJob*>(job);
   ConfigurationObject* confObject = AbstractJobDefaultConfiguration::CreateConfigurationObject(job);
   confObject->SetProperty(CommandProperty, consoleJob->GetCommand());
   confObject->SetProperty(ParameterProperty, consoleJob->GetCommandParameters());

   UserConsoleJob* userJob = dynamic_cast<UserConsoleJob*>(job);
   if (userJob)
      ConfigureObjectFromJob(confObject, userJob);

   return confObject;
}

AbstractJob *UserConsoleJobConfiguration::CreateJob()
{
    return new UserConsoleJob();
}

void UserConsoleJobConfiguration::ConfigureJob(AbstractJob *job, ConfigurationObject *confObject, std::vector<string> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const string command =        confObject->GetFirstProperty(CommandProperty, "param0");
    const string parameters =     confObject->GetFirstProperty(ParameterProperty, "param1");
    const string rawReturnCode =  confObject->GetFirstProperty(ReturnCodeProperty,"param2");
    const string expectedOutput = confObject->GetProperty(ExpectedOutputProperty);
    const string outputFile     = confObject->GetProperty(OutputFilenameProperty);
    const string parserCommand  = confObject->GetProperty(ParserCommandProperty);
    const string parserUsingBuffer = confObject->GetProperty(ParserUsesBufferProperty);

    UserConsoleJob* castJob = static_cast<UserConsoleJob*>(job);
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

    list<ConfigurationObject*>::iterator it = confObject->objectList.begin();
    for (; it != confObject->objectList.end(); ++it)
    {
       if ((*it)->GetName() == UserAttachmentObject)
          castJob->AddUserAttachment((*it)->GetProperty("param0"));
    }
}

void UserConsoleJobConfiguration::FillKnownProperties(std::vector<string> &properties)
{
    AbstractJobDefaultConfiguration::FillKnownProperties(properties);

    properties.push_back(CommandProperty);
    properties.push_back(ParameterProperty);
    properties.push_back(ReturnCodeProperty);
    properties.push_back(ExpectedOutputProperty);
    properties.push_back(OutputFilenameProperty);
    properties.push_back(ParserCommandProperty);
    properties.push_back(ParserUsesBufferProperty);
}

void UserConsoleJobConfiguration::FillKnownSubObjects(std::vector<string>& objects)
{
   AbstractJobDefaultConfiguration::FillKnownSubObjects(objects);
   objects.push_back(UserAttachmentObject);
}

void UserConsoleJobConfiguration::ConfigureObjectFromJob(
      ConfigurationObject* confObject,
      UserConsoleJob* job)
{
   if (job->GetExpectedOutput() != "")
      confObject->SetProperty(ExpectedOutputProperty, job->GetExpectedOutput());
   else
      confObject->SetProperty(ReturnCodeProperty, job->GetExpectedReturnCode());

   if (job->GetOutputFile() != "")
      confObject->SetProperty(OutputFilenameProperty, job->GetOutputFile());

   if (job->GetMiniDescriptionParserCommand() != "")
   {
      confObject->SetProperty(ParserCommandProperty, job->GetMiniDescriptionParserCommand());
      confObject->SetProperty(ParserUsesBufferProperty, job->IsParsingUsingBuffer());
   }

   vector<string> attachments;
   job->GetUserAttachments(attachments);
   vector<string>::const_iterator it = attachments.begin();
   for (; it != attachments.end(); ++it)
   {
      ConfigurationObject* attachmentObj = new ConfigurationObject(UserAttachmentObject);
      attachmentObj->SetProperty("param0", *it);
      confObject->AddObject(attachmentObj);
   }
}
