#include "gittools.h"

#include <QDir>
#include <QTest>

#include "filetools.h"
#include "tools.h"

using namespace std;

void GitTools::Init(const string& repository)
{
    string command = string("git init ") + repository;
    string unusedOutput;
    Tools::RunExternalCommandToBuffer(command, unusedOutput);

    CommitAllChanges(repository);
}

void GitTools::Clone(const string &source, const string &destination)
{
    std::string command("git clone ");
    command += source + " " + destination;
    command += " 2>&1 > /dev/null";
    std::string unusedOutput;
    Tools::RunExternalCommandToBuffer(command, unusedOutput);
}

void GitTools::CommitAllChanges(const string &repository)
{
    string fullCommand = "cd " + repository + "/ ";
    fullCommand += "&& git add -A && git commit -m \"auto\"";
    string output;
    Tools::RunExternalCommandToBuffer(fullCommand, output);
}

void GitTools::Update(const std::string &repository,
                                const QStringList &added,
                                const QStringList &modified,
                                const QStringList &removed)
{
    GitTools::AddProceduralFilesAndCommit(repository, added, 10000);
    GitTools::ChangeProcedurallyFilesAndCommit(repository, modified);
    GitTools::RemoveFilesAndCommit(repository, removed);
}

void GitTools::CreatePopulatedFolder(const string &folder, const QStringList &files)
{
    FileTools::CreateFolder(folder);
    for (int i=0; i<files.size(); ++i)
    {
        std::string filename = folder + "/" +
                               files.at(i).toStdString();
        FileTools::CreateFile(filename, 1000, true);
    }
}

void GitTools::RemoveAll(const QString &folder)
{
    std::string removeAllCommand("rm -Rf ");
    removeAllCommand += folder.toStdString();

    std::string unusedOutput;
    Tools::RunExternalCommandToBuffer(removeAllCommand, unusedOutput);
}

void GitTools::CheckFolderExistence(const QString &folder, const bool expectedExistence)
{
    QCOMPARE(FileTools::FolderExists(folder.toStdString()), expectedExistence);
}

void GitTools::CheckFolderContent(const string &folder, const QStringList &expectedFiles)
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

void GitTools::AddProceduralFilesAndCommit(const string &repository,
                                          const QStringList &list,
                                          const size_t size)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        Q_ASSERT(FileTools::CreateFile(fullname, size, true));
    }
    GitTools::CommitAllChanges(repository);
}

void GitTools::ChangeProcedurallyFilesAndCommit(const string &repository, const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        std::remove(fullname.c_str());
        Q_ASSERT(FileTools::CreateFile(fullname, 4000, true));
    }
    GitTools::CommitAllChanges(repository);
}

void GitTools::RemoveFilesAndCommit(const string &repository, const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        std::remove(fullname.c_str());
    }
    GitTools::CommitAllChanges(repository);
}
