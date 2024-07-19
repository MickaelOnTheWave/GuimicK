#ifndef ARCHIVETOOL_H
#define ARCHIVETOOL_H

#include <string>
#include <vector>

#include "abstractfilebackupparser.h"
#include "filebackupreport.h"

class ArchiveToolResult
{
public:
   std::vector<std::wstring> ErrorList;
   FileBackupReport FileList;
   bool isOk;
   std::wstring errorMessage;
};

class ArchiveTool
{
public:
   ArchiveTool();
   ArchiveTool(const ArchiveTool& other);
   virtual ~ArchiveTool();

   virtual void Initialize(const std::wstring& _filename);

   virtual ArchiveTool* Clone() const = 0;

   virtual void CreateArchive(const std::wstring& pathData,
                              ArchiveToolResult& result) = 0;

   virtual void AddToArchive(const std::wstring& pathToAdd,
                             ArchiveToolResult& result) = 0;

   virtual void ExtractArchive(const std::wstring& destinationPath,
                               ArchiveToolResult& result) = 0;

protected:
   void ConvertToArchiveResult(AbstractFileBackupParser& parser,
                               ArchiveToolResult& result) const;

   std::wstring filename;
};

#endif // ARCHIVETOOL_H
