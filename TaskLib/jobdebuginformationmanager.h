#ifndef JOBDEBUGINFORMATIONMANAGER_H
#define JOBDEBUGINFORMATIONMANAGER_H

#include <sstream>
#include <string>

#include "jobstatus.h"

class JobDebugInformationManager
{
public:
    JobDebugInformationManager(const std::string& jobName, const bool _usingDebug);
    JobDebugInformationManager(const JobDebugInformationManager& other);

    void Reset();
    bool IsUsed() const;
    void SetUse(const bool value);

    void AddTagLine(const std::string& tag);

    template <class T>
    void AddDataLine(const std::string& label, const T& data)
    {
        if (usingDebug == false)
            return;

        std::stringstream line;
        line << label << " : " << data << std::endl;
        allData += line.str();
    }

    JobStatus* UpdateStatus(JobStatus* status) const;
    JobStatus* CreateStatus(const int code, const std::string &description) const;
    void WriteToFile() const;

private:
    std::string debugFilename;
    bool usingDebug;
    std::string allData;
};

#endif // JOBDEBUGINFORMATIONMANAGER_H
