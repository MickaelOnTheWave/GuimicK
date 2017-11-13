#ifndef SSHCONSOLEJOB_H
#define SSHCONSOLEJOB_H

#include "AbstractConsoleJob.h"

class SshConsoleJob : public AbstractConsoleJob
{
public:
    static std::string NoTargetError;
    static std::string InvalidTargetError;
    static std::string NoTerminalForPasswordError;
    static std::string FailedRemoteCopyError;

    SshConsoleJob(AbstractConsoleJob* _job);
    SshConsoleJob(const std::string& _title, const std::string& _command = "");
    SshConsoleJob(const SshConsoleJob& other);
    virtual ~SshConsoleJob();

    virtual std::string GetName();

    virtual AbstractJob* Clone();

    void SetTarget(const std::string& _user, const std::string& _host);

    virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    virtual void SetTitle(const std::string& value);

    virtual int GetExpectedReturnCode() const;
    virtual void SetExpectedReturnCode(const int value);

    virtual std::string GetCommand() const;
    virtual void SetCommand(const std::string& command);

    virtual std::string GetCommandParameters() const;
    virtual void SetCommandParameters(const std::string& parameters);

    virtual int GetCommandReturnCode() const;
    virtual void SetCommandReturnCode(const int value);

    virtual std::string GetCommandOutput() const;
    virtual void SetCommandOutput(const std::string& value);

    virtual bool IsCommandAvailable() const;

    virtual bool IsRunOk() const;

    void GetUserAttachments(std::vector<std::string>& attachments);
    void AddUserAttachment(const std::string& name);

    void SetRemoteJob(AbstractConsoleJob* _remoteJob);

    std::string GetExpectedOutput() const;
    void SetExpectedOutput(const std::string& value);

    std::string GetOutputFile() const;

    std::string GetMiniDescriptionParserCommand() const;

    bool IsParsingUsingBuffer() const;

private:
   AbstractConsoleJob* CreateSshJob();

   bool IsAskTerminalError(JobStatus* status,
                         const std::string &message) const;

   bool CopyRemoteAttachments();

   std::string title;
   std::string user;
   std::string host;

   AbstractConsoleJob* remoteJob;
};

#endif // SSHCONSOLEJOB_H
