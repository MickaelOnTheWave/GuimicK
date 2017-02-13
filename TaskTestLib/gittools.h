#ifndef GITTOOLS_H
#define GITTOOLS_H

#include <QStringList>
#include <string>

class GitTools
{
public:

    static void Init(const std::string &repository);

    static void Clone(const std::string &source, const std::string& destination);

    static void CommitAllChanges(const std::string &repository);

    static void Update(const std::string &repository,
                                 const QStringList &added,
                                 const QStringList &modified,
                                 const QStringList &removed);

    // Non-Git but useful, and for now without a proper place.
    // TODO : put these helpers in proper place

    static void CreatePopulatedFolder(const std::string &folder, const QStringList& files);

    static void RemoveAll(const QString& folder);

    static void CheckFolderExistence(const QString& folder, const bool expectedExistence);

    static void CheckFolderContent(const std::string &folder, const QStringList &expectedFiles);

private:
    static void AddProceduralFilesAndCommit(const std::string& repository,
                                            const QStringList &list,
                                            const size_t size);

    static void ChangeProcedurallyFilesAndCommit(const std::string& repository,
                                                 const QStringList &list);

    static void RemoveFilesAndCommit(const std::string& repository, const QStringList &list);

};

#endif // GITTOOLS_H
