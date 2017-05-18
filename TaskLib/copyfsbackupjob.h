#ifndef COPYFSBACKUPJOB_H
#define COPYFSBACKUPJOB_H

#include "abstractbackupjob.h"
#include "filebackupreport.h"
#include "jobdebuginformationmanager.h"

class CopyFsBackupJob : public AbstractBackupJob
{
public:
    CopyFsBackupJob();
    CopyFsBackupJob(const CopyFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();
    virtual JobStatus* Run();

    /**
     * @return command return code.
     */
    int RunOnParameters(const std::string &source,
                         const std::string &destination);

protected:
    std::pair<JobStatus*, FileBackupReport*> RunRepositoryBackup(
                                   const std::string& source,
                                   const std::string& destination);
private:
    JobDebugInformationManager debugManager;
    bool usingDebugInformation;
};

#endif // COPYFSBACKUPJOB_H
