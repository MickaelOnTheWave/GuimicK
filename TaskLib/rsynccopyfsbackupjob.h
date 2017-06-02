#ifndef RSYNCCOPYFSBACKUPJOB_H
#define RSYNCCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class RsyncCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    RsyncCopyFsBackupJob();
    RsyncCopyFsBackupJob(const std::string& debugFilename);
    RsyncCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual std::string GetName();
    virtual AbstractJob* Clone();

    static bool IsAvailable();

    virtual bool IsInitialized(void);

protected:
    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::string&,
                              const std::string& output, ResultCollection& results);

private:
    std::string BuildSource(const std::string& originalSource) const;

};

#endif // RSYNCCOPYFSBACKUPJOB_H
