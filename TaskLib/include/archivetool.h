#ifndef ARCHIVETOOL_H
#define ARCHIVETOOL_H

#include <string>
#include <vector>

#include "abstractfilebackupparser.h"

class ArchiveToolResult
{
public:
   std::vector<std::string> ErrorList;
   std::vector<std::string> FileList;
   bool isOk;
   std::string errorMessage;
};

class ArchiveTool
{
public:
   ArchiveTool();
   ArchiveTool(const ArchiveTool& other);
   virtual ~ArchiveTool();

   virtual void Initialize(const std::string& _filename);

   virtual ArchiveTool* Clone() const = 0;

   virtual void CreateArchive(const std::string& pathData,
                              ArchiveToolResult& result) = 0;

   virtual void AddToArchive(const std::string& pathToAdd,
                             ArchiveToolResult& result) = 0;

   virtual void ExtractArchive(const std::string& destinationPath,
                               ArchiveToolResult& result) = 0;

protected:
   void ConvertToArchiveResult(AbstractFileBackupParser& parser,
                               ArchiveToolResult& result) const;

   std::string filename;
};

#endif // ARCHIVETOOL_H
