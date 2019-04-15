#include "gitcommontools.h"

#include <string>
#include "pathtools.h"

using namespace std;

static const wstring changeDirError                      = L"Failed accessing folder";

bool GitCommonTools::ChangeCurrentDir(const wstring &newDir,
                                    AbstractBackupJob::ResultCollection &statusList)
{
   JobStatus* status = ChangeCurrentDir(newDir);
   if (!status->IsOk())
     statusList.push_back(make_pair(status, static_cast<FileBackupReport*>(NULL)));

   return status->IsOk();
}

JobStatus* GitCommonTools::ChangeCurrentDir(const wstring &newDir)
{
   const bool ok = PathTools::ChangeCurrentDir(newDir);
   return (ok) ? new JobStatus(JobStatus::Ok) : new JobStatus(JobStatus::Error, changeDirError);
}

