#include "gittesttools.h"

#include <unistd.h>
#include <QDir>
#include <QTest>

#include "filetools.h"
#include "tools.h"

using namespace std;

void GitTestTools::Init(const string& repository)
{
    string command = string("git init ") + repository;
    string unusedOutput;
    Tools::RunExternalCommandToBuffer(command, unusedOutput);

    CommitAllChanges(repository);
}

void GitTestTools::Clone(const string &source, const string &destination, const bool isMirror)
{
    std::string command("git clone ");
    if (isMirror)
        command += "--mirror ";
    command += source + " " + destination;
    command += " 2>&1 > /dev/null";
    std::string unusedOutput;
    Tools::RunExternalCommandToBuffer(command, unusedOutput);
}

void GitTestTools::CommitAllChanges(const string &repository)
{
    string fullCommand = "cd " + repository + "/ ";
    fullCommand += "&& git add -A && git commit -m \"auto\"";
    string output;
    Tools::RunExternalCommandToBuffer(fullCommand, output);
}

void GitTestTools::Update(const std::string &repository,
                                const QStringList &added,
                                const QStringList &modified,
                                const QStringList &removed)
{
    GitTestTools::AddProceduralFilesAndCommit(repository, added, 10000);
    sleep(1);
    GitTestTools::ChangeProcedurallyFilesAndCommit(repository, modified);
    sleep(1);
    GitTestTools::RemoveFilesAndCommit(repository, removed);
}

void GitTestTools::AddProceduralFilesAndCommit(const string &repository,
                                          const QStringList &list,
                                          const size_t size)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        Q_ASSERT(FileTools::CreateFile(fullname, size, true));
    }
    GitTestTools::CommitAllChanges(repository);
}

void GitTestTools::ChangeProcedurallyFilesAndCommit(const string &repository, const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        std::remove(fullname.c_str());
        Q_ASSERT(FileTools::CreateFile(fullname, 4000, true));
    }
    GitTestTools::CommitAllChanges(repository);
}

void GitTestTools::RemoveFilesAndCommit(const string &repository, const QStringList &list)
{
    for (int i=0; i<list.size(); ++i)
    {
        std::string fullname = repository + "/" + list.at(i).toStdString();
        std::remove(fullname.c_str());
    }
    GitTestTools::CommitAllChanges(repository);
}
