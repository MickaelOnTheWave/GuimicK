#include "jobdebuginformationmanager.h"

#include <sstream>
#include "filetools.h"

using namespace std;

JobDebugInformationManager::JobDebugInformationManager(const string &jobName,
        const bool _usingDebug
        ) : debugUse(_usingDebug), allData("")
{
    debugFilename = jobName + "DebugInformation.txt";
}

JobDebugInformationManager::JobDebugInformationManager(const JobDebugInformationManager &other)
{
    debugFilename = other.debugFilename;
    debugUse = other.debugUse;
    allData = other.allData;
}

void JobDebugInformationManager::Reset()
{
    allData = "";
}

int JobDebugInformationManager::GetUse() const
{
    return debugUse;
}

void JobDebugInformationManager::SetUse(const int value)
{
    debugUse = value;
}

void JobDebugInformationManager::AddTagLine(const string &tag)
{
    if (debugUse == false)
        return;

    std::stringstream line;
    line << tag << std::endl;
    allData += line.str();
}

JobStatus *JobDebugInformationManager::UpdateStatus(JobStatus *status) const
{
    if (debugUse == true)
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
