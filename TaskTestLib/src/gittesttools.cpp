#include "gittesttools.h"

#include <QDir>
#include <QTest>

#include "filetools.h"
#include "tools.h"

using namespace std;

namespace
{
   QString CreateFullFilename(const wstring& repository,
                              const QString& file)
   {
      return QString::fromStdWString(repository) + "/" + file;
   }
}

void GitTestTools::Init(const wstring& repository)
{
    const wstring command = wstring(L"git init ") + repository;
    wstring unusedOutput;
    Tools::RunExternalCommandToBuffer(command, unusedOutput);

    CommitAllChanges(repository);
}

void GitTestTools::Clone(const wstring &source, const wstring &destination, const bool isMirror)
{
    std::wstring command(L"git clone ");
    if (isMirror)
        command += L"--mirror ";
    command += source + L" " + destination;
    command += L" 2>&1 > /dev/null";
    std::wstring unusedOutput;
    Tools::RunExternalCommandToBuffer(command, unusedOutput);
}

void GitTestTools::CommitAllChanges(const wstring &repository)
{
    wstring fullCommand = L"cd " + repository + L"/ ";
    fullCommand += L"&& git add -A && git commit -m \"auto\"";
    wstring output;
    Tools::RunExternalCommandToBuffer(fullCommand, output);
}

void GitTestTools::Update(const std::wstring &repository,
                                const QStringList &added,
                                const QStringList &modified,
                                const QStringList &removed)
{
    GitTestTools::AddProceduralFilesAndCommit(repository, added, 10000);
    Tools::Wait(1);
    GitTestTools::ChangeProcedurallyFilesAndCommit(repository, modified);
    Tools::Wait(1);
    GitTestTools::RemoveFilesAndCommit(repository, removed);
}

void GitTestTools::AddProceduralFilesAndCommit(const wstring &repository,
                                          const QStringList &list,
                                          const size_t size)
{
   CreateProceduralFiles(repository, list, size);
   GitTestTools::CommitAllChanges(repository);
}

void GitTestTools::ChangeProcedurallyFilesAndCommit(const wstring &repository,
                                                    const QStringList &list)
{
   RemoveFiles(repository, list);
   CreateProceduralFiles(repository, list, 4000);
   GitTestTools::CommitAllChanges(repository);
}

void GitTestTools::RemoveFilesAndCommit(const wstring &repository, const QStringList &list)
{
   RemoveFiles(repository, list);
   GitTestTools::CommitAllChanges(repository);
}

void GitTestTools::CreateProceduralFiles(const wstring& repository,
                                         const QStringList& list,
                                         const size_t size)
{
   for (const auto& file : list)
   {
       const QString fullname = CreateFullFilename(repository, file);
       Q_ASSERT(FileTools::CreateDummyFile(fullname.toStdWString(), size, true));
   }
}

void GitTestTools::RemoveFiles(const wstring& repository, const QStringList& list)
{
   for (const auto& file : list)
   {
       const QString fullname = CreateFullFilename(repository, file);
       remove(fullname.toUtf8());
   }
}
