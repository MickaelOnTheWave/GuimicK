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

   virtual void CreateArchive(const std::string& pathData,
                              ArchiveToolResult& result);

   virtual void AddToArchive(const std::string& pathToAdd,
                             ArchiveToolResult& result);

   virtual void ExtractArchive(const std::string& destinationPath,
                               ArchiveToolResult& result);

   void SetGzipCompression(const bool value);

private:
   void RunTarCommand(const std::string& command, ArchiveToolResult& result);

   void ParseOutput(const std::string& commandOutput,
                    const int returnValue,
                    ArchiveToolResult& result) const;

   std::string TarCreateFlags() const;
   std::string TarUpdateFlags() const;

   void ConvertToArchiveResult(TarCommandParser& parser,
                               ArchiveToolResult& result) const;

   bool useGzipCompression;
};

#endif // TARTOOL_H
