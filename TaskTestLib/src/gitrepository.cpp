#include "gitrepository.h"

GitRepository::GitRepository(const std::string &_source, const std::string &_destination,
                             const QStringList &_added, const QStringList &_modified, const QStringList &_removed,
                             const bool _mustAlreadyExist)
    : source(_source), destination(_destination), added(_added), modified(_modified), removed(_removed),
      mustAlreadyExist(_mustAlreadyExist)
{
}
