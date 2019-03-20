#include "sshconsolejob.h"

#include <sstream>
#include "consolejob.h"
#include "pathtools.h"
#include "tools.h"
#include "userconsolejob.h"

using namespace std;

wstring SshConsoleJob::NoTargetError = L"No target specified";
wstring SshConsoleJob::InvalidTargetError = L"Invalid target specified";
wstring SshConsoleJob::NoTerminalForPasswordError = L"Password needed";
wstring SshConsoleJob::FailedRemoteCopyError = L"Failed to attach files";

const wstring emptyString = L"";

SshConsoleJob::SshConsoleJob(AbstractConsoleJob *_job)
    : AbstractConsoleJob(L""), title(L""), user(L""), host(L"")
{
    SetRemoteJob(_job);
}

SshConsoleJob::SshConsoleJob(const wstring &_title, const wstring &_command)
    : AbstractConsoleJob(L""), title(_title), user(L""), host(L"")
{

    SetRemoteJob(new ConsoleJob(_command));
}

SshConsoleJob::SshConsoleJob(const SshConsoleJob &other)
    : AbstractConsoleJob(other)
{
    user = other.user;
    host = other.host;
    remoteJob = static_cast<AbstractConsoleJob*>(other.remoteJob->Clone());
}

SshConsoleJob::~SshConsoleJob()
{
    delete remoteJob;
}

wstring SshConsoleJob::GetName()
{
    return remoteJob->GetName();
}

AbstractJob *SshConsoleJob::Clone()
{
    return new SshConsoleJob(*this);
}

void SshConsoleJob::SetTarget(const wstring &_user, const wstring &_host)
{
    user = _user;
    host= _host;
}

bool SshConsoleJob::InitializeFromClient(Client *client)
{
    if (AbstractConsoleJob::InitializeFromClient(client))
    {
        if (user == L"")
            user = client->GetProperty("sshuser");

        if (host == L"")
            host = client->GetProperty("ip");

        return IsInitialized();
    }
    else
        return false;
}

bool SshConsoleJob::IsInitialized()
{
    return (user != L"" && host != L"");
}

JobStatus *SshConsoleJob::Run()
{
    if (IsInitialized() == false)
        return new JobStatus(JobStatus::Error, NoTargetError);
    else if (Tools::IsComputerAlive(host) == false)
        return new JobStatus(JobStatus::Error, InvalidTargetError);

    AbstractConsoleJob* sshJob = CreateSshJob();

    debugManager->AddDataLine<wstring>(L"Child command", sshJob->GetCommand());
    debugManager->AddDataLine<wstring>(L"Child params", sshJob->GetCommandParameters());

    bool remoteCopyWithoutErrors = CopyRemoteAttachments();

    JobStatus* status = sshJob->Run();

    remoteJob->SetCommandReturnCode(sshJob->GetCommandReturnCode());
    remoteJob->SetCommandOutput(sshJob->GetCommandOutput());

    if (IsAskTerminalError(status, sshJob->GetCommandOutput()))
        status->SetDescription(NoTerminalForPasswordError);
    else if (!remoteCopyWithoutErrors)
    {
       status->SetCode(JobStatus::OkWithWarnings);
       status->SetDescription(FailedRemoteCopyError);
    }

    delete sshJob;
    return status;
}

void SshConsoleJob::SetTitle(const wstring &value)
{
    title = value;
}

int SshConsoleJob::GetExpectedReturnCode() const
{
    return remoteJob->GetExpectedReturnCode();
}

void SshConsoleJob::SetExpectedReturnCode(const int value)
{
    remoteJob->SetExpectedReturnCode(value);
}

wstring SshConsoleJob::GetCommand() const
{
    return remoteJob->GetCommand();
}

void SshConsoleJob::SetCommand(const wstring &command)
{
    remoteJob->SetCommand(command);
}

wstring SshConsoleJob::GetCommandParameters() const
{
    return remoteJob->GetCommandParameters();
}

void SshConsoleJob::SetCommandParameters(const wstring &parameters)
{
    remoteJob->SetCommandParameters(parameters);
}

int SshConsoleJob::GetCommandReturnCode() const
{
    return remoteJob->GetCommandReturnCode();
}

void SshConsoleJob::SetCommandReturnCode(const int value)
{
    remoteJob->SetCommandReturnCode(value);
}

wstring SshConsoleJob::GetCommandOutput() const
{
    return remoteJob->GetCommandOutput();
}

void SshConsoleJob::SetCommandOutput(const wstring &value)
{
    remoteJob->SetCommandOutput(value);
}

bool SshConsoleJob::IsCommandAvailable() const
{
    // No way to check remotely if command exists.
    return true;
}

bool SshConsoleJob::IsRunOk() const
{
   return remoteJob->IsRunOk();
}

void SshConsoleJob::GetUserAttachments(std::vector<wstring>& attachments)
{
   UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
   if (castJob)
      castJob->GetUserAttachments(attachments);
}

void SshConsoleJob::AddUserAttachment(const wstring& name)
{
   UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
   if (castJob)
      castJob->AddUserAttachment(name);
}

void SshConsoleJob::SetRemoteJob(AbstractConsoleJob *_remoteJob)
{
    remoteJob = _remoteJob;
    remoteJob->SetParentDebugManager(debugManager);
}

wstring SshConsoleJob::GetExpectedOutput() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetExpectedOutput() : wstring(L"");
}

void SshConsoleJob::SetExpectedOutput(const wstring &value)
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    if (castJob)
        castJob->SetExpectedOutput(value);
}

wstring SshConsoleJob::GetOutputFile() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetOutputFile() : emptyString;
}

wstring SshConsoleJob::GetMiniDescriptionParserCommand() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetMiniDescriptionParserCommand() : emptyString;
}

bool SshConsoleJob::IsParsingUsingBuffer() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->IsParsingUsingBuffer() : false;
}

AbstractConsoleJob* SshConsoleJob::GetRemoteJob()
{
   return remoteJob;
}

AbstractConsoleJob *SshConsoleJob::CreateSshJob()
{
    AbstractConsoleJob* sshJob = static_cast<AbstractConsoleJob*>(remoteJob->Clone());
    const wstring remoteJobCommand = remoteJob->GetCommand() + L" " +
                                    remoteJob->GetCommandParameters();

    const wstring sshBatchMode = L"-o BatchMode=yes ";
    const wstring sshParameters = sshBatchMode + user + L"@" + host + L" \"" + remoteJobCommand + L"\"";
    sshJob->SetCommand(L"ssh");
    sshJob->SetCommandParameters(sshParameters);
    sshJob->SetParentDebugManager(debugManager);

    debugManager->AddDataLine<wstring>(L"Ssh parameters", sshParameters);
    return sshJob;
}

bool SshConsoleJob::IsAskTerminalError(JobStatus *status,
                                       const wstring& message) const
{
    const wstring expectedOutput = L"sudo: no tty present";
    return (status->GetCode() == JobStatus::Error &&
            message.find(expectedOutput) == 0);
}

bool SshConsoleJob::CopyRemoteAttachments()
{
   const wstring startTag = L"\"'";
   const wstring endTag = L"'\"";

   UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
   const bool hasAttachments = (!castJob || !castJob->HasUserAttachments());
   debugManager->AddDataLine<bool>(L"Has User Attachments?", hasAttachments);
   if (hasAttachments)
      return true;

   vector<wstring> userAttachments;
   castJob->GetUserAttachments(userAttachments);
   castJob->EmptyUserAttachments();

   bool allOk = true;
   vector<wstring>::const_iterator it = userAttachments.begin();
   for (; it != userAttachments.end(); ++it)
   {
      debugManager->AddDataLine<wstring>(L"Copying Attachment", *it);
      wstring scpParams = user + L"@" + host + L":";
      if ((*it)[0] == '/')
         scpParams += startTag + *it;
      else
         scpParams += wstring(L"~/") + startTag + *it;
      scpParams += endTag + L" ./";

      debugManager->AddDataLine<wstring>(L"Scp parameters", scpParams);

      ConsoleJob copyJob(L"scp", scpParams);
      copyJob.RunWithoutStatus();

      debugManager->AddDataLine<bool>(L"Scp Ok", copyJob.IsRunOk());
      debugManager->AddDataLine<wstring>(L"Scp Output", copyJob.GetCommandOutput());

      if (copyJob.IsRunOk())
      {
         const wstring localAttachment = PathTools::GetFilenameOnly(*it);
         debugManager->AddDataLine<wstring>(L"Adding Local Attachment", localAttachment);
         castJob->AddUserAttachment(localAttachment);
      }
      else
         allOk = false;
   }
   return allOk;
}
