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
    GitReportParser();
    virtual ~GitReportParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetFullDescription();

    virtual void GetReport(FileBackupReport& _report);

private:

    void WriteFileList(const std::vector<std::string>& fileList,
                       const std::string& operation,
                       std::stringstream &stream);
    virtual void SplitIntoSections(const std::vector<std::string>& linesList,
                           std::vector<std::string>& fileLinesList,
                           std::vector<std::string>& informationLinesList);
    void RemoveLinesWithoutStartingWhitespaces(std::vector<std::string>& linesList);
    void CreateFileList(const std::vector<std::string>& linesList, std::vector<std::string>& fileList);
    void FillReportData(const std::vector<std::string>& files,
                        const std::vector<std::string>& informationLines,
                        FileBackupReport &reportData
                        );

    std::string GetLineWithSubstring(const std::string& stringToFind,
                                     const std::vector<std::string>& stringList);

    std::string GetLeftTrimmed(const std::string &input);
    std::string GetRightTrimmed(const std::string &input);
};

#endif // GITREPORTPARSER_H
