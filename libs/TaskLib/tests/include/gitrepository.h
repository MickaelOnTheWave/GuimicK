#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QStringList>
#include <string>

class GitRepository
{
public:
    GitRepository(const std::wstring& _source, const std::wstring& _destination,
                  const QStringList& _added, const QStringList& _modified, const QStringList& _removed,
                  const bool _mustAlreadyExist);


    std::wstring source;
    std::wstring destination;
    QStringList added;
    QStringList modified;
    QStringList removed;
    bool mustAlreadyExist;
};

#endif // GITREPOSITORY_H
