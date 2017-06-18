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

void JobDebugInformationManager::AddTagLine(const string &tag)
{
    if (usingDebug == false)
        return;

    std::stringstream line;
    line << tag << std::endl;
    allData += line.str();
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
