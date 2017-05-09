#ifndef GITFSBACKUPJOB_H
#define GITFSBACKUPJOB_H

#include "abstractbackupjob.h"

#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"

class GitFsBackupJob : public AbstractBackupJob
{
public:
    GitFsBackupJob();
    GitFsBackupJob(const GitFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual JobStatus* Run();

    void SetOutputDebugInformation(const bool value);
    void SetJoinAllBackups(const bool value);

private:
    std::pair<JobStatus *, FileBackupReport *> RunRepositoryBackup(
                                    const std::string& source,
                                    const std::string& destination);
    JobStatus* CreateGlobalStatus(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses);
    void CreateGitRepository(const std::string& path, JobStatus* status);
    void CleanDestination(const std::string& destination, JobStatus* status);
    void CopyData(const std::string& source, const std::string& destination,
                       JobStatus* status);
    void AddData(JobStatus* status);
    std::string CommitData(JobStatus* status);
    void CreateReport(const std::string &commitId, JobStatus* status, FileBackupReport &report);

    int RunCopyCommand(const std::string& source, const std::string& destination);

    void FixCRLFIssue();
    void SetGitUserToFixUtf8Warning();

    int GetRevisionCount() const;
    void CreateInitialReport(JobStatus* status, FileBackupReport& report);
    void CreateDifferentialReport(const std::string &commitId, JobStatus* status,
                                  FileBackupReport& report);

    std::string GetCommitId(const std::string& output);

    bool IsGitInstalled() const;

    // TODO : refactor this list of pairs into dedicated object, with its methods.
    bool AreAllStatusesEqual(const std::vector<std::pair<JobStatus *, FileBackupReport *> > &statuses,
                             const int expectedCode);

    JobStatus* CreateSingleStatus(const std::vector<std::pair<JobStatus *,
                                  FileBackupReport *> > &statuses);
    JobStatus* CreateAllOkStatus(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses);
    JobStatus* CreateMixedStatus(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses);

    JobStatus* CreateJoinedStatus(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses);
    JobStatus* CreateSeparatedStatus(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses);

    std::string CreateStatusesDescription(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses);
    std::string CreateFoldersMiniDescription(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses);

    int ComputeSuccessCount(const std::vector<std::pair<JobStatus*,FileBackupReport*> > &statuses) const;

    std::string BuildRepositoryHeader(const std::string &name);
    std::string BuildFooter();

    JobDebugInformationManager debugManager;
    bool usingDebugInformation;
    bool joinRepositoriesReports;
};

#endif // GITFSBACKUPJOB_H
