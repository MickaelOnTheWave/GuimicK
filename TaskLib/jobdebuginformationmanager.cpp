#include "jobdebuginformationmanager.h"

#include <sstream>
#include "filetools.h"

using namespace std;

JobDebugInformationManager::JobDebugInformationManager(const int _debugUse, const std::string& jobName)
    : debugFilename(""), debugUse(_debugUse), allData("")
{
    if (jobName != "")
        SetJobName(jobName);
}

JobDebugInformationManager::JobDebugInformationManager(const JobDebugInformationManager &other)
    : debugFilename(other.debugFilename),
      debugUse(other.debugUse),
      allData(other.allData)
{
}

void JobDebugInformationManager::SetJobName(const string &name)
{
    debugFilename = name + " - Debug.txt";
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
    if (debugUse == DebugOutput::NEVER)
        return;

    std::stringstream line;
    line << tag << std::endl;
    allData += line.str();
}

JobStatus *JobDebugInformationManager::UpdateStatus(JobStatus *status) const
{
    if (status && ShouldAttachDebugInformation(status->GetCode()))
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

bool JobDebugInformationManager::ShouldAttachDebugInformation(const int statusCode) const
{
    const bool isStatusOk = (statusCode == JobStatus::OK);
    const bool isStatusNotExecuted = (statusCode == JobStatus::NOT_EXECUTED);
    const bool isStatusError = (statusCode == JobStatus::ERROR);

    switch (debugUse)
    {
        case (DebugOutput::ALWAYS) :
            return true;
        case (DebugOutput::ON_ANY_ERROR) :
            return !isStatusOk;
        case (DebugOutput::ON_FATAL_ERROR) :
            return isStatusError || isStatusNotExecuted;
        default :
            return false;
    }
}
