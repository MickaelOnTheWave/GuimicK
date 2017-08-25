#include "backupstatusmanager.h"

#include <sstream>

using namespace std;

static const string defaultItemBackupMessage = "backups succeeded";

BackupStatusManager::BackupStatusManager(const string &_attachmentName)
    : resultCollection(NULL), debugManager(NULL), joinReports(true),
      attachmentName(_attachmentName),
      itemBackupMessage(defaultItemBackupMessage)
{
}

BackupStatusManager::BackupStatusManager(const BackupStatusManager &other)
    : resultCollection(other.resultCollection),
      debugManager(other.debugManager),
      joinReports(other.joinReports),
      attachmentName(other.attachmentName),
      itemBackupMessage(other.itemBackupMessage)
{
}

void BackupStatusManager::SetDebugManager(JobDebugInformationManager *_debugManager)
{
    debugManager = _debugManager;
}

bool BackupStatusManager::GetJoinReports() const
{
    return joinReports;
}

void BackupStatusManager::SetJoinReports(const bool value)
{
    joinReports = value;
}

void BackupStatusManager::SetAttachmentName(const string &name)
{
    attachmentName = name;
}

void BackupStatusManager::SetItemBackupMessage(const string &message)
{
    itemBackupMessage = message;
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
            return CreateSeparatedStatus(JobStatus::ERROR);
    }
}

JobStatus *BackupStatusManager::CreateSingleStatus()
{
    debugManager->AddTagLine("Creating Single status");
    const AbstractBackupJob::ResultEntry& result = resultCollection->front();

    JobStatus* status = new JobStatus(result.first->GetCode());
    status->SetDescription(GetCorrectMiniDescription(result));
    if (result.second)
        status->AddFileBuffer(attachmentName, result.second->GetFullDescription());
    return status;
}

JobStatus *BackupStatusManager::CreateAllOkStatus()
{
    JobStatus* status = new JobStatus(JobStatus::OK);

    if (joinReports)
        return CreateJoinedStatus();
    else
        return CreateSeparatedStatus(JobStatus::OK);

    return status;
}

JobStatus *BackupStatusManager::CreateJoinedStatus()
{
    if (debugManager)
        debugManager->AddTagLine("Creating Joined status");

    FileBackupReport* globalReport = CreateGlobalReport();

    JobStatus* status = new JobStatus(JobStatus::OK);
    status->SetDescription(CreateRepositoriesMiniDescription());
    status->AddFileBuffer(attachmentName, globalReport->GetFullDescription());

    delete globalReport;
    return status;
}

JobStatus *BackupStatusManager::CreateSeparatedStatus(const int code)
{
    if (debugManager)
        debugManager->AddTagLine("Creating Separated status");

    JobStatus* status = new JobStatus(code);

    status->SetDescription(CreateRepositoriesMiniDescription());
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

string BackupStatusManager::CreateRepositoriesMiniDescription()
{
    const int successCount = ComputeSuccessCount();
    const int failureCount = resultCollection->size() - successCount;

    stringstream miniDescription;
    if (successCount > 0)
    {
        miniDescription << successCount << " " << itemBackupMessage;
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

string BackupStatusManager::GetCorrectMiniDescription(
        const AbstractBackupJob::ResultEntry &result) const
{
    const int statusCode = result.first->GetCode();
    const bool isStatusCodeAcceptable = (statusCode == JobStatus::OK ||
                                         statusCode == JobStatus::OK_WITH_WARNINGS);
    string description;
    if (isStatusCodeAcceptable && result.second != NULL)
        description = result.second->GetMiniDescription();
    else
        description = result.first->GetDescription();
    return description;
}

FileBackupReport *BackupStatusManager::CreateGlobalReport() const
{
    FileBackupReport* report = new FileBackupReport();
    AbstractBackupJob::BackupCollection::const_iterator itDestination = backupCollection->begin();
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it, ++itDestination)
    {
        if (it->second != NULL)
            report->AddWithPrefix(*it->second, itDestination->second);
    }
    return report;
}
