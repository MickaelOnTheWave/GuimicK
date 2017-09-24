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

private:
    bool RetrieveRemoteConfiguration(std::string& output);

    JobStatus* CreateErrorStatus(const std::string& message);


    std::string configurationFile;
    std::string host, user;
};

#endif // REMOTEJOBSRUNNER_H
