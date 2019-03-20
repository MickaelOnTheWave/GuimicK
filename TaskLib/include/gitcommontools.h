#ifndef GITCOMMONTOOLS_H
#define GITCOMMONTOOLS_H

#include "abstractbackupjob.h"

class GitCommonTools
{
public:

    static bool ChangeCurrentDir(const std::wstring &newDir,
                                 AbstractBackupJob::ResultCollection &statusList);

    static JobStatus* ChangeCurrentDir(const std::wstring &newDir);
};

#endif // GITCOMMONTOOLS_H
