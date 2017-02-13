#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QStringList>
#include <string>

class GitRepository
{
public:
    GitRepository(const std::string& _source, const std::string& _destination,
                  const QStringList& _added, const QStringList& _modified, const QStringList& _removed,
                  const bool _mustAlreadyExist);


    std::string source;
    std::string destination;
    QStringList added;
    QStringList modified;
    QStringList removed;
    bool mustAlreadyExist;
};

#endif // GITREPOSITORY_H
