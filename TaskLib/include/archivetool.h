#ifndef ARCHIVETOOL_H
#define ARCHIVETOOL_H

#include <string>
#include <vector>

class ArchiveToolResult
{
public:
   std::vector<std::string> ErrorList;
   std::vector<std::string> FileList;
   bool isOk;
};

class ArchiveTool
{
public:
   ArchiveTool() : filename("") {}
   ArchiveTool(const ArchiveTool& other) : filename(other.filename) {}

   virtual ~ArchiveTool() {}

   virtual void Initialize(const std::string& _filename)
   {
      filename = _filename;
   }

   virtual ArchiveTool* Clone() const = 0;

   virtual void CreateArchive(const std::string& pathData,
                              ArchiveToolResult& result) = 0;

   virtual void AddToArchive(const std::string& pathToAdd,
                             ArchiveToolResult& result) = 0;

   virtual void ExtractArchive(const std::string& destinationPath,
                               ArchiveToolResult& result) = 0;

protected:
   std::string filename;
};

#endif // ARCHIVETOOL_H
