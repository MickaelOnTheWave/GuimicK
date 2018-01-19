#ifndef ABSTRACTBACKUPJOB_H
#define ABSTRACTBACKUPJOB_H

#include "abstractjob.h"

#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"

class BackupStatusManager;

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

    JobStatus* RestoreBackup(const std::string& destination, const int repositoryIndex = 0);

    bool IsTargetLocal() const;
    void SetTargetRemote(const std::string& user = "", const std::string& host = "");
    void SetTargetLocal();
    void CopyTarget(const AbstractBackupJob& other);

    std::string GetRepository() const;
    virtual void SetRepository(const std::string& value);
    void AddFolder(const std::string& source, const std::string& destination);
    void GetFolderList(std::vector<std::pair<std::string, std::string> >& folders);
    void ClearFolderList(void);

    bool GetJoinReports() const;
    void SetJoinReports(const bool value);

protected:
    virtual bool Initialize();
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results) = 0;
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

    std::string repository;
    BackupCollection folderList;

    std::string sshUser;
    std::string sshHost;
    bool isTargetLocal;

    BackupStatusManager* statusManager;
    bool isDebugManagerParent;

private:
    virtual JobStatus* RestoreBackup(const std::string& source, const std::string& destination) = 0;
    bool IsRemoteTargetConsistent() const;
};
#endif // ABSTRACTBACKUPJOB_H
