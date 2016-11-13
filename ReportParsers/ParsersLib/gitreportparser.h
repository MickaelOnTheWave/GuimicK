#ifndef GITREPORTPARSER_H
#define GITREPORTPARSER_H

#include "abstractfilebackupparser.h"

#include <fstream>
#include <string>
#include <vector>

#include "filebackupreport.h"

class GitReportParser : public AbstractFileBackupParser
{
public:

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    virtual void GetReport(FileBackupReport& report);

private:

    void WriteFileList(const std::vector<std::string>& fileList,
                       const std::string& operation,
                       std::stringstream &stream);

    void TokenizeString(const std::string& input, const char separator, std::vector<std::string>& tokenList) const;
    void RemoveLinesWithoutStartingWhitespaces(std::vector<std::string>& linesList);
    void SplitIntoSections(const std::vector<std::string>& linesList,
                           std::vector<std::string>& fileLinesList,
                           std::vector<std::string>& informationLinesList);
    void CreateFileList(const std::vector<std::string>& linesList, std::vector<std::string>& fileList);
    void FillReportData(const std::vector<std::string>& files,
                        const std::vector<std::string>& informationLines,
                        FileBackupReport &reportData
                        );

    std::string GetLineWithSubstring(const std::string& stringToFind,
                                     const std::vector<std::string>& stringList);

    std::string GetLeftTrimmed(const std::string &input);
    std::string GetRightTrimmed(const std::string &input);

    FileBackupReport reportData;
};

#endif // GITREPORTPARSER_H
