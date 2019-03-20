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
   std::wstring destination;
   int folderIndex;
   int timeIndex;
};

class BackupRestoreTarget
{
public:
   std::wstring host;
   std::wstring user;
   std::wstring password;
};

class AbstractBackupJob : public AbstractJob
{
public:
    typedef std::pair<JobStatus*, FileBackupReport*> ResultEntry;
    typedef std::vector<ResultEntry> ResultCollection;
    typedef std::vector<std::pair<std::wstring, std::wstring> > BackupCollection;

    AbstractBackupJob(const std::wstring& _title);
    AbstractBackupJob(const AbstractBackupJob& other);
    virtual ~AbstractBackupJob();

    virtual bool InitializeFromClient(Client* client);
    virtual bool IsInitialized(void);

    virtual JobStatus* Run();

    virtual std::wstring GetTypeName() const = 0;

    JobStatus* RestoreBackupFromServer(const std::wstring& destination, const int folderIndex = 0);

    virtual JobStatus* RestoreBackupFromServer(const std::wstring& destination,
                                               const int folderIndex,
                                               const int timeIndex);

    virtual JobStatus* RestoreBackupFromClient( const BackupRestoreParameters& parameters,
                                                const BackupRestoreTarget& target);

    bool IsTargetLocal() const;
    void SetTargetRemote(const std::wstring& user = L"", const std::wstring& host = L"");
    void SetTargetLocal();
    void CopyTarget(const AbstractBackupJob& other);

    std::wstring GetRepository() const;
    virtual void SetRepository(const std::wstring& value);
    virtual void AddFolder(const std::wstring& source, const std::wstring& destination);
    void GetFolderList(std::vector<std::pair<std::wstring, std::wstring> >& folders);
    int GetFolderCount() const;
    void ClearFolderList(void);

    bool GetJoinReports() const;
    void SetJoinReports(const bool value);

protected:
    virtual bool Initialize();
    virtual void RunRepositoryBackup(const std::wstring& source,
                                     const std::wstring& destination,
                                     ResultCollection& results) = 0;
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

    virtual std::wstring CreateBackupSourcePath(const std::wstring& backupTag) const;

    std::wstring repository;
    BackupCollection folderList;
    JobExecutionTarget target;

    BackupStatusManager* statusManager;
    bool isDebugManagerParent;

private:
    virtual JobStatus* RestoreBackupFromServer(const std::wstring& source, const std::wstring& destination) = 0;
};
#endif // ABSTRACTBACKUPJOB_H
