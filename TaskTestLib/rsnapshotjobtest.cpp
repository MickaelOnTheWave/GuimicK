#include "rsnapshotjobtest.h"

#include <QTest>

#include <unistd.h>

#include "filetools.h"
#include "rsnapshotbackupjob.h"
#include "tools.h"

using namespace std;

const string templateConfigurationFile = "rsnapshot.conf";
const string defaultConfigurationFile = "backuptests.conf";

RsnapshotJobTest::RsnapshotJobTest()
{
    repository = BuildFullPathOnCurrentDir("rsnapshotroot");
}

void RsnapshotJobTest::init()
{
    string unusedOutput;
    string removeCommand = string("rm ") + defaultConfigurationFile;
    Tools::RunExternalCommandToBuffer(removeCommand, unusedOutput, true);
}

void RsnapshotJobTest::cleanup()
{

}

void RsnapshotJobTest::testCreate_InvalidSource()
{
    const QString invalidFolder = "InvalidFolder";
    CreateConfigurationFile(invalidFolder);
    JobStatus* status = RunRsnapshotJob();
    CheckStatusIsInvalid(status);
}

void RsnapshotJobTest::testCreate_AllValid()
{
    const QString folder = "ex1_dest";
    CreateConfigurationFile(folder);
    JobStatus* status = RunRsnapshotJob();

    QFAIL("Implementation not finished.");
}

void RsnapshotJobTest::CreateConfigurationFile(const QString &folder)
{
    std::string contents;
    bool ok = FileTools::GetTextFileContent(templateConfigurationFile, contents);
    QCOMPARE(ok, true);

    contents += string("\nsnapshot_root\t") + repository + "\n";
    contents += string("\nbackup\t") + BuildFullPathOnCurrentDir(folder.toStdString()) + "/\tdest/\n";

    FileTools::WriteBufferToFile(defaultConfigurationFile, contents);
}

JobStatus *RsnapshotJobTest::RunRsnapshotJob()
{
    RsnapshotBackupJob job(repository, defaultConfigurationFile);
    return job.Run();
}

void RsnapshotJobTest::CheckStatusIsInvalid(JobStatus *status)
{
    QFAIL("Not implemented yet.");
}

std::string RsnapshotJobTest::BuildFullPathOnCurrentDir(const std::string& name)
{
    char unusedBuffer[1024];
    char* currentPath = getcwd(unusedBuffer, 1024);
    return string(currentPath) + "/" + name + "/";
}
