#include "gitcommontools.h"

#include <string>
#include <unistd.h>

using namespace std;

static const string changeDirError                      = "Failed accessing folder";

bool GitCommonTools::ChangeCurrentDir(const string &newDir,
                                    AbstractBackupJob::ResultCollection &statusList)
{
    const int errorValue = 999; // TODO : put real value there
    int returnValue = chdir(newDir.c_str());
    const bool ok =  (returnValue != errorValue);

    if (!ok)
    {
        JobStatus* status = new JobStatus(JobStatus::ERROR, changeDirError);
        statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));
    }

    return ok;
}
