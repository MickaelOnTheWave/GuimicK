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
