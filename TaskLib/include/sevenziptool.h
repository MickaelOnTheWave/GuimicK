#ifndef SEVENZIPTOOL_H
#define SEVENZIPTOOL_H

#include "archivetool.h"

class SevenZipTool : public ArchiveTool
{
public:
   SevenZipTool();
   SevenZipTool(const SevenZipTool& other);

   virtual ArchiveTool* Clone() const;

   virtual void CreateArchive(const std::string& pathData,
                              ArchiveToolResult& result);

   virtual void AddToArchive(const std::string& pathToAdd,
                             ArchiveToolResult& result);

   virtual void ExtractArchive(const std::string& destinationPath,
                               ArchiveToolResult& result);

private:
   void Run7zipCommand(const std::string& command, ArchiveToolResult& result);

   void ParseOutput(const std::string& commandOutput,
                    const int returnValue,
                    ArchiveToolResult& result) const;


};

#endif // SEVENZIPTOOL_H
