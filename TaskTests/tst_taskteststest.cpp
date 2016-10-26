#include <QString>
#include <QtTest>

#include <filetools.h>
#include <gitbackupjob.h>
#include <jobstatus.h>
#include <tools.h>

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
    void CreateDefaultBackup();
    void AddFiles(const QVector<QString>& list);
    void ChangeFiles(const QVector<QString>& list);
    void RemoveFiles(const QVector<QString>& list);
    void RunGitCommit(const QString& repository);

    void CheckGitJobReturnsError(const QString& description);
    void CheckGitJobReturnsOk(const QString& description);
    void CheckGitJobReturn(const int expectedStatus,
                           const unsigned long expectedReportFileCount,
                           const QString& description);
    void CheckFolderExistence(const QString &folder, const bool expectedExistence);
    void CheckRepositoryContent(const QString& folder,
                                const QVector<QString>& expectedFiles);

    const QString sourceRepository = "GitRepository";
    const QString destinationRepository = "GitDestRepository";
    const QString nonexistentRepository = "blablabla";
    const QString invalidRepository = "image.jpg";
    const QString existingFolder = "existingFolder";

    const QString messageNoSource = "Source repository does not exist";
    const QString messageInvalidSource = "Invalid source repository";

    const QVector<QString> defaultRepositoryContent = {"file0", "file1", "file2", "file3"};

    JobStatus* currentStatus = nullptr;
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
    CreateGitBackup(nonexistentRepository, destinationRepository);
    CheckGitJobReturnsError(messageNoSource);
    CheckFolderExistence(nonexistentRepository, false);
    CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_InvalidSource()
{
    CreateGitBackup(invalidRepository, destinationRepository);
    CheckGitJobReturnsError(messageInvalidSource);
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
    CheckFolderExistence(destinationRepository, true);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::testUpdate_Added()
{
    const QString expectedDescription = "";

    CreateDefaultBackup();

    QVector<QString> changedFiles = {"added0", "added1", "added2"};
    AddFiles(changedFiles);

    CheckGitJobReturnsOk(expectedDescription);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent+changedFiles);

}

void GitJobTest::testUpdate_Removed()
{
    const QString expectedDescription = "";

    CreateDefaultBackup();

    QVector<QString> changedFiles;
    changedFiles.push_back(defaultRepositoryContent.at(0));
    changedFiles.push_back(defaultRepositoryContent.at(1));
    RemoveFiles(changedFiles);

    CheckGitJobReturnsOk(expectedDescription);
    QVector<QString> expectedFiles = defaultRepositoryContent;
    expectedFiles.pop_front();
    expectedFiles.pop_front();
    CheckRepositoryContent(destinationRepository, expectedFiles);

}

void GitJobTest::testUpdate_Changed()
{
    const QString expectedDescription = "";

    CreateDefaultBackup();

    QVector<QString> changedFiles;
    changedFiles.push_back(defaultRepositoryContent.at(0));
    changedFiles.push_back(defaultRepositoryContent.at(1));
    ChangeFiles(changedFiles);

    CheckGitJobReturnsOk(expectedDescription);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::testUpdate_MixedChanges()
{
    const QString expectedDescription = "";

    CreateDefaultBackup();

    QVector<QString> addedFiles = {"added0", "added1", "added2"};
    AddFiles(addedFiles);

    QVector<QString> changedFiles = {defaultRepositoryContent.back()};
    ChangeFiles(changedFiles);

    QVector<QString> removedFiles = {defaultRepositoryContent.front()};
    RemoveFiles(removedFiles);

    CheckGitJobReturnsOk(expectedDescription);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::CreateGitBackup(const QString &source, const QString &destination)
{
    std::vector<std::pair<std::string, std::string> > repositoryList;
    repositoryList.push_back(make_pair(source.toStdString(), destination.toStdString()));
    GitBackupJob job(repositoryList);

    delete currentStatus;
    currentStatus = job.Run();
}

void GitJobTest::CreateDefaultBackup()
{
    CreateGitBackup(sourceRepository, destinationRepository);
}

void GitJobTest::AddFiles(const QVector<QString> &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        QString fullname = sourceRepository + "/" + list.at(i);
        Q_ASSERT(FileTools::CreateFile(fullname.toStdString(), 1000, true));
    }
    RunGitCommit(sourceRepository);
}

void GitJobTest::ChangeFiles(const QVector<QString> &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        QString fullname = sourceRepository + "/" + list.at(i);
        std::remove(fullname.toUtf8());
        Q_ASSERT(FileTools::CreateFile(fullname.toStdString(), 4000, true));
    }
    RunGitCommit(sourceRepository);
}

void GitJobTest::RemoveFiles(const QVector<QString> &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        QString fullname = sourceRepository + "/" + list.at(i);
        std::remove(fullname.toUtf8());
    }
    RunGitCommit(sourceRepository);
}

void GitJobTest::RunGitCommit(const QString &repository)
{
    QString fullCommand = "cd " + repository + "/ ";
    fullCommand += "&& git add -A && git commit -m \"auto\"";
    std::string output;
    Tools::RunExternalCommand(fullCommand.toStdString(), output);
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

void GitJobTest::CheckRepositoryContent(const QString &folder, const QVector<QString> &expectedFiles)
{
    QDir repositoryDir(folder);
    QStringList filesInRepository = repositoryDir.entryList();
    Q_ASSERT(filesInRepository.size() == expectedFiles.size()+2);
    Q_ASSERT(filesInRepository.contains("."));
    Q_ASSERT(filesInRepository.contains(".."));

    for (int i=0; i<expectedFiles.size(); ++i)
    {
        QString fullname = folder + "/" + expectedFiles.at(i);
        Q_ASSERT(filesInRepository.contains(fullname));
    }
}

QTEST_APPLESS_MAIN(GitJobTest)

#include "tst_taskteststest.moc"
