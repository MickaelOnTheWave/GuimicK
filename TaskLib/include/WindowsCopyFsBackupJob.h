#ifndef WINDOWSCOPYFSBACKUPJOB_H
#define WINDOWSCOPYFSBACKUPJOB_H

#include "rawcopyfsbackupjob.h"

class WindowsCopyFsBackupJob : public AbstractRawCopyFsBackupJob
{
public:
    WindowsCopyFsBackupJob();
    WindowsCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual AbstractJob* Clone();

protected:
    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::string& destination,
                              const std::string& output, ResultCollection& results);

private:
   FileBackupReport* CreateBackupReport(const std::string& output) const;

};

#endif // WINDOWSCOPYFSBACKUPJOB_H
