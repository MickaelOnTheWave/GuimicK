#ifndef GITFSBACKUPJOB_H
#define GITFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "abstractcopyfsbackupjob.h"
#include "backupstatusmanager.h"
#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"

class GitFsBackupJob : public AbstractBackupJob
{
public:
    GitFsBackupJob();
    GitFsBackupJob(const GitFsBackupJob& other);

    virtual AbstractJob* Clone();
    virtual JobStatus* Run();

    virtual std::wstring GetTypeName() const;

    bool GetForceRawCopyUse() const;
    void SetForceRawCopyUse(const bool value);

private:
    virtual JobStatus* RestoreBackupFromServer(const std::wstring &source, const std::wstring &destination);

    void RunRepositoryBackup(const std::wstring& source,
                             const std::wstring& destination,
                             ResultCollection &results);

    void CreateGitRepository(const std::wstring& path, JobStatus* status);
    void CleanDestination(const std::wstring& destination, JobStatus* status);
    void CopyData(const std::wstring& source, const std::wstring& destination,
                       JobStatus* status);
    void AddData(JobStatus* status);
    std::wstring CommitData(JobStatus* status);
    bool HasChangesInRepository() const;
    void CreateReport(const std::wstring &commitId, JobStatus* status, FileBackupReport &report);

    void FixCRLFIssue();
    void SetGitUserToFixUtf8Warning();

    int GetRevisionCount() const;
    void CreateInitialReport(JobStatus* status, FileBackupReport& report);
    void CreateDifferentialReport(const std::wstring &commitId, JobStatus* status,
                                  FileBackupReport& report);

    std::wstring GetCommitId(const std::wstring& output);

    bool IsGitInstalled() const;

    bool IsCommitCodeOk(const int code) const;

    AbstractCopyFsBackupJob* PrepareCopy(const std::wstring& destination, JobStatus* status);
    void RunCopy(AbstractCopyFsBackupJob* copyJob,
                 const std::wstring& source, const std::wstring& destination, JobStatus* status);

    void LogDebugCommand(const std::wstring& title, const ConsoleJob &job);

    bool ConfigureGitRepository();
    bool SetupGitConfig(const std::wstring& configuration, const std::wstring& value);

    std::wstring CreateFilteredFileName(const std::wstring& name);

    void RunGitExport(const std::wstring& destination, JobStatus* status);

    bool forceRawCopy;
};

#endif // GITFSBACKUPJOB_H
