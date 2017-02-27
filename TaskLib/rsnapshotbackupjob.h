#ifndef BACKUPJOB_H
#define BACKUPJOB_H

#include "abstractjob.h"
#include "consolejob.h"
#include <rsnapshotreportparser.h>

class RsnapshotBackupJob : public AbstractJob
{
public:
    RsnapshotBackupJob(const std::string& _backupRepositoryPath = "",
              const std::string& _rsnapshotConfFile = "");
	~RsnapshotBackupJob();

	virtual std::string GetName();

    virtual AbstractJob* Clone();

    void SetRepositoryPath(const std::string& path);

    void SeConfigurationFile(const std::string& file);

    virtual bool InitializeFromClient(Client*);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

    void SetOutputDebugInformation(const bool value);

    /**
     * On automated testing, if there is no wait after
     * Rsnapshot run, data is not retrieved normally.
     */
    void SetWaitAfterRun(const bool value);

private:
	ConsoleJob* backupCommand;
	ConsoleJob* reportCommand;
    std::string backupRepositoryPath;
    std::string configurationFile;
    bool showDebugOutput;
    bool waitAfterRun;
};

#endif // BACKUPJOB_H
