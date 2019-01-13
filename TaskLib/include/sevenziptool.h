#ifndef SEVENZIPTOOL_H
#define SEVENZIPTOOL_H

#include "archivetool.h"

class SevenZipTool : public ArchiveTool
{
public:
   SevenZipTool(const std::string& _filename);

   virtual void CreateArchive(const std::string& pathData,
                              ArchiveToolResult& result);

   virtual void AddToArchive(const std::string& pathToAdd,
                             ArchiveToolResult& result);

   virtual void ExtractArchive(const std::string& destinationPath,
                               ArchiveToolResult& result);

private:
   void Run7zipCommand(const std::string& command, ArchiveToolResult& result);

};

#endif // SEVENZIPTOOL_H
