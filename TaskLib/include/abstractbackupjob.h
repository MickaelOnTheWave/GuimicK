#ifndef ABSTRACTBACKUPJOB_H
#define ABSTRACTBACKUPJOB_H

#include "abstractjob.h"

#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"
#include "jobexecutiontarget.h"

class BackupStatusManager;

class BackupRestoreParameters
{
public:
   std::string destination;
   int folderIndex;
   int timeIndex;
};

class BackupRestoreTarget
{
public:
   std::string host;
   std::string user;
   std::string password;
};

class AbstractBackupJob : public AbstractJob
{
public:
    typedef std::pair<JobStatus*, FileBackupReport*> ResultEntry;
    typedef std::vector<ResultEntry> ResultCollection;
    typedef std::vector<std::pair<std::string, std::string> > BackupCollection;

    AbstractBackupJob(const std::string& _title);
    AbstractBackupJob(const AbstractBackupJob& other);
    virtual ~AbstractBackupJob();

    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    virtual std::string GetTypeName() const = 0;

    JobStatus* RestoreBackupFromServer(const std::string& destination, const int folderIndex = 0);

    virtual JobStatus* RestoreBackupFromServer(const std::string& destination,
                                               const int folderIndex,
                                               const int timeIndex);

    virtual JobStatus* RestoreBackupFromClient( const BackupRestoreParameters& parameters,
                                                const BackupRestoreTarget& target);

    bool IsTargetLocal() const;
    void SetTargetRemote(const std::string& user = "", const std::string& host = "");
    void SetTargetLocal();
    void CopyTarget(const AbstractBackupJob& other);

    std::string GetRepository() const;
    virtual void SetRepository(const std::string& value);
    virtual void AddFolder(const std::string& source, const std::string& destination);
    void GetFolderList(std::vector<std::pair<std::string, std::string> >& folders);
    int GetFolderCount() const;
    void ClearFolderList(void);

    bool GetJoinReports() const;
    void SetJoinReports(const bool value);

protected:
    virtual bool Initialize();
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results) = 0;
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

    virtual std::string CreateBackupSourcePath(const std::string& backupTag) const;

    std::string repository;
    BackupCollection folderList;
    JobExecutionTarget target;

    BackupStatusManager* statusManager;
    bool isDebugManagerParent;

private:
    virtual JobStatus* RestoreBackupFromServer(const std::string& source, const std::string& destination) = 0;
};
#endif // ABSTRACTBACKUPJOB_H
