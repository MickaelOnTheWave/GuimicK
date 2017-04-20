#include "gitjobtest.h"

#include <QDir>
#include <QTest>

#include "filetools.h"
#include "gitbackupjob.h"
#include "filetestutils.h"
#include "gittools.h"
#include "tools.h"

GitJobTest::GitJobTest()
{
}

void GitJobTest::init()
{
    CreateDefaultData(sourceRepository.toStdString());
    GitTools::Init(sourceRepository.toStdString());
}

void GitJobTest::cleanup()
{
    delete currentStatus;
    currentStatus = nullptr;

    FileTestUtils::RemoveAll(sourceRepository);
    FileTestUtils::RemoveAll(destinationRepository);
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
    FileTestUtils::CheckFolderExistence(invalidRepository, false);
    FileTestUtils::CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreate_AllValid()
{
    RunGitBackup(sourceRepository.toStdString(), destinationRepository.toStdString());
    CheckGitJobReturnsOk(messageRepositoryCreationOk);
    FileTestUtils::CheckFolderExistence(sourceRepository, true);
    FileTestUtils::CheckFolderExistence(destinationRepository, true);
    FileTestUtils::CheckFolderContent(destinationRepository.toStdString(), defaultRepositoryContent);
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
    GitTools::Clone(stdSource, stdDestination);
    UpdateSourceRepository(added, modified, removed);
    RunGitBackup(stdSource, stdDestination);
    CheckGitJobReturnsOk(BuildDescriptionString(added.size(),
                                                modified.size(),
                                                removed.size() ));
    QStringList expectedFileList = CreatedExpectedDestinationRepositoryContent(added, removed);
    FileTestUtils::CheckFolderContent(stdDestination, expectedFileList);
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
        FileTestUtils::CheckFolderContent((*it)->destination, expectedFileList);
    }

    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
    {
        FileTestUtils::RemoveAll(QString((*it)->source.c_str()));
        FileTestUtils::RemoveAll(QString((*it)->destination.c_str()));
        delete *it;
    }
    repositories.clear();
    delete currentStatus;
    currentStatus = nullptr;

}

void GitJobTest::testUpdate_InvalidSource()
{
    const std::string stdSource = sourceRepository.toStdString();
    const std::string stdDestination = destinationRepository.toStdString();
    const std::string errorInvalidTarget = "Unknown error";

    GitTools::Clone(stdSource, stdDestination);
    QDir dir(sourceRepository);
    dir.removeRecursively();
    RunGitBackup(stdSource, stdDestination);

    CheckGitJobReturnsError(QString(errorInvalidTarget.c_str()));
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
            GitTools::Init((*it)->source);
            if ((*it)->mustAlreadyExist)
            {
                GitTools::Clone((*it)->source, (*it)->destination);
                GitTools::Update((*it)->source, (*it)->added, (*it)->modified, (*it)->removed);
            }
    }
}

void GitJobTest::CreateDefaultData(const std::string& repository)
{
    FileTestUtils::CreatePopulatedFolder(repository, defaultRepositoryContent);
}

void GitJobTest::RunGitBackup(const std::string &source, const std::string &destination)
{
    std::vector<std::pair<std::string, std::string> > repositoryList;
    repositoryList.push_back(make_pair(source, destination));
    RunGitBackup(repositoryList);
}

void GitJobTest::RunGitBackup(const std::vector<std::pair<std::string, std::string> >& repositoryList)
{
    GitBackupJob job(repositoryList);
    currentStatus = job.Run();
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

    std::vector<std::pair<std::string,std::string> > reportFiles;
    currentStatus->GetFileBuffers(reportFiles);
    QCOMPARE(reportFiles.size(), expectedReportFileCount);
}

QString GitJobTest::BuildDescriptionString(const int added, const int modified, const int deleted)
{
    QString description = QString::number(added) + " added, ";
    description += QString::number(modified) + " modified, ";
    description += QString::number(deleted) + " removed";
    return description;
}

QString GitJobTest::BuildMultiDescriptionString(const std::vector<GitRepository *> &repositories)
{
    return QString::number(repositories.size()) + " Git repositories successfully backed up.";
}

void GitJobTest::UpdateSourceRepository(const QStringList &added, const QStringList &modified, const QStringList &removed)
{
    GitTools::Update(sourceRepository.toStdString(), added, modified, removed);
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
