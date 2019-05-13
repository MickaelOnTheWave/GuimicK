#include "gitjobtest.h"

#include <unistd.h>
#include <QDir>
#include <QTest>

#include "consolejob.h"
#include "filetools.h"
#include "gitbackupjob.h"
#include "filetestutils.h"
#include "gittesttools.h"
#include "tools.h"

using namespace std;

GitJobTest::GitJobTest()
{
}

void GitJobTest::init()
{
    cleanup();

    CreateDefaultData(sourceRepository.toStdWString());
    GitTestTools::Init(sourceRepository.toStdWString());
}

void GitJobTest::cleanup()
{
    delete currentStatus;
    currentStatus = nullptr;
    ConsoleJob::Run(L"rm", L"-Rf *");
}

void GitJobTest::testCreate_InvalidSource()
{
    RunGitBackup(invalidRepository.toStdWString(), destinationRepository.toStdWString());
    CheckGitJobReturn(JobStatus::Error, 0, messageInvalidSource);
    FileTestUtils::CheckFolderExistence(invalidRepository, false);
    FileTestUtils::CheckFolderExistence(destinationRepository, false);
}

void GitJobTest::testCreateOne_Ok()
{
    RunGitBackup(sourceRepository.toStdWString(), destinationRepository.toStdWString());
    CheckGitJobReturn(JobStatus::Ok, 0, "Repository cloned successfully");
    FileTestUtils::CheckFolderExistence(sourceRepository, true);
    FileTestUtils::CheckFolderExistence(destinationRepository, true);
    CheckGitHeadContent(destinationRepository.toStdWString(), defaultRepositoryContent);
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

    const std::wstring stdSource = sourceRepository.toStdWString();
    const std::wstring stdDestination = destinationRepository.toStdWString();
    GitTestTools::Clone(stdSource, stdDestination, true);
    sleep(1);

    UpdateSourceRepository(added, modified, removed);
    RunGitBackup(stdSource, stdDestination);
    CheckGitJobReturnsOk(BuildDescriptionString(added.size(),
                                                modified.size(),
                                                removed.size() ));
    QStringList expectedFileList = CreatedExpectedDestinationRepositoryContent(added, removed);
    CheckGitHeadContent(stdDestination, expectedFileList);
}

void GitJobTest::testUpdate_MultipleRepositories()
{
    std::vector<GitRepository*> repositories = CreateMultipleRepositories();
    CreateInitialRepositoryData(repositories);

    RunGitBackup(CreateRepositoryListForBackup(repositories));
    CheckGitJobReturn(JobStatus::Ok, 1, BuildMultiDescriptionString(repositories));

    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
    {
        QStringList expectedFileList = CreatedExpectedDestinationRepositoryContent((*it)->added, (*it)->removed);
        CheckGitHeadContent((*it)->destination, expectedFileList);
    }

    repositories.clear();
    delete currentStatus;
    currentStatus = nullptr;
}

void GitJobTest::testUpdate_InvalidSource()
{
    const std::wstring stdSource = sourceRepository.toStdWString();
    const std::wstring stdDestination = destinationRepository.toStdWString();
    const std::wstring errorInvalidTarget = L"Invalid source repository";

    GitTestTools::Clone(stdSource, stdDestination);
    QDir dir(sourceRepository);
    dir.removeRecursively();
    RunGitBackup(stdSource, stdDestination);

    CheckGitJobReturn(JobStatus::Error, 1, QString::fromStdWString(errorInvalidTarget));
}

std::vector<GitRepository*> GitJobTest::CreateMultipleRepositories()
{
    std::vector<GitRepository*> repositories;
    repositories.push_back(new GitRepository(
        L"source0",
        L"dest0",
        QStringList(),
        QStringList(),
        QStringList(),
        false
    ));
    repositories.push_back(new GitRepository(
        L"source1",
        L"dest1",
        QStringList(),
        QStringList(),
        QStringList(),
        true
    ));
    repositories.push_back(new GitRepository(
        L"source2",
        L"dest2",
        QStringList({"add0", "add1"}),
        QStringList({"file0", "file1", "file2"}),
        QStringList(),
        true
    ));
    repositories.push_back(new GitRepository(
        L"source3",
        L"dest3",
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
            GitTestTools::Init((*it)->source);
            if ((*it)->mustAlreadyExist)
            {
                GitTestTools::Clone((*it)->source, (*it)->destination, true);
                GitTestTools::Update((*it)->source, (*it)->added, (*it)->modified, (*it)->removed);
            }
    }
}

void GitJobTest::CreateDefaultData(const std::wstring& repository)
{
    FileTestUtils::CreatePopulatedFolder(QString::fromStdWString(repository),
                                         defaultRepositoryContent);
}

void GitJobTest::RunGitBackup(const wstring& source, const wstring& destination)
{
    RepositoryList repositoryList;
    repositoryList.push_back(make_pair(source, destination));
    RunGitBackup(repositoryList);
}

void GitJobTest::RunGitBackup(const RepositoryList& repositoryList)
{
    GitBackupJob job(repositoryList);
    job.InitializeFromClient(nullptr);
    job.SetTargetLocal();
    currentStatus = job.Run();
}

void GitJobTest::CheckGitJobReturnsError(const QString& description)
{
    CheckGitJobReturn(JobStatus::Error, 0, description);
}

void GitJobTest::CheckGitJobReturnsOk(const QString &description)
{
    CheckGitJobReturn(JobStatus::Ok, 1, description);
}

void GitJobTest::CheckGitJobReturn(const int expectedStatus,
                                   const unsigned long expectedReportFileCount,
                                   const QString &description)
{
    QCOMPARE(currentStatus->GetCode(), expectedStatus);
    const bool isSameDescription = (currentStatus->GetDescription() ==
                                    description.toStdWString());
    if (!isSameDescription)
    {
       const QString newLine = "\n";
       const QString tab = "\t";
       QString message = "Description is different :" + newLine;
       message += tab + "expected: " + description + newLine;
       message += tab + "got: " + QString::fromStdWString(currentStatus->GetDescription());
       message += newLine;
       QFAIL(message.toUtf8());
    }

    std::vector<std::pair<std::wstring,std::wstring> > reportFiles;
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
    return QString::number(repositories.size()) + " repositories backed up.";
}

void GitJobTest::UpdateSourceRepository(const QStringList &added, const QStringList &modified, const QStringList &removed)
{
    GitTestTools::Update(sourceRepository.toStdWString(), added, modified, removed);
}

QStringList GitJobTest::CreatedExpectedDestinationRepositoryContent(const QStringList &added, const QStringList &removed)
{
    QStringList expected = defaultRepositoryContent;
    for (int i=0; i<removed.size(); ++i)
        expected.removeOne(removed.at(i));
    expected << added;
    return expected;
}

const GitJobTest::RepositoryList GitJobTest::CreateRepositoryListForBackup(
        const std::vector<GitRepository *> &repositories
)
{
    RepositoryList repositoryTargetList;
    for (auto it=repositories.begin(); it!=repositories.end(); ++it)
        repositoryTargetList.push_back(make_pair((*it)->source, (*it)->destination));
    return repositoryTargetList;
}

void GitJobTest::CheckGitHeadContent(const wstring &repository,
                                     const QStringList& expectedContents)
{
    const QString checkFolder = "checkFolder";
    const wstring stdCheckFolder = checkFolder.toStdWString();
    const wstring params = wstring(L"clone ") + repository + L" " + stdCheckFolder;
    ConsoleJob::Run(L"git", params);

    FileTestUtils::CheckFolderContent(checkFolder, expectedContents);

    ConsoleJob::Run(L"rm", wstring(L"-Rf ") + stdCheckFolder);
}
