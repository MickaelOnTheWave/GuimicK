#ifndef JOBDEBUGINFORMATIONMANAGER_H
#define JOBDEBUGINFORMATIONMANAGER_H

#include <string>

#include "jobstatus.h"

class JobDebugInformationManager
{
public:
    JobDebugInformationManager(const std::string& jobName, const bool _usingDebug);

    void AddIntDataLine(const std::string& label, const int data);
    void AddStringDataLine(const std::string& label, const std::string& data);

    JobStatus* UpdateStatus(JobStatus* status) const;

private:
    std::string debugFilename;
    bool usingDebug;
    std::string allData;
};

#endif // JOBDEBUGINFORMATIONMANAGER_H
