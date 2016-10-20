#ifndef APTGETUPGRADEPARSER_H
#define APTGETUPGRADEPARSER_H

#include "abstractoutputparser.h"

#include <fstream>
#include <list>

class AptGetUpgradeParser : public AbstractOutputParser
{
public:
    AptGetUpgradeParser();
    virtual ~AptGetUpgradeParser();

    bool ParseUsingFiles(const std::string& inputFile,
                         const std::string& outputFile, std::string& description);

protected:
    bool IsPackageListLine(const std::string& line);
    bool IsSizeLine(const std::string& line);
    bool IsPackageDownloadLine(const std::string& line);

    void ParseDownloadSizeInformation(const std::string& line);

    std::string BuildPackageList(const std::string& line, std::ifstream& inputStream);

    std::string BuildShortDescription();

    void BuildFullDescriptionList(const std::string& file, std::list<std::string>& downloadLines, std::list<std::string>& otherLines);

    void WriteListData(std::list<std::string>* packageList, const std::string& listLabel,
                       std::list<std::string> &downloadLines, std::list<std::string> &otherLines, std::ofstream& fileStream);

    std::string FindAndRemoveStringContaining(const std::string& tag, std::list<std::string>* lineList);

    std::list<std::string> obsoletePackages;
    std::list<std::string> keptPackages;
    std::list<std::string> upgradedPackages;
    std::list<std::string> installedPackages;
    std::list<std::string> removedPackages;
    std::string updateFileSize;

};

#endif // APTGETUPGRADEPARSER_H
