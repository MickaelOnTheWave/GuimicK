#ifndef FILEBACKUPREPORT_H
#define FILEBACKUPREPORT_H

#include <string>
#include <vector>

class FileBackupReport
{
public:
    virtual ~FileBackupReport();

    virtual void Clear();
    void operator=(const FileBackupReport& other);

    std::vector<std::string> added;
    std::vector<std::string> modified;
    std::vector<std::string> removed;
};

#endif // FILEBACKUPREPORT_H
