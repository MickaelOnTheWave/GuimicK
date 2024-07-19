#ifndef REMOTEJOBSRUNNER_H
#define REMOTEJOBSRUNNER_H

#include "abstractjob.h"

class RemoteJobsRunner : public AbstractJob
{
public:
    static std::wstring TargetNotAccessibleError;
    static std::wstring ConfigurationFileNotFoundError;
    static std::wstring PasswordNeededError;
    static std::wstring GenericRetrieveError;

    RemoteJobsRunner();
    RemoteJobsRunner(const RemoteJobsRunner& other);

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client *client);
    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    std::wstring GetConfigurationFile() const;
    void SetConfigurationFile(const std::wstring& value);

    bool GetIsWorkListTimed() const;
    void SetIsWorkListTimed(const bool value);

private:
    bool RetrieveRemoteConfiguration(std::wstring& output);

    JobStatus* CreateErrorStatus(const std::wstring& message);    
    JobStatus* CreateConfigurationErrorStatus(const std::vector<std::wstring>& errors);

    bool IsInvalidFileError(const std::wstring& output, const std::wstring& file) const;
    bool IsPasswordError(const std::wstring& output) const;

    std::wstring CreateConfigurationErrorDescription(const std::vector<std::wstring>& errors) const;

    void SetErrorMessage(const std::wstring& commandOutput, std::wstring& message) const;

    std::wstring configurationFile;
    std::wstring host, user;
    bool isWorkListTimed;
    Client* originalClient;
};

#endif // REMOTEJOBSRUNNER_H
