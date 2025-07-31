#ifndef SSHCONSOLEJOB_H
#define SSHCONSOLEJOB_H

#include "AbstractConsoleJob.h"

class SshConsoleJob : public AbstractConsoleJob
{
public:
    static std::wstring NoTargetError;
    static std::wstring InvalidTargetError;
    static std::wstring NoTerminalForPasswordError;
    static std::wstring FailedRemoteCopyError;

    SshConsoleJob(AbstractConsoleJob* _job);
    SshConsoleJob(const std::wstring& _title, const std::wstring& _command = L"");
    SshConsoleJob(const SshConsoleJob& other);
    virtual ~SshConsoleJob();

    std::wstring GetName() const override;

    virtual AbstractJob* Clone();

    void SetTarget(const std::wstring& _user, const std::wstring& _host);

    bool InitializeFromClient(Client *client) override;

    bool IsInitialized(void) override;

    JobStatus* Run() override;

    virtual void SetTitle(const std::wstring& value);

    virtual int GetExpectedReturnCode() const;
    virtual void SetExpectedReturnCode(const int value);

    virtual std::wstring GetCommand() const;
    virtual void SetCommand(const std::wstring& command);

    virtual std::wstring GetCommandParameters() const;
    virtual void SetCommandParameters(const std::wstring& parameters);

    virtual int GetCommandReturnCode() const;
    virtual void SetCommandReturnCode(const int value);

    virtual std::wstring GetCommandOutput() const override;
    virtual void SetCommandOutput(const std::wstring& value) override;

    virtual bool IsCommandAvailable() const;

    virtual bool IsRunOk() const override;

    void GetUserAttachments(std::vector<std::wstring>& attachments);
    void AddUserAttachment(const std::wstring& name);

    void SetRemoteJob(AbstractConsoleJob* _remoteJob);

    std::wstring GetExpectedOutput() const;
    void SetExpectedOutput(const std::wstring& value);

    std::wstring GetOutputFile() const;

    std::wstring GetMiniDescriptionParserCommand() const;

    bool IsParsingUsingBuffer() const;

    AbstractConsoleJob* GetRemoteJob();

private:
   AbstractConsoleJob* CreateSshJob();

   bool IsAskTerminalError(JobStatus* status,
                         const std::wstring &message) const;

   bool CopyRemoteAttachments();

   std::wstring title;
   std::wstring user;
   std::wstring host;

   AbstractConsoleJob* remoteJob;
};

#endif // SSHCONSOLEJOB_H
