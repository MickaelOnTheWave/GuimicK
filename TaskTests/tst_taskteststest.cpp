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
    void testCreate_InvalidSource();
    void testCreate_AllValid();

    void testUpdate_NoChanges();
    void testUpdate_Added();
    void testUpdate_Removed();
    void testUpdate_Changed();
    void testUpdate_MixedChanges();

private:
    void InitializeTestData();
    void RemoveAll(const QString &folder);
    void CreateDefaultData();
    void StartGitRepository();

    void RunGitBackup(const QString& source, const QString& destination);
    void CreateDefaultBackup();
    void AddFiles(const QVector<QString>& list, const size_t size = 1000);
    void ChangeFiles(const QVector<QString>& list);
    void RemoveFiles(const QVector<QString>& list);
    void RunGitCommit(const QString& repository);

    void RunGitBackup();

    void CheckGitJobReturnsError(const QString& description);
    void CheckGitJobReturnsOk(const QString& description);
    void CheckGitJobReturn(const int expectedStatus,
                           const unsigned long expectedReportFileCount,
                           const QString& description);
    void CheckFolderExistence(const QString &folder, const bool expectedExistence);
    void CheckRepositoryContent(const QString& folder,
                                const QVector<QString>& expectedFiles);
    QString BuildDescriptionString(const int added, const int modified, const int deleted);

    const QString sourceRepository = "GitRepository";
    const QString destinationRepository = "GitDestRepository";
    const QString invalidRepository = "blablabla";
    const QString existingFolder = "existingFolder";

    const QString messageInvalidSource = "Invalid source repository";
    const QString messageRepositoryCreationOk = "Repository cloned successfully";
    const QString messageRepositoryUpdateOk = "Repository successfully updated";

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

void GitJobTest::testCreate_InvalidSource()
{
    InitializeTestData();
    RunGitBackup(invalidRepository, destinationRepository);
    CheckGitJobReturnsError(messageInvalidSource);
    CheckFolderExistence(invalidRepository, false);
    CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_AllValid()
{
    InitializeTestData();
    RunGitBackup(sourceRepository, destinationRepository);
    CheckGitJobReturn(JobStatus_OK, 0, messageRepositoryCreationOk);
    CheckFolderExistence(sourceRepository, true);
    CheckFolderExistence(destinationRepository, true);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::testUpdate_NoChanges()
{
    InitializeTestData();
    CreateDefaultBackup();
    RunGitBackup(sourceRepository, destinationRepository);
    CheckGitJobReturnsOk(BuildDescriptionString(0,0,0));
    CheckFolderExistence(sourceRepository, true);
    CheckFolderExistence(destinationRepository, true);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::testUpdate_Added()
{
    InitializeTestData();
    CreateDefaultBackup();

    QVector<QString> changedFiles = {"added0", "added1", "added2"};
    AddFiles(changedFiles);

    RunGitBackup(sourceRepository, destinationRepository);

    CheckGitJobReturnsOk(BuildDescriptionString(3,0,0));
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent+changedFiles);
}

void GitJobTest::testUpdate_Removed()
{
    InitializeTestData();
    CreateDefaultBackup();

    QVector<QString> changedFiles;
    changedFiles.push_back(defaultRepositoryContent.at(0));
    changedFiles.push_back(defaultRepositoryContent.at(1));
    RemoveFiles(changedFiles);

    RunGitBackup(sourceRepository, destinationRepository);

    CheckGitJobReturnsOk(BuildDescriptionString(0,0,2));
    QVector<QString> expectedFiles = defaultRepositoryContent;
    expectedFiles.pop_front();
    expectedFiles.pop_front();
    CheckRepositoryContent(destinationRepository, expectedFiles);

}

void GitJobTest::testUpdate_Changed()
{
    InitializeTestData();
    CreateDefaultBackup();

    QVector<QString> changedFiles;
    changedFiles.push_back(defaultRepositoryContent.at(0));
    changedFiles.push_back(defaultRepositoryContent.at(1));
    ChangeFiles(changedFiles);

    RunGitBackup(sourceRepository, destinationRepository);

    CheckGitJobReturnsOk(BuildDescriptionString(0,2,0));
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::testUpdate_MixedChanges()
{
    InitializeTestData();
    CreateDefaultBackup();

    QVector<QString> addedFiles = {"added0", "added1", "added2"};
    AddFiles(addedFiles, 10000);

    QVector<QString> changedFiles = {defaultRepositoryContent.back()};
    ChangeFiles(changedFiles);

    QVector<QString> removedFiles = {defaultRepositoryContent.front()};
    RemoveFiles(removedFiles);

    RunGitBackup(sourceRepository, destinationRepository);

    CheckGitJobReturnsOk(BuildDescriptionString(3,1,1));
    QVector<QString> expectedFiles = defaultRepositoryContent;
    expectedFiles.pop_front();
    expectedFiles.push_front("added2");
    expectedFiles.push_front("added1");
    expectedFiles.push_front("added0");
    CheckRepositoryContent(destinationRepository, expectedFiles);
}

void GitJobTest::InitializeTestData()
{
    delete currentStatus;
    currentStatus = nullptr;

    RemoveAll(sourceRepository);
    RemoveAll(destinationRepository);
    CreateDefaultData();
    StartGitRepository();
}

void GitJobTest::RemoveAll(const QString& folder)
{
    std::string removeAllCommand("rm -Rf ");
    removeAllCommand += folder.toStdString();

    std::string unusedOutput;
    Tools::RunExternalCommand(removeAllCommand, unusedOutput);
}

void GitJobTest::CreateDefaultData()
{
    std::string stdSourceRepository = sourceRepository.toStdString();
    FileTools::CreateFolder(stdSourceRepository);
    for (int i=0; i<defaultRepositoryContent.size(); ++i)
    {
        std::string filename = stdSourceRepository + "/" +
                               defaultRepositoryContent.at(i).toStdString();
        FileTools::CreateFile(filename, 1000, true);
    }
}

void GitJobTest::StartGitRepository()
{
    std::string stdSourceRepository = sourceRepository.toStdString();
    std::string gitCreationCommand("git init ");
    gitCreationCommand += stdSourceRepository + " && cd " + stdSourceRepository + " && ";
    gitCreationCommand += "git add -A && git commit -m \"initial data\"";

    std::string unusedOutput;
    Tools::RunExternalCommand(gitCreationCommand, unusedOutput);
}

void GitJobTest::RunGitBackup(const QString &source, const QString &destination)
{
    std::vector<std::pair<std::string, std::string> > repositoryList;
    repositoryList.push_back(make_pair(source.toStdString(), destination.toStdString()));
    GitBackupJob job(repositoryList);
    currentStatus = job.Run();
}

void GitJobTest::CreateDefaultBackup()
{
    std::string stdSourceRepository = sourceRepository.toStdString();
    std::string command("git clone ");
    command += sourceRepository.toStdString() + " " + destinationRepository.toStdString();
    command += " 2>&1 > /dev/null";
    std::string unusedOutput;
    Tools::RunExternalCommand(command, unusedOutput);
}

void GitJobTest::AddFiles(const QVector<QString> &list, const size_t size)
{
    for (int i=0; i<list.size(); ++i)
    {
        QString fullname = sourceRepository + "/" + list.at(i);
        Q_ASSERT(FileTools::CreateFile(fullname.toStdString(), size, true));
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
    QDir repositoryDir = QDir::currentPath();
    repositoryDir.cd(folder);
    QStringList filesInRepository = repositoryDir.entryList();
    QCOMPARE(filesInRepository.size(), expectedFiles.size()+2);
    Q_ASSERT(filesInRepository.contains("."));
    Q_ASSERT(filesInRepository.contains(".."));

    for (int i=0; i<expectedFiles.size(); ++i)
        Q_ASSERT(filesInRepository.contains(expectedFiles.at(i)));
}

QString GitJobTest::BuildDescriptionString(const int added, const int modified, const int deleted)
{
    QString description = QString::number(added) + " added, ";
    description += QString::number(modified) + " modified and ";
    description += QString::number(deleted) + " removed.";
    return description;
}

QTEST_APPLESS_MAIN(GitJobTest)

#include "tst_taskteststest.moc"
