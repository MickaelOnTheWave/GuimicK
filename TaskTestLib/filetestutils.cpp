#include "filetestutils.h"

#include <QDir>
#include <QTest>

#include "filetools.h"
#include "tools.h"

using namespace std;

void FileTestUtils::RemoveAll(const QString &folder)
{
    std::string removeAllCommand("rm -Rf ");
    removeAllCommand += folder.toStdString();

    std::string unusedOutput;
    Tools::RunExternalCommandToBuffer(removeAllCommand, unusedOutput);
}

void FileTestUtils::CheckFolderExistence(const QString &folder, const bool expectedExistence)
{
    QCOMPARE(FileTools::FolderExists(folder.toStdString()), expectedExistence);
}

QStringList FileTestUtils::GetFolderList(const QString &folder)
{
    QDir currentDir = QDir::current();
    currentDir.cd(folder);
    QStringList folderList = currentDir.entryList(QDir::Dirs);
    folderList.removeAll(".");
    folderList.removeAll("..");
    return folderList;
}

void FileTestUtils::CheckFolderContent(const string &folder, const QStringList &expectedFiles)
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

void FileTestUtils::CreatePopulatedFolder(const string &folder, const QStringList &files)
{
    FileTools::CreateFolder(folder);
    for (int i=0; i<files.size(); ++i)
    {
        std::string filename = folder + "/" +
                               files.at(i).toStdString();
        FileTools::CreateFile(filename, 1000, true);
    }
}
