#include "userconsolejobconfiguration.h"

#include <sstream>

using namespace std;

const std::wstring UserConsoleJobConfiguration::CommandProperty = L"command";
const std::wstring UserConsoleJobConfiguration::ParameterProperty = L"params";
const std::wstring UserConsoleJobConfiguration::ReturnCodeProperty = L"returnCode";
const std::wstring UserConsoleJobConfiguration::ExpectedOutputProperty = L"expectedOutput";
const std::wstring UserConsoleJobConfiguration::OutputFilenameProperty = L"outputFileName";
const std::wstring UserConsoleJobConfiguration::ParserCommandProperty = L"parserCommand";
const std::wstring UserConsoleJobConfiguration::ParserUsesBufferProperty = L"parserUsesBuffer";
const std::wstring UserConsoleJobConfiguration::UserAttachmentObject = L"AttachFile";

UserConsoleJobConfiguration::UserConsoleJobConfiguration()
    : AbstractJobDefaultConfiguration(L"Console")
{
}

UserConsoleJobConfiguration::UserConsoleJobConfiguration(const wstring &tag)
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

void UserConsoleJobConfiguration::ConfigureJob(AbstractJob *job, ConfigurationObject *confObject, std::vector<wstring> &errorMessages)
{
    AbstractJobDefaultConfiguration::ConfigureJob(job, confObject, errorMessages);

    const wstring command =        confObject->GetFirstProperty(CommandProperty, L"param0");
    const wstring parameters =     confObject->GetFirstProperty(ParameterProperty, L"param1");
    const wstring rawReturnCode =  confObject->GetFirstProperty(ReturnCodeProperty, L"param2");
    const wstring expectedOutput = confObject->GetProperty(ExpectedOutputProperty);
    const wstring outputFile     = confObject->GetProperty(OutputFilenameProperty);
    const wstring parserCommand  = confObject->GetProperty(ParserCommandProperty);
    const wstring parserUsingBuffer = confObject->GetProperty(ParserUsesBufferProperty);

    UserConsoleJob* castJob = static_cast<UserConsoleJob*>(job);
    castJob->Initialize(command);
    castJob->SetCommandParameters(parameters);

    if (rawReturnCode != L"")
    {
        int returnCode = 0;
        wstringstream ss(rawReturnCode);
        ss >> returnCode;
        castJob->SetExpectedReturnCode(returnCode);
    }

    if (expectedOutput != L"")
        castJob->SetExpectedOutput(expectedOutput);

    if (outputFile != L"")
        castJob->SetOutputTofile(outputFile);
    else
        castJob->SetAttachOutput(true);

    if (parserCommand != L"")
        castJob->SetMiniDescriptionParserCommand(parserCommand);

    if (parserUsingBuffer == L"true")
        castJob->SetParsingUsingBuffer(true);

    list<ConfigurationObject*>::iterator it = confObject->BeginObjects();
    for (; it != confObject->EndObjects(); ++it)
    {
       if ((*it)->GetName() == UserAttachmentObject)
          castJob->AddUserAttachment((*it)->GetProperty(L"param0"));
    }
}

void UserConsoleJobConfiguration::FillKnownProperties(std::vector<wstring> &properties)
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

void UserConsoleJobConfiguration::FillKnownSubObjects(std::vector<wstring>& objects)
{
   AbstractJobDefaultConfiguration::FillKnownSubObjects(objects);
   objects.push_back(UserAttachmentObject);
}

void UserConsoleJobConfiguration::ConfigureObjectFromJob(
      ConfigurationObject* confObject,
      UserConsoleJob* job)
{
   if (job->GetExpectedOutput() != L"")
      confObject->SetProperty(ExpectedOutputProperty, job->GetExpectedOutput());
   else
      confObject->SetProperty(ReturnCodeProperty, job->GetExpectedReturnCode());

   if (job->GetOutputFile() != L"")
      confObject->SetProperty(OutputFilenameProperty, job->GetOutputFile());

   if (job->GetMiniDescriptionParserCommand() != L"")
   {
      confObject->SetProperty(ParserCommandProperty, job->GetMiniDescriptionParserCommand());
      confObject->SetProperty(ParserUsesBufferProperty, job->IsParsingUsingBuffer());
   }

   vector<wstring> attachments;
   job->GetUserAttachments(attachments);
   vector<wstring>::const_iterator it = attachments.begin();
   for (; it != attachments.end(); ++it)
   {
      ConfigurationObject* attachmentObj = new ConfigurationObject(UserAttachmentObject);
      attachmentObj->SetProperty(L"param0", *it);
      confObject->AddObject(attachmentObj);
   }
}
