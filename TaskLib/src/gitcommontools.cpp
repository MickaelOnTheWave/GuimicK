#include "gitcommontools.h"

#include <string>

#ifdef _MSC_VER
   #include <direct.h>
   #define chdir _chdir
#else
   #include <unistd.h>
#endif

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
   int returnValue = _wchdir(newDir.c_str());
   const bool ok = (returnValue == 0);
   return (ok) ? new JobStatus(JobStatus::Ok) : new JobStatus(JobStatus::Error, changeDirError);
}

