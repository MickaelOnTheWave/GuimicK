#ifndef SSHCONSOLEJOB_H
#define SSHCONSOLEJOB_H

#include "AbstractConsoleJob.h"
#include "consolejob.h"

class SshConsoleJob : public AbstractConsoleJob
{
public:
    SshConsoleJob(const std::string& _title, ConsoleJob* _job);
    SshConsoleJob(const std::string& _title, const std::string& _command = "");
    SshConsoleJob(const SshConsoleJob& other);
    virtual ~SshConsoleJob();

    virtual std::string GetName();

    virtual AbstractJob* Clone();

    void SetTarget(const std::string& _user, const std::string& _host);

    virtual bool InitializeFromClient(Client *client);

    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    virtual int GetExpectedReturnCode() const;
    virtual void SetExpectedReturnCode(const int value);

    virtual std::string GetCommand() const;
    virtual void SetCommand(const std::string& command);

    virtual int GetCommandReturnCode() const;
    virtual std::string GetCommandOutput() const;

private:
    std::string title;
	std::string user;
	std::string host;

    ConsoleJob* remoteJob;
};

#endif // SSHCONSOLEJOB_H
