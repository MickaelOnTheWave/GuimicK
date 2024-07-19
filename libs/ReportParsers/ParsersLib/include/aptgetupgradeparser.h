#ifndef APTGETUPGRADEPARSER_H
#define APTGETUPGRADEPARSER_H

#include "abstractoutputparser.h"

#include <fstream>
#include <vector>

class AptGetUpgradeReport
{
public:
    void Clear();
    std::vector<std::wstring> *GetListPointerFromDescription(const std::wstring &line);

    std::vector<std::wstring> obsoletePackages;
    std::vector<std::wstring> keptPackages;
    std::vector<std::wstring> upgradedPackages;
    std::vector<std::wstring> installedPackages;
    std::vector<std::wstring> removedPackages;
    std::wstring updateFileSize;
};

class AptGetUpgradeParser : public AbstractOutputParser
{
public:
    AptGetUpgradeParser();
    virtual ~AptGetUpgradeParser();

    virtual bool ParseBuffer(const std::wstring& buffer);
    virtual std::wstring GetMiniDescription();
    virtual std::wstring GetFullDescription();

    void GetReport(AptGetUpgradeReport& _report);

protected:
    void ParseLines(const std::vector<std::wstring>& lines,
                    std::vector<std::wstring>& downloadLines,
                    std::vector<std::wstring>& otherLines);
    void BuildFullDescriptionList(  std::vector<std::wstring> &downloadLines,
                                    std::vector<std::wstring> &otherLines);
    bool IsPackageListLine(const std::wstring& line);
    bool IsSizeLine(const std::wstring& line);
    bool IsPackageDownloadLine(const std::wstring& line);

    void ParseDownloadSizeInformation(const std::wstring& line);

    void BuildPackageList(std::vector<std::wstring>::const_iterator &lineIterator);

    void WriteListData(std::vector<std::wstring>* packageList, const std::wstring& listLabel,
                       std::vector<std::wstring> &downloadLines,
                       std::vector<std::wstring> &otherLines, std::wstringstream &fileStream);

    std::wstring FindAndRemoveStringContaining(const std::wstring& tag, std::vector<std::wstring>* lineList);

    AptGetUpgradeReport report;
    std::wstring fullDescription;

};

#endif // APTGETUPGRADEPARSER_H
