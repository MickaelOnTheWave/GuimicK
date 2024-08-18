#ifndef FILEBACKUPREPORT_H
#define FILEBACKUPREPORT_H

#include <string>
#include <vector>

class FileBackupReport
{
public:
   FileBackupReport() = default;
   FileBackupReport(const FileBackupReport& other);
   virtual ~FileBackupReport() = default;

   bool operator==(const FileBackupReport& other) const;
   FileBackupReport& operator=(const FileBackupReport& other);

   virtual void Clear();

   void SortData();

   virtual std::wstring GetMiniDescription() const;
   virtual std::wstring GetFullDescription() const;

   void AddAsAdded(const std::vector<std::wstring>& files);
   void AddAsAdded(const std::wstring& file);

   void AddAsModified(const std::vector<std::wstring>& files);
   void AddAsModified(const std::wstring& file);

   void AddAsRemoved(const std::vector<std::wstring>& files);
   void AddAsRemoved(const std::wstring& file);

   void Add(const FileBackupReport& otherReport);
   void AddWithPrefix(const FileBackupReport& otherReport, const std::wstring& prefix);

   void GetAddedFiles(std::vector<std::wstring>& files);
   void GetModifiedFiles(std::vector<std::wstring>& files);
   void GetRemovedFiles(std::vector<std::wstring>& files);

protected:
    std::vector<std::wstring> added;
    std::vector<std::wstring> modified;
    std::vector<std::wstring> removed;

private:
    std::wstring FileListDescription(const std::vector<std::wstring> &fileList,
                                    const std::wstring &operation) const;
    void CopyPrefixed(std::vector<std::wstring>& destination,
                      const std::vector<std::wstring> &source,
                      const std::wstring& prefix);
};

#endif // FILEBACKUPREPORT_H
