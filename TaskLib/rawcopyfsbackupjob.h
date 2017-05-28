#ifndef RAWCOPYFSBACKUPJOB_H
#define RAWCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class RawCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    RawCopyFsBackupJob();
    RawCopyFsBackupJob(const std::string& debugFilename);
    RawCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();

protected:
    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::string& destination,
                              const std::string& output, ResultCollection& results);

};

#endif // RAWCOPYFSBACKUPJOB_H
