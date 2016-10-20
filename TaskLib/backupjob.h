#ifndef BACKUPJOB_H
#define BACKUPJOB_H

#include "abstractjob.h"
#include "consolejob.h"
#include <rsnapshotreportparser.h>

class BackupJob : public AbstractJob
{
public:
    BackupJob(const std::string& _backupRepositoryPath = "",
              const std::string& _rsnapshotConfFile = "");
	~BackupJob();

	virtual std::string GetName();

    void SetRepositoryPath(const std::string& path);

    void SetRsnapshotConfFile(const std::string& file);

    virtual bool InitializeFromClient(Client*);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

protected:
	ConsoleJob* backupCommand;
	ConsoleJob* reportCommand;
    std::string backupRepositoryPath;
    std::string rsnapshotConfFile;

};

#endif // BACKUPJOB_H
