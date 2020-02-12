#ifndef JOBDEBUGINFORMATIONMANAGER_H
#define JOBDEBUGINFORMATIONMANAGER_H

#include <sstream>
#include <string>

#include "debugoutput.h"
#include "jobstatus.h"

class JobDebugInformationManager
{
public:
    explicit JobDebugInformationManager(const int _debugUse, const std::wstring& jobName = L"");
    JobDebugInformationManager(const JobDebugInformationManager& other);

    void SetJobName(const std::wstring& name);
    void Reset();
    void SetUse(const int value);
    int GetUse() const;

    void AddTagLine(const std::wstring& tag);

    template <class T>
    void AddDataLine(const std::wstring& label, const T& data)
    {
        if (debugUse == DebugOutput::NEVER)
            return;

        std::wstringstream line;
        line << label << " : " << data << std::endl;
        allData += line.str();
    }

    JobStatus* UpdateStatus(JobStatus* status) const;
    JobStatus* CreateStatus(const int code, const std::wstring &description) const;
    void WriteToFile() const;

    bool ShouldAttachDebugInformation(const int statusCode) const;

private:
    std::wstring debugFilename;
    int debugUse;
    std::wstring allData;
};

#endif // JOBDEBUGINFORMATIONMANAGER_H
