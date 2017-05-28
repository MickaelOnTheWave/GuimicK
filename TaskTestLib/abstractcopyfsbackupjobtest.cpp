#include "abstractcopyfsbackupjobtest.h"

#include <QTest>

#include "filetestutils.h"
#include "filetools.h"

using namespace std;

const string repository = "repository/";
const string sshUser = "mickael";
const string sshHost = "192.168.1.101";

AbstractCopyFsBackupJobTest::AbstractCopyFsBackupJobTest(const std::string &dataPrefix)
 : AbstractFsBackupJobTest(dataPrefix)
{

}

AbstractCopyFsBackupJobTest::~AbstractCopyFsBackupJobTest()
{
}

void AbstractCopyFsBackupJobTest::testRunBackup_data()
{
    LoadExternalDataSamples(false);
    LoadExternalDataSamples(true);
}

void AbstractCopyFsBackupJobTest::CheckBackedUpDataIsOk()
{
    FileTestUtils::CheckFoldersHaveSameContent(repository, currentSourceFolder);
}

JobStatus *AbstractCopyFsBackupJobTest::RunBackupJob()
{
    QFETCH(bool, remote);

    AbstractCopyFsBackupJob* job = CreateCopyJob();
    job->AddFolder(FileTools::BuildFullPath(currentSourceFolder), repository);
    if (remote)
        job->SetTargetRemote(sshUser, sshHost);
    else
        job->SetTargetLocal();

    JobStatus* status = job->Run();

    delete job;
    return status;
}
