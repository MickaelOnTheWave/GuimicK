#ifndef APTGETUPGRADEPARSER_H
#define APTGETUPGRADEPARSER_H

#include "abstractoutputparser.h"

#include <vector>
#include "tools.h"

class AptGetUpgradeReport
{
public:
   bool operator==(const AptGetUpgradeReport& other) const;
   void Clear();

   using StringVec = std::vector<std::wstring>;
   StringVec *GetListPointerFromDescription(const std::wstring &line);

   StringVec obsoletePackages;
   StringVec keptPackages;
   StringVec upgradedPackages;
   StringVec installedPackages;
   StringVec removedPackages;
   std::wstring updateFileSize;
};

class AptGetUpgradeParser : public AbstractOutputParser
{
public:
    AptGetUpgradeParser() = default;
    virtual ~AptGetUpgradeParser() = default;

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
    std::wstring fullDescription = L"";

};

#endif // APTGETUPGRADEPARSER_H
