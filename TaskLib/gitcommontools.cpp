#include "gitcommontools.h"

#include <string>
#include <unistd.h>

using namespace std;

static const string changeDirError                      = "Failed accessing folder";

bool GitCommonTools::ChangeCurrentDir(const string &newDir,
                                    AbstractBackupJob::ResultCollection &statusList)
{
    int returnValue = chdir(newDir.c_str());
    const bool ok =  (returnValue == 0);

    if (!ok)
    {
        JobStatus* status = new JobStatus(JobStatus::ERROR, changeDirError);
        statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));
    }

    return ok;
}
