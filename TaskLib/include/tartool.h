#ifndef TARTOOL_H
#define TARTOOL_H

#include "archivetool.h"


class TarTool : public ArchiveTool
{
public:
   TarTool(const std::string& _filename);

   void SetGzipCompression(const bool value);

   virtual bool CreateArchive(const std::string& pathData,
                              ErrorList& errors) = 0;

   virtual bool AddToArchive(const std::string& pathToAdd,
                             ErrorList& errors) = 0;

   virtual bool ExtractArchive(const std::string& destinationPath,
                               ErrorList& errors) = 0;

private:
   std::string TarCreateFlags() const;
   std::string TarUpdateFlags() const;
   std::string AddCompressionFlag(const std::string& baseFlags) const;

   bool useGzipCompression;
};

#endif // TARTOOL_H
