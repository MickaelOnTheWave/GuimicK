#ifndef TARTOOL_H
#define TARTOOL_H

#include "archivetool.h"
#include "tarcommandparser.h"

class TarTool : public ArchiveTool
{
public:
   TarTool();
   TarTool(const TarTool& other);

   virtual ArchiveTool* Clone() const;

   virtual void CreateArchive(const std::wstring& pathData,
                              ArchiveToolResult& result);

   virtual void AddToArchive(const std::wstring& pathToAdd,
                             ArchiveToolResult& result);

   virtual void ExtractArchive(const std::wstring& destinationPath,
                               ArchiveToolResult& result);

   void SetGzipCompression(const bool value);

private:
   void RunTarCommand(const std::wstring& command, ArchiveToolResult& result);

   void ParseOutput(const std::wstring& commandOutput,
                    const int returnValue,
                    ArchiveToolResult& result) const;

   std::wstring TarCreateFlags() const;
   std::wstring TarUpdateFlags() const;

   bool useGzipCompression;
};

#endif // TARTOOL_H
