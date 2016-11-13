#include "filebackupreport.h"

FileBackupReport::~FileBackupReport()
{
}

void FileBackupReport::Clear()
{
    added.clear();
    modified.clear();
    removed.clear();
}

void FileBackupReport::operator=(const FileBackupReport &other)
{
    added.clear();
    std::copy(other.added.begin(), other.added.end(), std::back_inserter(added));

    removed.clear();
    std::copy(other.removed.begin(), other.removed.end(), std::back_inserter(removed));

    modified.clear();
    std::copy(other.modified.begin(), other.modified.end(), std::back_inserter(modified));
}
