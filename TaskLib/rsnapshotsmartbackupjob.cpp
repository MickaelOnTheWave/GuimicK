#include "rsnapshotsmartbackupjob.h"

#include <cstdio>

using namespace std;

RsnapshotSmartBackupJob::RsnapshotSmartBackupJob(const string &_backupRepositoryPath,
                                                 const string &_rsnapshotConfFile)
    : RsnapshotBackupJob(_backupRepositoryPath, _rsnapshotConfFile)
{

}

RsnapshotSmartBackupJob::~RsnapshotSmartBackupJob()
{
    remove(configurationFile.c_str());
}



