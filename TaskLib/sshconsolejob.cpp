#include "sshconsolejob.h"

#include <sstream>
#include "consolejob.h"
#include "tools.h"
#include "userconsolejob.h"

using namespace std;

string SshConsoleJob::NoTargetError = "No target specified";
string SshConsoleJob::InvalidTargetError = "Invalid target specified";
string SshConsoleJob::NoTerminalForPasswordError = "Password needed";
string SshConsoleJob::FailedRemoteCopyError = "Failed to attach files";

SshConsoleJob::SshConsoleJob(AbstractConsoleJob *_job)
    : AbstractConsoleJob(""), title(""), user(""), host("")
{
    SetRemoteJob(_job);
}

SshConsoleJob::SshConsoleJob(const string &_title, const string &_command)
    : AbstractConsoleJob(""), title(_title), user(""), host("")
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

string SshConsoleJob::GetName()
{
    return remoteJob->GetName();
}

AbstractJob *SshConsoleJob::Clone()
{
    return new SshConsoleJob(*this);
}

void SshConsoleJob::SetTarget(const string &_user, const string &_host)
{
    user = _user;
    host= _host;
}

bool SshConsoleJob::InitializeFromClient(Client *client)
{
    if (AbstractConsoleJob::InitializeFromClient(client))
    {
        if (user == "")
            user = client->GetProperty("sshuser");

        if (host == "")
            host = client->GetProperty("ip");

        return IsInitialized();
    }
    else
        return false;
}

bool SshConsoleJob::IsInitialized()
{
    return (user != "" && host != "");
}

JobStatus *SshConsoleJob::Run()
{
    if (IsInitialized() == false)
        return new JobStatus(JobStatus::ERROR, NoTargetError);
    else if (Tools::IsComputerAlive(host) == false)
        return new JobStatus(JobStatus::ERROR, InvalidTargetError);

    AbstractConsoleJob* sshJob = CreateSshJob();

    debugManager->AddDataLine<string>("Child command", sshJob->GetCommand());
    debugManager->AddDataLine<string>("Child params", sshJob->GetCommandParameters());

    bool remoteCopyWithoutErrors = CopyRemoteAttachments();

    JobStatus* status = sshJob->Run();

    remoteJob->SetCommandReturnCode(sshJob->GetCommandReturnCode());
    remoteJob->SetCommandOutput(sshJob->GetCommandOutput());

    if (IsAskTerminalError(status, sshJob->GetCommandOutput()))
        status->SetDescription(NoTerminalForPasswordError);
    else if (!remoteCopyWithoutErrors)
    {
       status->SetCode(JobStatus::OK_WITH_WARNINGS);
       status->SetDescription(FailedRemoteCopyError);
    }

    delete sshJob;
    return status;
}

void SshConsoleJob::SetTitle(const string &value)
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

string SshConsoleJob::GetCommand() const
{
    return remoteJob->GetCommand();
}

void SshConsoleJob::SetCommand(const string &command)
{
    remoteJob->SetCommand(command);
}

string SshConsoleJob::GetCommandParameters() const
{
    return remoteJob->GetCommandParameters();
}

void SshConsoleJob::SetCommandParameters(const string &parameters)
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

string SshConsoleJob::GetCommandOutput() const
{
    return remoteJob->GetCommandOutput();
}

void SshConsoleJob::SetCommandOutput(const string &value)
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

void SshConsoleJob::GetUserAttachments(std::vector<string>& attachments)
{
   UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
   if (castJob)
      castJob->GetUserAttachments(attachments);
}

void SshConsoleJob::AddUserAttachment(const string& name)
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

string SshConsoleJob::GetExpectedOutput() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetExpectedOutput() : string("");
}

void SshConsoleJob::SetExpectedOutput(const string &value)
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    if (castJob)
        castJob->SetExpectedOutput(value);
}

string SshConsoleJob::GetOutputFile() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetOutputFile() : string("");
}

string SshConsoleJob::GetMiniDescriptionParserCommand() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->GetMiniDescriptionParserCommand() : string("");
}

bool SshConsoleJob::IsParsingUsingBuffer() const
{
    UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
    return (castJob) ? castJob->IsParsingUsingBuffer() : false;
}

AbstractConsoleJob *SshConsoleJob::CreateSshJob()
{
    AbstractConsoleJob* sshJob = static_cast<AbstractConsoleJob*>(remoteJob->Clone());
    const string remoteJobCommand = remoteJob->GetCommand() + " " +
                                    remoteJob->GetCommandParameters();

    const string sshBatchMode = "-o BatchMode=yes ";
    const string sshParameters = sshBatchMode + user + "@" + host + " \"" + remoteJobCommand + "\"";
    sshJob->SetCommand("ssh");
    sshJob->SetCommandParameters(sshParameters);
    sshJob->SetParentDebugManager(debugManager);

    debugManager->AddDataLine<string>("Ssh parameters", sshParameters);
    return sshJob;
}

bool SshConsoleJob::IsAskTerminalError(JobStatus *status,
                                       const string& message) const
{
    const string expectedOutput = "sudo: no tty present";
    return (status->GetCode() == JobStatus::ERROR &&
            message.find(expectedOutput) == 0);
}

bool SshConsoleJob::CopyRemoteAttachments()
{
   const string startTag = "\"'";
   const string endTag = "'\"";

   UserConsoleJob* castJob = dynamic_cast<UserConsoleJob*>(remoteJob);
   const bool hasAttachments = (!castJob || !castJob->HasUserAttachments());
   debugManager->AddDataLine<bool>("Has User Attachments?", hasAttachments);
   if (hasAttachments)
      return true;

   vector<string> userAttachments;
   castJob->GetUserAttachments(userAttachments);
   castJob->EmptyUserAttachments();

   bool allOk = true;
   vector<string>::const_iterator it = userAttachments.begin();
   for (; it != userAttachments.end(); ++it)
   {
      debugManager->AddDataLine<string>("Copying Attachment", *it);
      string scpParams = user + "@" + host + ":";
      if ((*it)[0] == '/')
         scpParams += startTag + *it;
      else
         scpParams += string("~/") + startTag + *it;
      scpParams += endTag + " ./";

      debugManager->AddDataLine<string>("Scp parameters", scpParams);

      ConsoleJob copyJob("scp", scpParams);
      copyJob.RunWithoutStatus();

      debugManager->AddDataLine<bool>("Scp Ok", copyJob.IsRunOk());
      debugManager->AddDataLine<string>("Scp Output", copyJob.GetCommandOutput());

      if (copyJob.IsRunOk())
      {
         const string localAttachment = Tools::GetFilenameOnly(*it);
         debugManager->AddDataLine<string>("Adding Local Attachment", localAttachment);
         castJob->AddUserAttachment(localAttachment);
      }
      else
         allOk = false;
   }
   return allOk;
}
