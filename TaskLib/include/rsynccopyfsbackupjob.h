#ifndef RSYNCCOPYFSBACKUPJOB_H
#define RSYNCCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class RsyncCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    RsyncCopyFsBackupJob();
    RsyncCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual AbstractJob* Clone();

    virtual std::string GetTypeName() const;

    static bool IsAvailable();

    virtual bool IsInitialized(void);

    void GetExclusionList(std::vector<std::string>& exclusions);
    void AddToExclusions(const std::string& item);
    void ClearExclusionList();

protected:
    virtual void PrepareCopyCommand(const std::string &source,
                                    const std::string &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::string&,
                              const std::string& output, ResultCollection& results);

private:
    std::string BuildSource(const std::string& originalSource) const;
    std::string BuildExclusions() const;

    std::vector<std::string> itemsToExclude;
};

#endif // RSYNCCOPYFSBACKUPJOB_H
