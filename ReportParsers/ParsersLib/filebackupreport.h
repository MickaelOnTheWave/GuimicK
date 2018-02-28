#ifndef FILEBACKUPREPORT_H
#define FILEBACKUPREPORT_H

#include <string>
#include <vector>

class FileBackupReport
{
public:
   FileBackupReport();
   virtual ~FileBackupReport();

   FileBackupReport& operator=(const FileBackupReport& other);

   virtual void Clear();

   void SortData();

   virtual std::string GetMiniDescription() const;
   virtual std::string GetFullDescription() const;

   void AddAsAdded(const std::vector<std::string>& files);
   void AddAsAdded(const std::string& file);
   void AddAsModified(const std::string& file);
   void AddAsRemoved(const std::string& file);

   void Add(const FileBackupReport& otherReport);
   void AddWithPrefix(const FileBackupReport& otherReport, const std::string& prefix);

   void GetAddedFiles(std::vector<std::string>& files);
   void GetModifiedFiles(std::vector<std::string>& files);
   void GetRemovedFiles(std::vector<std::string>& files);

protected:
    std::vector<std::string> added;
    std::vector<std::string> modified;
    std::vector<std::string> removed;

private:
    std::string FileListDescription(const std::vector<std::string> &fileList,
                                    const std::string &operation) const;
    void CopyPrefixed(std::vector<std::string>& destination,
                      const std::vector<std::string> &source,
                      const std::string& prefix);
};

#endif // FILEBACKUPREPORT_H
