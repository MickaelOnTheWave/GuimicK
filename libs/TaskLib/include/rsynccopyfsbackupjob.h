#ifndef RSYNCCOPYFSBACKUPJOB_H
#define RSYNCCOPYFSBACKUPJOB_H

#include "abstractcopyfsbackupjob.h"

class RsyncCopyFsBackupJob : public AbstractCopyFsBackupJob
{
public:
    RsyncCopyFsBackupJob();
    RsyncCopyFsBackupJob(const AbstractCopyFsBackupJob& other);

    virtual AbstractJob* Clone();

    virtual std::wstring GetTypeName() const;

    static bool IsAvailable();

    virtual bool IsInitialized(void);

    void GetExclusionList(std::vector<std::wstring>& exclusions);
    void AddToExclusions(const std::wstring& item);
    void ClearExclusionList();

protected:
    virtual void PrepareCopyCommand(const std::wstring &source,
                                    const std::wstring &destination,
                                    ConsoleJob& commandJob);

    virtual void CreateReport(const std::wstring&,
                              const std::wstring& output, ResultCollection& results);

private:
    std::wstring BuildSource(const std::wstring& originalSource) const;
    std::wstring BuildExclusions() const;

    std::vector<std::wstring> itemsToExclude;
};

#endif // RSYNCCOPYFSBACKUPJOB_H
