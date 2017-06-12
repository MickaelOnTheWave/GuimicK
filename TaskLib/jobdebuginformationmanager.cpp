#include "jobdebuginformationmanager.h"

#include <sstream>
#include "filetools.h"

using namespace std;

JobDebugInformationManager::JobDebugInformationManager(const string &jobName,
        const bool _usingDebug
        ) : usingDebug(_usingDebug), allData("")
{
    debugFilename = jobName + "DebugInformation.txt";
}

JobDebugInformationManager::JobDebugInformationManager(const JobDebugInformationManager &other)
{
    debugFilename = other.debugFilename;
    usingDebug = other.usingDebug;
    allData = other.allData;
}

void JobDebugInformationManager::Reset()
{
    allData = "";
}

bool JobDebugInformationManager::IsUsed() const
{
    return usingDebug;
}

void JobDebugInformationManager::SetUse(const bool value)
{
    usingDebug = value;
}

void JobDebugInformationManager::AddBoolDataLine(const string &label, const bool value)
{
    AddDataLine<bool>(label, value);
}

void JobDebugInformationManager::AddIntDataLine(const string &label, const int data)
{
    AddDataLine<int>(label, data);
}

void JobDebugInformationManager::AddStringDataLine(const string &label, const string &data)
{
    AddDataLine<string>(label, data);
}

JobStatus *JobDebugInformationManager::UpdateStatus(JobStatus *status) const
{
    if (usingDebug == true)
        status->AddFileBuffer(debugFilename, allData);
    return status;
}

JobStatus *JobDebugInformationManager::CreateStatus(const int code, const string &description) const
{
    return UpdateStatus(new JobStatus(code, description));
}

void JobDebugInformationManager::WriteToFile() const
{
    FileTools::WriteBufferToFile(debugFilename, allData);
}
