#ifndef JOBDEBUGINFORMATIONMANAGER_H
#define JOBDEBUGINFORMATIONMANAGER_H

#include <sstream>
#include <string>

#include "debugoutput.h"
#include "jobstatus.h"

class JobDebugInformationManager
{
public:
    explicit JobDebugInformationManager(const int _debugUse, const std::string& jobName = "");
    JobDebugInformationManager(const JobDebugInformationManager& other);

    void SetJobName(const std::string& name);
    void Reset();
    void SetUse(const int value);
    int GetUse() const;

    void AddTagLine(const std::string& tag);

    template <class T>
    void AddDataLine(const std::string& label, const T& data)
    {
        if (debugUse == DebugOutput::NEVER)
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
    int debugUse;
    std::string allData;
};

#endif // JOBDEBUGINFORMATIONMANAGER_H
