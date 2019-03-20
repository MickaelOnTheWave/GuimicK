#ifndef SEVENZIPTOOL_H
#define SEVENZIPTOOL_H

#include "archivetool.h"

class SevenZipTool : public ArchiveTool
{
public:
   SevenZipTool();
   SevenZipTool(const SevenZipTool& other);

   virtual ArchiveTool* Clone() const;

   virtual void CreateArchive(const std::wstring& pathData,
                              ArchiveToolResult& result);

   virtual void AddToArchive(const std::wstring& pathToAdd,
                             ArchiveToolResult& result);

   virtual void ExtractArchive(const std::wstring& destinationPath,
                               ArchiveToolResult& result);

   void SetExecutablePath(const std::wstring& value);

private:
   void Run7zipCommand(const std::wstring& command, ArchiveToolResult& result);

   void ParseOutput(const std::wstring& commandOutput,
                    const int returnValue,
                    ArchiveToolResult& result) const;

   std::wstring sevenZipExecutable;
};

#endif // SEVENZIPTOOL_H
