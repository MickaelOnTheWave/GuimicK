#ifndef GITTESTTOOLS_H
#define GITTESTTOOLS_H

#include <QStringList>
#include <string>

class GitTestTools
{
public:

    static void Init(const std::wstring &repository);

    static void Clone(const std::wstring &source,
                      const std::wstring& destination,
                      const bool isMirror = false);

    static void CommitAllChanges(const std::wstring &repository);

    static void Update(const std::wstring &repository,
                                 const QStringList &added,
                                 const QStringList &modified,
                                 const QStringList &removed);


private:
    static void AddProceduralFilesAndCommit(const std::wstring& repository,
                                            const QStringList &list,
                                            const size_t size);

    static void ChangeProcedurallyFilesAndCommit(const std::wstring& repository,
                                                 const QStringList &list);

    static void RemoveFilesAndCommit(const std::wstring& repository, const QStringList &list);

    static void CreateProceduralFiles(const std::wstring& repository,
                                      const QStringList &list,
                                      const size_t size);
    static void RemoveFiles(const std::wstring& repository, const QStringList &list);

};

#endif // GITTOOLS_H
