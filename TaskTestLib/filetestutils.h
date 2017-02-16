#ifndef FILETESTUTILS_H
#define FILETESTUTILS_H

#include <QString>
#include <string>

class FileTestUtils
{
public:

    static void CheckFolderContent(const std::string &folder, const QStringList &expectedFiles);

    static void CreatePopulatedFolder(const std::string &folder, const QStringList& files);

    static void RemoveAll(const QString& folder);

    static void CheckFolderExistence(const QString& folder, const bool expectedExistence);

    // TODO : create a folder comparison function that compares file contents too.
};

#endif // FILETESTUTILS_H
