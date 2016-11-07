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

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    bool ParseUsingFiles(const std::string& inputFile,
                         const std::string& outputFile, std::string& description);

    void GetData(GitReportData& data) const;

private:

    void WriteFileList(const std::vector<std::string>& fileList, const std::string& operation, std::ofstream& fileStream);

    void CreateFullFileDescriptionFromData(const std::string& file);

    void TokenizeString(const std::string& input, const char separator, std::vector<std::string>& tokenList) const;
    void RemoveLinesWithoutStartingWhitespaces(std::vector<std::string>& linesList);
    void SplitIntoSections(const std::vector<std::string>& linesList,
                           std::vector<std::string>& fileLinesList,
                           std::vector<std::string>& informationLinesList);
    void CreateFileList(const std::vector<std::string>& linesList, std::vector<std::string>& fileList);
    void FillReportData(const std::vector<std::string>& files,
                        const std::vector<std::string>& informationLines,
                        GitReportData &reportData
                        );

    std::string GetLineWithSubstring(const std::string& stringToFind,
                                     const std::vector<std::string>& stringList);

    std::string GetLeftTrimmed(const std::string &input);
    std::string GetRightTrimmed(const std::string &input);

    GitReportData reportData;
};

#endif // GITREPORTPARSER_H
