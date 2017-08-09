#ifndef BACKUPJOB_H
#define BACKUPJOB_H

#include "abstractbackupjob.h"
#include "consolejob.h"
#include <rsnapshotreportparser.h>

class RsnapshotBackupJob : public AbstractBackupJob
{
public:
    RsnapshotBackupJob(const std::string& _backupRepositoryPath = "",
              const std::string& _rsnapshotConfFile = "");
    virtual ~RsnapshotBackupJob();

	virtual std::string GetName();

    virtual AbstractJob* Clone();

    void SetRepositoryPath(const std::string& path);

    void SeConfigurationFile(const std::string& file);

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

    /**
     * On automated testing, if there is no wait after
     * Rsnapshot run, data is not retrieved normally.
     */
    void SetWaitAfterRun(const bool value);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                     const std::string& destination,
                                     ResultCollection& results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);

    std::string configurationFile;

private:
	ConsoleJob* backupCommand;
	ConsoleJob* reportCommand;
    std::string backupRepositoryPath;
    bool waitAfterRun;
};

#endif // BACKUPJOB_H
