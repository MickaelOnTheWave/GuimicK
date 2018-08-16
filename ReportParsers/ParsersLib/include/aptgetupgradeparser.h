#ifndef APTGETUPGRADEPARSER_H
#define APTGETUPGRADEPARSER_H

#include "abstractoutputparser.h"

#include <fstream>
#include <vector>

class AptGetUpgradeReport
{
public:
    void Clear();
    std::vector<std::string> *GetListPointerFromDescription(const std::string &line);

    std::vector<std::string> obsoletePackages;
    std::vector<std::string> keptPackages;
    std::vector<std::string> upgradedPackages;
    std::vector<std::string> installedPackages;
    std::vector<std::string> removedPackages;
    std::string updateFileSize;
};

class AptGetUpgradeParser : public AbstractOutputParser
{
public:
    AptGetUpgradeParser();
    virtual ~AptGetUpgradeParser();

    virtual bool ParseBuffer(const std::string& buffer);
    virtual std::string GetMiniDescription();
    virtual std::string GetFullDescription();

    void GetReport(AptGetUpgradeReport& _report);

protected:
    void ParseLines(const std::vector<std::string>& lines,
                    std::vector<std::string>& downloadLines,
                    std::vector<std::string>& otherLines);
    void BuildFullDescriptionList(  std::vector<std::string> &downloadLines,
                                    std::vector<std::string> &otherLines);
    bool IsPackageListLine(const std::string& line);
    bool IsSizeLine(const std::string& line);
    bool IsPackageDownloadLine(const std::string& line);

    void ParseDownloadSizeInformation(const std::string& line);

    void BuildPackageList(std::vector<std::string>::const_iterator &lineIterator);

    void WriteListData(std::vector<std::string>* packageList, const std::string& listLabel,
                       std::vector<std::string> &downloadLines,
                       std::vector<std::string> &otherLines, std::stringstream &fileStream);

    std::string FindAndRemoveStringContaining(const std::string& tag, std::vector<std::string>* lineList);

    AptGetUpgradeReport report;
    std::string fullDescription;

};

#endif // APTGETUPGRADEPARSER_H
