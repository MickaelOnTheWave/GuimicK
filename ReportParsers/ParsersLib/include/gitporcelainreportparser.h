#ifndef GITPORCELAINREPORTPARSER_H
#define GITPORCELAINREPORTPARSER_H

#include "abstractfilebackupparser.h"

#include <fstream>
#include <string>
#include <vector>

#include "filebackupreport.h"

class GitPorcelainReportParser : public AbstractFileBackupParser
{
public:
    GitPorcelainReportParser();
    virtual ~GitPorcelainReportParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetFullDescription();

    virtual void GetReport(FileBackupReport& _report);

private:

    void WriteFileList(const std::vector<std::wstring>& fileList,
                       const std::wstring& operation,
                       std::wstringstream &stream);
    virtual void SplitIntoSections(const std::vector<std::wstring>& linesList,
                           std::vector<std::wstring>& fileLinesList,
                           std::vector<std::wstring>& informationLinesList);
    void RemoveLinesWithoutStartingWhitespaces(std::vector<std::wstring>& linesList);
    void CreateFileList(const std::vector<std::wstring>& linesList, std::vector<std::wstring>& fileList);
    void FillReportData(const std::vector<std::wstring>& files,
                        const std::vector<std::wstring>& informationLines,
                        FileBackupReport &reportData
                        );

    std::wstring GetLineWithSubstring(const std::wstring& stringToFind,
                                     const std::vector<std::wstring>& stringList);

    std::wstring GetLeftTrimmed(const std::wstring &input);
    std::wstring GetRightTrimmed(const std::wstring &input);
};

#endif // GITPORCELAINREPORTPARSER_H
