#ifndef GITCOMMONTOOLS_H
#define GITCOMMONTOOLS_H

#include "abstractbackupjob.h"

class GitCommonTools
{
public:

    static bool ChangeCurrentDir(const std::string &newDir,
                                 AbstractBackupJob::ResultCollection &statusList);
};

#endif // GITCOMMONTOOLS_H
