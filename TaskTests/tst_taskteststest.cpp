#include <QString>
#include <QtTest>

#include <filetools.h>
#include <jobstatus.h>

class GitJobTest : public QObject
{
    Q_OBJECT

public:
    GitJobTest();

private Q_SLOTS:
    void testCreate_NoSource();
    void testCreate_InvalidSource();
    void testCreate_InvalidDestination();
    void testCreate_ExistingDestination();
    void testCreate_AllValid();

    void testUpdate_Added();
    void testUpdate_Removed();
    void testUpdate_Changed();
    void testUpdate_MixedChanges();

private:
    void CreateGitBackup(const QString& source, const QString& destination);

    const std::string sourceRepository = "GitRepository";
    const std::string destinationRepository = "GitDestRepository";

    JobStatus* currentStatus;
};

GitJobTest::GitJobTest()
{
}

// To force GCC to include definition into object file, so that it is possible
// to use them externally.
const int JobStatus::NOT_EXECUTED;
//const int JobStatus::OK;
//const int JobStatus::OK_WITH_WARNINGS;
//const int JobStatus::ERROR;

void GitJobTest::testCreate_NoSource()
{
    const QString destination = "destNonExistentPath";

    CreateGitBackup("nonexistentPath", destination);

    QCOMPARE(currentStatus->GetCode(), JobStatus::ERROR);
    QCOMPARE(currentStatus->GetDescription().c_str(), "Source repository does not exist");

    std::vector<std::string> reportFiles;
    currentStatus->GetFilenames(reportFiles);
    QCOMPARE(reportFiles.size(), 0ul);

    QCOMPARE(FileTools::FolderExists(destination.toStdString()), false);
}

void GitJobTest::testCreate_InvalidSource()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testCreate_InvalidDestination()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testCreate_ExistingDestination()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testCreate_AllValid()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testUpdate_Added()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testUpdate_Removed()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testUpdate_Changed()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::testUpdate_MixedChanges()
{
    QFAIL("Not implemented yet");
}

void GitJobTest::CreateGitBackup(const QString &source, const QString &destination)
{
    // TODO : Run job, get its status and its report data.
}

QTEST_APPLESS_MAIN(GitJobTest)

#include "tst_taskteststest.moc"
