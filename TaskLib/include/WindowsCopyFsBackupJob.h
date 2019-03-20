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
    virtual void PrepareCopyCommand(const std::wstring &source,
                                    const std::wstring &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::wstring& destination,
                              const std::wstring& output, ResultCollection& results);

private:
   FileBackupReport* CreateBackupReport(const std::wstring& output) const;

};

#endif // WINDOWSCOPYFSBACKUPJOB_H
