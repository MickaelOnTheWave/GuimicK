#include "gitcommontools.h"

#include <string>

#ifdef _MSC_VER
   #include <direct.h>
   #define chdir _chdir
#else
   #include <unistd.h>
#endif

using namespace std;

static const string changeDirError                      = "Failed accessing folder";

bool GitCommonTools::ChangeCurrentDir(const string &newDir,
                                    AbstractBackupJob::ResultCollection &statusList)
{
   JobStatus* status = ChangeCurrentDir(newDir);
   if (!status->IsOk())
     statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));

   return status->IsOk();
}

JobStatus* GitCommonTools::ChangeCurrentDir(const string &newDir)
{
   int returnValue = chdir(newDir.c_str());
   const bool ok = (returnValue == 0);
   return (ok) ? new JobStatus(JobStatus::Ok) : new JobStatus(JobStatus::Error, changeDirError);
}

