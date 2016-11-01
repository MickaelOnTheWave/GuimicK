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
    void init();
    void cleanup();

    void testCreate_InvalidSource();
    void testCreate_AllValid();

    void testUpdate_data();
    void testUpdate();
    void testUpdate_MultipleRepositories();

private:
    void RemoveAll(const QString &folder);
    void CreateDefaultData();
    void StartGitRepository();

    void RunGitBackup(const QString& source, const QString& destination);
    void CreateDefaultBackup();
    void AddFiles(const QStringList &list, const size_t size = 1000);
    void ChangeFiles(const QStringList& list);
    void RemoveFiles(const QStringList &list);
    void RunGitCommit(const QString& repository);

    void RunGitBackup();

    void CheckGitJobReturnsError(const QString& description);
    void CheckGitJobReturnsOk(const QString& description);
    void CheckGitJobReturn(const int expectedStatus,
                           const unsigned long expectedReportFileCount,
                           const QString& description);
    void CheckFolderExistence(const QString &folder, const bool expectedExistence);
    void CheckRepositoryContent(const QString& folder,
                                const QStringList &expectedFiles);
    QString BuildDescriptionString(const int added, const int modified, const int deleted);

    void UpdateSourceRepository(const QStringList& added,
                                const QStringList& modified,
                                const QStringList& removed);
    QStringList CreatedExpectedDestinationRepositoryContent(const QStringList& added,
                                                            const QStringList& removed);

    const QString sourceRepository = "GitRepository";
    const QString destinationRepository = "GitDestRepository";
    const QString invalidRepository = "blablabla";
    const QString existingFolder = "existingFolder";

    const QString messageInvalidSource = "Invalid source repository";
    const QString messageRepositoryCreationOk = "Repository cloned successfully";
    const QString messageRepositoryUpdateOk = "Repository successfully updated";

    const QStringList defaultRepositoryContent = {"file0", "file1", "file2", "file3"};

    JobStatus* currentStatus = nullptr;
};

GitJobTest::GitJobTest()
{
}

void GitJobTest::init()
{
    CreateDefaultData();
    StartGitRepository();
}

void GitJobTest::cleanup()
{
    delete currentStatus;
    currentStatus = nullptr;

    RemoveAll(sourceRepository);
    RemoveAll(destinationRepository);
}

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

void GitJobTest::testCreate_InvalidSource()
{
    RunGitBackup(invalidRepository, destinationRepository);
    CheckGitJobReturnsError(messageInvalidSource);
    CheckFolderExistence(invalidRepository, false);
    CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_AllValid()
{
    RunGitBackup(sourceRepository, destinationRepository);
    CheckGitJobReturn(JobStatus_OK, 0, messageRepositoryCreationOk);
    CheckFolderExistence(sourceRepository, true);
    CheckFolderExistence(destinationRepository, true);
    CheckRepositoryContent(destinationRepository, defaultRepositoryContent);
}

void GitJobTest::testUpdate_data()
{
    QTest::addColumn<QStringList>("added");
    QTest::addColumn<QStringList>("modified");
    QTest::addColumn<QStringList>("removed");

    QTest::newRow("No changes") << QStringList() << QStringList() << QStringList();
    QTest::newRow("Added 3") << QStringList({"added0","added1","added2"}) << QStringList() << QStringList();
    QTest::newRow("Modified 2") << QStringList() << QStringList({"file2","file3"}) << QStringList();
    QTest::newRow("Removed 2") << QStringList() << QStringList() << QStringList({"file0","file1"});
    QTest::newRow("Mixed changes") << QStringList({"myfirst add","my other add", "third add", "last add"})
                                   << QStringList({"file0","file1"})
                                   << QStringList({"file2"});
}

void GitJobTest::testUpdate()
{
    QFETCH(QStringList, added);
    QFETCH(QStringList, modified);
    QFETCH(QStringList, removed);

    CreateDefaultBackup();

    UpdateSourceRepository(added, modified, removed);

    RunGitBackup(sourceRepository, destinationRepository);

    CheckGitJobReturnsOk(BuildDescriptionString(added.size(),
                                                modified.size(),
                                                removed.size() ));


    QStringList expectedFileList = CreatedExpectedDestinationRepositoryContent(added, removed);

    CheckRepositoryContent(destinationRepository, expectedFileList);
}

void GitJobTest::testUpdate_MultipleRepositories()
{
    QFAIL("Implement");
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

void GitJobTest::AddFiles(const QStringList &list, const size_t size)
{
    for (int i=0; i<list.size(); ++i)
    {
        QString fullname = sourceRepository + "/" + list.at(i);
        Q_ASSERT(FileTools::CreateFile(fullname.toStdString(), size, true));
    }
    RunGitCommit(sourceRepository);
}

void GitJobTest::ChangeFiles(const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        QString fullname = sourceRepository + "/" + list.at(i);
        std::remove(fullname.toUtf8());
        Q_ASSERT(FileTools::CreateFile(fullname.toStdString(), 4000, true));
    }
    RunGitCommit(sourceRepository);
}

void GitJobTest::RemoveFiles(const QStringList &list)
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

void GitJobTest::CheckRepositoryContent(const QString &folder, const QStringList &expectedFiles)
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

void GitJobTest::UpdateSourceRepository(const QStringList &added, const QStringList &modified, const QStringList &removed)
{
    AddFiles(added, 10000);
    ChangeFiles(modified);
    RemoveFiles(removed);
}

QStringList GitJobTest::CreatedExpectedDestinationRepositoryContent(const QStringList &added, const QStringList &removed)
{
    QStringList expected = defaultRepositoryContent;
    for (int i=0; i<removed.size(); ++i)
        expected.removeOne(removed.at(i));
    expected << added;
    return expected;
}

QTEST_APPLESS_MAIN(GitJobTest)

#include "tst_taskteststest.moc"
