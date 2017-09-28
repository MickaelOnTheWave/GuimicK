#ifndef REMOTEJOBSRUNNER_H
#define REMOTEJOBSRUNNER_H

#include "abstractjob.h"

class RemoteJobsRunner : public AbstractJob
{
public:
    static std::string TargetNotAccessibleError;

    RemoteJobsRunner();

    virtual bool InitializeFromClient(Client *client);
    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    std::string GetConfigurationFile() const;
    void SetConfigurationFile(const std::string& value);

    bool GetIsWorkListTimed() const;
    void SetIsWorkListTimed(const bool value);

private:
    bool RetrieveRemoteConfiguration(std::string& output);

    JobStatus* CreateErrorStatus(const std::string& message);    
    JobStatus* CreateConfigurationErrorStatus(const std::vector<std::string>& errors);

    bool IsInvalidFileError() const;
    bool IsPasswordError() const;

    std::string configurationFile;
    std::string host, user;
    bool isWorkListTimed;
};

#endif // REMOTEJOBSRUNNER_H
