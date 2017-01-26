#include "jobdebuginformationmanager.h"

#include <sstream>

using namespace std;

JobDebugInformationManager::JobDebugInformationManager(const string &jobName,
        const bool _usingDebug
        ) : usingDebug(_usingDebug), allData("")
{
    debugFilename = jobName + "DebugInformation.txt";
}

void JobDebugInformationManager::AddIntDataLine(const string &label, const int data)
{
    if (usingDebug == false)
        return;

    stringstream line;
    line << label << " : " << data << endl;
    allData += line.str();
}

void JobDebugInformationManager::AddStringDataLine(const string &label, const string &data)
{
    if (usingDebug == false)
        return;

    stringstream line;
    line << label << " : <" << data << ">" << endl;
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
