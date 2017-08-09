#include "backupstatusmanager.h"

#include <sstream>

using namespace std;

BackupStatusManager::BackupStatusManager(const string &_attachmentName)
    : resultCollection(NULL), debugManager(NULL), joinReports(false),
      attachmentName(_attachmentName)
{
}

BackupStatusManager::BackupStatusManager(const BackupStatusManager &other)
    : resultCollection(other.resultCollection),
      debugManager(other.debugManager),
      joinReports(other.joinReports),
      attachmentName(other.attachmentName)
{
}

void BackupStatusManager::SetDebugManager(JobDebugInformationManager *_debugManager)
{
    debugManager = _debugManager;
}

void BackupStatusManager::SetJoinReports(const bool value)
{
    joinReports = value;
}

void BackupStatusManager::SetAttachmentName(const string &name)
{
    attachmentName = name;
}

JobStatus *BackupStatusManager::CreateGlobalStatus(
        const AbstractBackupJob::ResultCollection &results,
        const AbstractBackupJob::BackupCollection& backups)
{
    resultCollection = &results;
    backupCollection = &backups;

    if (debugManager)
        debugManager->AddDataLine<int>("Statuses to handle", resultCollection->size());
    if (resultCollection->size() == 1)
        return CreateSingleStatus();
    else
    {
        if (AreAllStatusesEqual(JobStatus::OK))
            return CreateAllOkStatus();
        else
            return CreateMixedStatus();
    }
}

JobStatus *BackupStatusManager::CreateSingleStatus()
{
    debugManager->AddTagLine("Creating Single status");
    const AbstractBackupJob::ResultEntry& result = resultCollection->front();

    const int statusCode = result.first->GetCode();
    JobStatus* status = new JobStatus(statusCode);
    if (statusCode == JobStatus::OK || statusCode == JobStatus::OK_WITH_WARNINGS)
        status->SetDescription(result.second->GetMiniDescription());
    else
        status->SetDescription(result.first->GetDescription());

    status->AddFileBuffer(attachmentName, result.second->GetFullDescription());
    return status;
}

JobStatus *BackupStatusManager::CreateAllOkStatus()
{
    JobStatus* status = new JobStatus(JobStatus::OK);

    if (joinReports)
        return CreateJoinedStatus();
    else
        return CreateSeparatedStatus();

    return status;
}

JobStatus *BackupStatusManager::CreateMixedStatus()
{
    if (debugManager)
        debugManager->AddTagLine("Creating Mixed status");

    JobStatus* status = new JobStatus(JobStatus::ERROR);
    status->SetDescription(CreateFoldersMiniDescription());
    status->AddFileBuffer(attachmentName, CreateStatusesDescription());
    return status;
}

JobStatus *BackupStatusManager::CreateJoinedStatus()
{
    if (debugManager)
        debugManager->AddTagLine("Creating Joined status");

    JobStatus* status = new JobStatus(JobStatus::OK);
    FileBackupReport globalReport;
    AbstractBackupJob::BackupCollection::const_iterator itDestination = backupCollection->begin();
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it, ++itDestination)
        globalReport.AddWithPrefix(*it->second, itDestination->second);

    status->SetDescription(globalReport.GetMiniDescription());
    status->AddFileBuffer(attachmentName, globalReport.GetFullDescription());
    return status;
}

JobStatus *BackupStatusManager::CreateSeparatedStatus()
{
    if (debugManager)
        debugManager->AddTagLine("Creating Separated status");

    JobStatus* status = new JobStatus(JobStatus::OK);
    FileBackupReport globalReport;
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it)
        globalReport.Add(*it->second);

    status->SetDescription(globalReport.GetMiniDescription());
    status->AddFileBuffer(attachmentName, CreateStatusesDescription());
    return status;
}

bool BackupStatusManager::AreAllStatusesEqual(const int expectedCode)
{
    bool areAllOk = true;
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it != resultCollection->end(); ++it)
    {
        if (it->first->GetCode() != expectedCode)
        {
            areAllOk = false;
            break;
        }
    }

    return areAllOk;
}


string BackupStatusManager::CreateStatusesDescription()
{
    string fullDescription("");
    AbstractBackupJob::BackupCollection::const_iterator itDestination = backupCollection->begin();
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it, ++itDestination)
    {
        fullDescription += BuildRepositoryHeader(itDestination->second);
        if (it->second)
            fullDescription += it->second->GetFullDescription();
    }
    fullDescription += BuildFooter();
    return fullDescription;
}

string BackupStatusManager::CreateFoldersMiniDescription()
{
    const int successCount = ComputeSuccessCount();
    const int failureCount = resultCollection->size() - successCount;

    stringstream miniDescription;
    if (successCount > 0)
    {
        miniDescription << successCount << " succeeded";
        miniDescription << ((failureCount > 0) ? ", " : ".");
    }

    if (failureCount > 0)
        miniDescription << failureCount << "failed.";

    return miniDescription.str();
}

int BackupStatusManager::ComputeSuccessCount() const
{
    int count = 0;
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it)
    {
        if (it->first->GetCode() == JobStatus::OK)
            ++count;
    }
    return count;
}

string BackupStatusManager::BuildRepositoryHeader(const string& name)
{
    return string("--- ") + name + " ---\n";
}

string BackupStatusManager::BuildFooter()
{
    return string("------------------\n");
}
