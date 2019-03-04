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
          const std::string& jobName,
          ArchiveTool* _archiveTool
          );
    ArchiveFsBackupJob(const ArchiveFsBackupJob& other);
    virtual ~ArchiveFsBackupJob();

    static bool Restore(const std::string& backupFile,
                        const std::string& destination);

    std::string GetLocalDestination() const;
    void SetLocalDestination(const std::string& value);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);

    ArchiveTool* archiveTool;

private:
    virtual JobStatus* RestoreBackupFromServer(const std::string &source,
                                     const std::string &destination);

    bool UpdateBackupArchive(const std::string& folderToBackup,
                             const std::string& archiveName,
                             ResultCollection& results);
    bool RemovePreviousArchive(const std::string& destination,
                               ResultCollection& results);

    bool CopyBackupArchiveToDestination(const std::string& destination,
                                        ResultCollection& results);

    bool CleanBackupArchiveFromSource(ResultCollection& results);

    void AddStatusToResults(ResultCollection& results,
                            const int code, const std::string& message);

    std::string localDestination;
};

#endif // ARCHIVEFSBACKUPJOB_H
