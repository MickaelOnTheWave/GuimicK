#ifndef ARCHIVETOOL_H
#define ARCHIVETOOL_H

#include <string>
#include <vector>

typedef std::vector<std::string> ErrorList;

class ArchiveTool
{
public:
   ArchiveTool(const std::string& _filename) : filename(_filename) {}
   virtual ~ArchiveTool() {}

   virtual bool CreateArchive(const std::string& pathData,
                              ErrorList& errors) = 0;

   virtual bool AddToArchive(const std::string& pathToAdd,
                             ErrorList& errors) = 0;

   virtual bool ExtractArchive(const std::string& destinationPath,
                               ErrorList& errors) = 0;

protected:
   const std::string filename;
};

#endif // ARCHIVETOOL_H
