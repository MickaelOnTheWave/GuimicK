#ifndef LINUXCOPYFSBACKUPJOB_H
#define LINUXCOPYFSBACKUPJOB_H

#include "rawcopyfsbackupjob.h"

class LinuxCopyFsBackupJob : public AbstractRawCopyFsBackupJob
{
public:
    LinuxCopyFsBackupJob();
    LinuxCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual AbstractJob* Clone();

protected:
    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::string& destination,
                              const std::string& output, ResultCollection& results);

};

#endif // LINUXCOPYFSBACKUPJOB_H
