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
    virtual void PrepareCopyCommand(const std::wstring &source,
                                    const std::wstring &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::wstring& destination,
                              const std::wstring& output, ResultCollection& results);

};

#endif // LINUXCOPYFSBACKUPJOB_H
