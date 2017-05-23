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

    /**
     * @return command return code.
     */
    int RunOnParameters(const std::string &source,
                         const std::string &destination);

protected:
    virtual void RunRepositoryBackup(const std::string& source,
                                    const std::string& destination,
                                     ResultCollection& results);
    virtual JobStatus* CreateGlobalStatus(const ResultCollection& results);
private:
};

#endif // COPYFSBACKUPJOB_H
