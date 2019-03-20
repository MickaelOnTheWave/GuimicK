#ifndef ARCHIVEFSBACKUPJOB_H
#define ARCHIVEFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "AbstractConsoleJob.h"
#include "archivetool.h"
#include "backupstatusmanager.h"


// TODO : check that the job works with multiple folders
class ArchiveFsBackupJob : public AbstractBackupJob
{
public:
    ArchiveFsBackupJob(
          const std::wstring& jobName,
          ArchiveTool* _archiveTool
          );
    ArchiveFsBackupJob(const ArchiveFsBackupJob& other);
    virtual ~ArchiveFsBackupJob();

    static bool Restore(const std::wstring& backupFile,
                        const std::wstring& destination);

    std::wstring GetLocalDestination() const;
    void SetLocalDestination(const std::wstring& value);

protected:
    virtual void RunRepositoryBackup(const std::wstring& source,
                                     const std::wstring& destination,
                                     ResultCollection& results);

    ArchiveTool* archiveTool;

private:
    virtual JobStatus* RestoreBackupFromServer(const std::wstring &source,
                                     const std::wstring &destination);

    bool UpdateBackupArchive(const std::wstring& folderToBackup,
                             const std::wstring& archiveName,
                             ResultCollection& results);
    bool RemovePreviousArchive(const std::wstring& destination,
                               ResultCollection& results);

    bool CopyBackupArchiveToDestination(const std::wstring& destination,
                                        ResultCollection& results);

    bool CleanBackupArchiveFromSource(ResultCollection& results);

    void AddStatusToResults(ResultCollection& results,
                            const int code, const std::wstring& message);

    std::wstring localDestination;
};

#endif // ARCHIVEFSBACKUPJOB_H
