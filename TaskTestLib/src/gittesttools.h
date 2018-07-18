#ifndef GITTESTTOOLS_H
#define GITTESTTOOLS_H

#include <QStringList>
#include <string>

class GitTestTools
{
public:

    static void Init(const std::string &repository);

    static void Clone(const std::string &source,
                      const std::string& destination,
                      const bool isMirror = false);

    static void CommitAllChanges(const std::string &repository);

    static void Update(const std::string &repository,
                                 const QStringList &added,
                                 const QStringList &modified,
                                 const QStringList &removed);


private:
    static void AddProceduralFilesAndCommit(const std::string& repository,
                                            const QStringList &list,
                                            const size_t size);

    static void ChangeProcedurallyFilesAndCommit(const std::string& repository,
                                                 const QStringList &list);

    static void RemoveFilesAndCommit(const std::string& repository, const QStringList &list);

};

#endif // GITTOOLS_H
