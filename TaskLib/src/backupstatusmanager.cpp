#include "backupstatusmanager.h"

#include <sstream>

using namespace std;

static const wstring defaultItemBackupMessage = L"backups succeeded";

BackupStatusManager::BackupStatusManager(const wstring &_attachmentName)
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

void BackupStatusManager::SetAttachmentName(const wstring &name)
{
    attachmentName = name;
}

void BackupStatusManager::SetItemBackupMessage(const wstring &message)
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
        debugManager->AddDataLine<size_t>(L"Statuses to handle", resultCollection->size());
    if (resultCollection->size() == 1)
        return CreateSingleStatus();
    else
    {
        if (AreAllStatusesEqual(JobStatus::Ok))
            return CreateAllOkStatus();
        else
            return CreateSeparatedStatus(JobStatus::Error);
    }
}

JobStatus *BackupStatusManager::CreateSingleStatus()
{
   if (debugManager)
      debugManager->AddTagLine(L"Creating Single status");
   const AbstractBackupJob::ResultEntry& result = resultCollection->front();

   JobStatus* status = new JobStatus(result.first->GetCode());
   status->SetDescription(GetCorrectMiniDescription(result));
   if (result.second)
     status->AddFileBuffer(attachmentName, result.second->GetFullDescription());
   return status;
}

JobStatus *BackupStatusManager::CreateAllOkStatus()
{
    if (joinReports)
        return CreateJoinedStatus();
    else
        return CreateSeparatedStatus(JobStatus::Ok);
}

JobStatus *BackupStatusManager::CreateJoinedStatus()
{
    if (debugManager)
        debugManager->AddTagLine(L"Creating Joined status");

    FileBackupReport* globalReport = CreateGlobalReport();

    JobStatus* status = new JobStatus(JobStatus::Ok);
    status->SetDescription(CreateRepositoriesMiniDescription());
    status->AddFileBuffer(attachmentName, globalReport->GetFullDescription());

    delete globalReport;
    return status;
}

JobStatus *BackupStatusManager::CreateSeparatedStatus(const int code)
{
    if (debugManager)
        debugManager->AddTagLine(L"Creating Separated status");

    JobStatus* status = new JobStatus(code);

    status->SetDescription(CreateRepositoriesMiniDescription());
    const wstring attachmentContent = CreateStatusesDescription();
    if (attachmentContent != L"")
      status->AddFileBuffer(attachmentName, attachmentContent);
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


wstring BackupStatusManager::CreateStatusesDescription()
{
    wstring fullDescription(L"");
    AbstractBackupJob::BackupCollection::const_iterator itDestination = backupCollection->begin();
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it, ++itDestination)
    {
       fullDescription += BuildRepositoryHeader(itDestination->second);
       if (it->first->IsOk())
          fullDescription += BuildRepositoryOkReport(it->second);
       else
          fullDescription += BuildRepositoryErrorReport(it->first);
    }
    if (fullDescription != L"")
      fullDescription += BuildFooter();
    return fullDescription;
}

wstring BackupStatusManager::CreateRepositoriesMiniDescription()
{
    const int successCount = ComputeSuccessCount();
    const int failureCount = static_cast<int>(resultCollection->size()) - successCount;

    wstringstream miniDescription;
    if (successCount > 0)
    {
        miniDescription << successCount << " " << itemBackupMessage;
        miniDescription << ((failureCount > 0) ? ", " : ".");
    }

    if (failureCount > 0)
        miniDescription << failureCount << " failed.";

    return miniDescription.str();
}

int BackupStatusManager::ComputeSuccessCount() const
{
    int count = 0;
    AbstractBackupJob::ResultCollection::const_iterator it = resultCollection->begin();
    for (; it!=resultCollection->end(); ++it)
    {
        if (it->first->GetCode() == JobStatus::Ok)
            ++count;
    }
    return count;
}

wstring BackupStatusManager::BuildRepositoryHeader(const wstring& name)
{
    return wstring(L"--- ") + name + L" ---\n";
}

wstring BackupStatusManager::BuildFooter()
{
    return wstring(L"------------------\n");
}

wstring BackupStatusManager::GetCorrectMiniDescription(
        const AbstractBackupJob::ResultEntry &result) const
{
    const int statusCode = result.first->GetCode();
    const bool isStatusCodeAcceptable = (statusCode == JobStatus::Ok ||
                                         statusCode == JobStatus::OkWithWarnings);
    wstring description;
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

wstring BackupStatusManager::BuildRepositoryOkReport(FileBackupReport* report) const
{
   return (report) ? report->GetFullDescription() : L"";
}

wstring BackupStatusManager::BuildRepositoryErrorReport(JobStatus* status) const
{
   wstring description = status->GetCodeDescription() + L"\n";
   description += status->GetDescription();
   return description;
}
