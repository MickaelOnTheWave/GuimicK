#include "copyfsbackupjobtest.h"

#include <QTest>

#include "copyfsbackupjob.h"
#include "filetestutils.h"
#include "filetools.h"

using namespace std;

const string repository = "repository/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";

const string suitePrefix = "CopyFsBackup/";

CopyFsBackupJobTest::CopyFsBackupJobTest(const std::string &dataPrefix)
 : AbstractFsBackupJobTest(dataPrefix + suitePrefix)
{

}

CopyFsBackupJobTest::~CopyFsBackupJobTest()
{
}

void CopyFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
    LoadExternalDataSamples(true);
}

void CopyFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}

JobStatus *CopyFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    CopyFsBackupJob job;
    job.AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (remote)
        job.SetTargetRemote(sshUser, sshHost);
    else
        job.SetTargetLocal();

    JobStatus* status = job.Run();
    return status;
}
