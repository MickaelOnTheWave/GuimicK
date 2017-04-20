#ifndef FILEBACKUPREPORT_H
#define FILEBACKUPREPORT_H

#include <string>
#include <vector>

class FileBackupReport
{
public:
    FileBackupReport();
    virtual ~FileBackupReport();

    virtual void Clear();
    void operator=(const FileBackupReport& other);
    void SortData();

    virtual std::string GetMiniDescription() const;
    virtual std::string GetFullDescription() const;

    void AddAsAdded(const std::string& file);
    void AddAsModified(const std::string& file);
    void AddAsRemoved(const std::string& file);

// TODO : make that protected
public:
    std::vector<std::string> added;
    std::vector<std::string> modified;
    std::vector<std::string> removed;

private:
    std::string FileListDescription(const std::vector<std::string> &fileList,
                                    const std::string &operation) const;
};

#endif // FILEBACKUPREPORT_H
