#ifndef RAWCOPYFSBACKUPJOB_H
#define RAWCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class RawCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    RawCopyFsBackupJob();
    RawCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual AbstractJob* Clone();

    virtual std::string GetTypeName() const;

protected:
    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::string& destination,
                              const std::string& output, ResultCollection& results);

};

#endif // RAWCOPYFSBACKUPJOB_H
