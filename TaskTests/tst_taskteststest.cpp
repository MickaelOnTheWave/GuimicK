#include <QString>
#include <QtTest>

#include <filetools.h>
#include <gitbackupjob.h>
#include <jobstatus.h>
#include <tools.h>

#include "gitrepository.h"

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
    std::vector<GitRepository*> CreateMultipleRepositories();
    void CreateInitialRepositoryData(const std::vector<GitRepository*>& repositories);
    void RemoveAll(const QString &folder);
    void CreateDefaultData(const std::string& repository);
    void StartGitRepository(const std::string &repository);

    void RunGitBackup(const std::string &source, const std::string &destination);
    void CreateDefaultBackup(const std::string &source, const std::string &destination);
    void AddFiles(const std::string &repository, const QStringList &list, const size_t size = 1000);
    void ChangeFiles(const std::string& repository, const QStringList& list);
    void RemoveFiles(const std::string& repository, const QStringList &list);
    void RunGitCommit(const std::string &repository);

    void RunGitBackup();
    void RunGitBackup(const std::vector<std::pair<std::string, std::string> >& repositoryList);

    void CheckGitJobReturnsError(const QString& description);
    void CheckGitJobReturnsOk(const QString& description);
    void CheckGitJobReturn(const int expectedStatus,
                           const unsigned long expectedReportFileCount,
                           const QString& description);
    void CheckFolderExistence(const QString &folder, const bool expectedExistence);
    void CheckRepositoryContent(const std::string &folder,
                                const QStringList &expectedFiles);
    QString BuildDescriptionString(const int added, const int modified, const int deleted);
    QString BuildMultiDescriptionString(const std::vector<GitRepository*>& repositories);

    void UpdateSourceRepository(const QStringList& added,
                                const QStringList& modified,
                                const QStringList& removed);
    void UpdateRepository(      const std::string& repository,
                                const QStringList& added,
                                const QStringList& modified,
                                const QStringList& removed);
    QStringList CreatedExpectedDestinationRepositoryContent(const QStringList& added,
                                                            const QStringList& removed);
    const std::vector<std::pair<std::string, std::string> > CreateRepositoryListForBackup(
            const std::vector<GitRepository*>& repositories
    );

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
    CreateDefaultData(sourceRepository.toStdString());
    StartGitRepository(sourceRepository.toStdString());
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
    RunGitBackup(invalidRepository.toStdString(), destinationRepository.toStdString());
    CheckGitJobReturnsError(messageInvalidSource);
    CheckFolderExistence(invalidRepository, false);
    CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_AllValid()
{
    RunGitBackup(sourceRepository.toStdString(), destinationRepository.toStdString());
    CheckGitJobReturnsOk(messageRepositoryCreationOk);
    CheckFolderExistence(sourceRepository, true);
    CheckFolderExistence(destinationRepository, true);
    CheckRepositoryContent(destinationRepository.toStdString(), defaultRepositoryContent);
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

    const std::string stdSource = sourceRepository.toStdString();
    const std::string stdDestination = destinationRepository.toStdString();
    CreateDefaultBackup(stdSource, stdDestination);
    UpdateSourceRepository(added, modified, removed);
    RunGitBackup(stdSource, stdDestination);
    CheckGitJobReturnsOk(BuildDescriptionString(added.size(),
                                                modified.size(),
                                                removed.size() ));
    QStringList expectedFileList = CreatedExpectedDestinationRepositoryContent(added, removed);
    CheckRepositoryContent(stdDestination, expectedFileList);
}

void GitJobTest::testUpdate_MultipleRepositories()
{
    std::vector<GitRepository*> repositories = CreateMultipleRepositories();
    CreateInitialRepositoryData(repositories);

    RunGitBackup(CreateRepositoryListForBackup(repositories));
    CheckGitJobReturn(JobStatus_OK, repositories.size(), BuildMultiDescriptionString(repositories));

    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
    {
        QStringList expectedFileList = CreatedExpectedDestinationRepositoryContent((*it)->added, (*it)->removed);
        CheckRepositoryContent((*it)->destination, expectedFileList);
    }

    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
    {
        RemoveAll(QString((*it)->source.c_str()));
        RemoveAll(QString((*it)->destination.c_str()));
        delete *it;
    }
    repositories.clear();
    delete currentStatus;
    currentStatus = nullptr;

}

std::vector<GitRepository*> GitJobTest::CreateMultipleRepositories()
{
    std::vector<GitRepository*> repositories;
    repositories.push_back(new GitRepository(
        "source0",
        "dest0",
        QStringList(),
        QStringList(),
        QStringList(),
        false
    ));
    repositories.push_back(new GitRepository(
        "source1",
        "dest1",
        QStringList(),
        QStringList(),
        QStringList(),
        true
    ));
    repositories.push_back(new GitRepository(
        "source2",
        "dest2",
        QStringList({"add0", "add1"}),
        QStringList({"file0", "file1", "file2"}),
        QStringList(),
        true
    ));
    repositories.push_back(new GitRepository(
        "source3",
        "dest3",
        QStringList(),
        QStringList({"file0", "file1", "file3"}),
        QStringList("file2"),
        true
    ));
    return repositories;
}

void GitJobTest::CreateInitialRepositoryData(const std::vector<GitRepository*>& repositories)
{
    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
    {
            CreateDefaultData((*it)->source);
            StartGitRepository((*it)->source);
            if ((*it)->mustAlreadyExist)
            {
                CreateDefaultBackup((*it)->source, (*it)->destination);
                UpdateRepository((*it)->source, (*it)->added, (*it)->modified, (*it)->removed);
            }
    }
}

void GitJobTest::RemoveAll(const QString& folder)
{
    std::string removeAllCommand("rm -Rf ");
    removeAllCommand += folder.toStdString();

    std::string unusedOutput;
    Tools::RunExternalCommand(removeAllCommand, unusedOutput);
}

void GitJobTest::CreateDefaultData(const std::string& repository)
{
    FileTools::CreateFolder(repository);
    for (int i=0; i<defaultRepositoryContent.size(); ++i)
    {
        std::string filename = repository + "/" +
                               defaultRepositoryContent.at(i).toStdString();
        FileTools::CreateFile(filename, 1000, true);
    }
}

void GitJobTest::StartGitRepository(const std::string& repository)
{
    std::string gitCreationCommand("git init ");
    gitCreationCommand += repository + " && cd " + repository + " && ";
    gitCreationCommand += "git add -A && git commit -m \"initial data\"";

    std::string unusedOutput;
    Tools::RunExternalCommand(gitCreationCommand, unusedOutput);
}

void GitJobTest::RunGitBackup(const std::string &source, const std::string &destination)
{
    std::vector<std::pair<std::string, std::string> > repositoryList;
    repositoryList.push_back(make_pair(source, destination));
    GitBackupJob job(repositoryList);
    currentStatus = job.Run();
}

void GitJobTest::RunGitBackup(const std::vector<std::pair<std::string, std::string> >& repositoryList)
{
    GitBackupJob job(repositoryList);
    currentStatus = job.Run();
}

void GitJobTest::CreateDefaultBackup(const std::string& source, const std::string& destination)
{
    std::string command("git clone ");
    command += source + " " + destination;
    command += " 2>&1 > /dev/null";
    std::string unusedOutput;
    Tools::RunExternalCommand(command, unusedOutput);
}

void GitJobTest::AddFiles(const std::string& repository, const QStringList &list, const size_t size)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        Q_ASSERT(FileTools::CreateFile(fullname, size, true));
    }
    RunGitCommit(repository);
}

void GitJobTest::ChangeFiles(const std::string& repository, const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        std::remove(fullname.c_str());
        Q_ASSERT(FileTools::CreateFile(fullname, 4000, true));
    }
    RunGitCommit(repository);
}

void GitJobTest::RemoveFiles(const std::string& repository, const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        std::remove(fullname.c_str());
    }
    RunGitCommit(repository);
}

void GitJobTest::RunGitCommit(const std::string &repository)
{
    std::string fullCommand = "cd " + repository + "/ ";
    fullCommand += "&& git add -A && git commit -m \"auto\"";
    std::string output;
    Tools::RunExternalCommand(fullCommand, output);
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
    currentStatus->GetExternalFilenames(reportFiles);
    QCOMPARE(reportFiles.size(), expectedReportFileCount);
}

void GitJobTest::CheckFolderExistence(const QString& folder,
                                      const bool expectedExistence)
{
    QCOMPARE(FileTools::FolderExists(folder.toStdString()), expectedExistence);
}

void GitJobTest::CheckRepositoryContent(const std::string &folder, const QStringList &expectedFiles)
{
    QDir repositoryDir = QDir::currentPath();
    repositoryDir.cd(folder.c_str());
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

QString GitJobTest::BuildMultiDescriptionString(const std::vector<GitRepository *> &repositories)
{
    return QString::number(repositories.size()) + " Git repositories successfully backed up.";
}

void GitJobTest::UpdateSourceRepository(const QStringList &added, const QStringList &modified, const QStringList &removed)
{
    UpdateRepository(sourceRepository.toStdString(), added, modified, removed);
}

void GitJobTest::UpdateRepository(const std::string &repository, const QStringList &added, const QStringList &modified, const QStringList &removed)
{
    AddFiles(repository, added, 10000);
    ChangeFiles(repository, modified);
    RemoveFiles(repository, removed);
}

QStringList GitJobTest::CreatedExpectedDestinationRepositoryContent(const QStringList &added, const QStringList &removed)
{
    QStringList expected = defaultRepositoryContent;
    for (int i=0; i<removed.size(); ++i)
        expected.removeOne(removed.at(i));
    expected << added;
    return expected;
}

const std::vector<std::pair<std::string, std::string> > GitJobTest::CreateRepositoryListForBackup(
        const std::vector<GitRepository *> &repositories
)
{
    std::vector<std::pair<std::string, std::string> > repositoryTargetList;
    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
        repositoryTargetList.push_back(std::pair<std::string, std::string>((*it)->source, (*it)->destination));
    return repositoryTargetList;
}

QTEST_APPLESS_MAIN(GitJobTest)

#include "tst_taskteststest.moc"
