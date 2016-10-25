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
    void testCreate_ExistingDestination();
    void testCreate_AllValid();

    void testUpdate_Added();
    void testUpdate_Removed();
    void testUpdate_Changed();
    void testUpdate_MixedChanges();

private:
    void CreateGitBackup(const QString& source, const QString& destination);
    void CheckGitJobReturnsError(const QString& description);
    void CheckGitJobReturnsOk(const QString& description);
    void CheckGitJobReturn(const int expectedStatus,
                           const unsigned long expectedReportFileCount,
                           const QString& description);
    void CheckFolderExistence(const QString &folder, const bool expectedExistence);

    const QString sourceRepository = "GitRepository";
    const QString destinationRepository = "GitDestRepository";
    const QString nonexistentRepository = "blablabla";
    const QString invalidRepository = "data/image.jpeg";
    const QString existingFolder = "data/existingFolder";

    JobStatus* currentStatus;
};

GitJobTest::GitJobTest()
{
}

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

void GitJobTest::testCreate_NoSource()
{
    const QString expectedDescription = "Source repository does not exist";

    CreateGitBackup(nonexistentRepository, destinationRepository);
    CheckGitJobReturnsError(expectedDescription);
    CheckFolderExistence(nonexistentRepository, false);
    CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_InvalidSource()
{
    const QString expectedDescription = "Invalid source repository";

    CreateGitBackup(invalidRepository, destinationRepository);
    CheckGitJobReturnsError(expectedDescription);
    CheckFolderExistence(invalidRepository, false);
    CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_ExistingDestination()
{
    const QString expectedDescription = "Destination folder already exists";

    CreateGitBackup(sourceRepository, existingFolder);
    CheckGitJobReturnsError(expectedDescription);
    CheckFolderExistence(sourceRepository, true);
    CheckFolderExistence(existingFolder, true);
}

void GitJobTest::testCreate_AllValid()
{
    const QString expectedDescription = "Destination folder already exists";

    CreateGitBackup(sourceRepository, destinationRepository);
    CheckGitJobReturnsOk(expectedDescription);
    CheckFolderExistence(sourceRepository, true);
    CheckFolderExistence(existingFolder, true);
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

void GitJobTest::CheckGitJobReturnsError(const QString& description)
{
    CheckGitJobReturn(JobStatus_ERROR, 0, description);
}

void GitJobTest::CheckGitJobReturnsOk(const QString &description)
{
    CheckGitJobReturn(JobStatus_OK, 1, description);
}

void GitJobTest::CheckGitJobReturn(const int expectedStatus,
                                   const unsigned long expectedReportFileCount,
                                   const QString &description)
{
    QCOMPARE(currentStatus->GetCode(), expectedStatus);
    QCOMPARE(currentStatus->GetDescription(), description.toStdString());

    std::vector<std::string> reportFiles;
    currentStatus->GetFilenames(reportFiles);
    QCOMPARE(reportFiles.size(), expectedReportFileCount);
}

void GitJobTest::CheckFolderExistence(const QString& folder,
                                      const bool expectedExistence)
{
    QCOMPARE(FileTools::FolderExists(folder.toStdString()), expectedExistence);
}

QTEST_APPLESS_MAIN(GitJobTest)

#include "tst_taskteststest.moc"
