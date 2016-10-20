#ifndef GITREPORTPARSER_H
#define GITREPORTPARSER_H

#include "abstractoutputparser.h"

#include <fstream>
#include <string>
#include <vector>

class GitReportData
{
public :
    void Clear();

    std::vector<std::string> addedFileList;
    std::vector<std::string> modifiedFileList;
    std::vector<std::string> removedFileList;
};

class GitReportParser : public AbstractOutputParser
{
public:
    bool ParseUsingFiles(const std::string& inputFile,
                         const std::string& outputFile, std::string& description);

private:
    bool Parse(const std::string &buffer, GitReportData& reportData);

    // TODO : make this generic, for all parsers. This way thye all can work based on buffer ;-)
    bool GetFileData(const std::string& fileName, std::string& fileContents);

    void WriteFileList(const std::vector<std::string>& fileList, const std::string& operation, std::ofstream& fileStream);
};

#endif // GITREPORTPARSER_H
