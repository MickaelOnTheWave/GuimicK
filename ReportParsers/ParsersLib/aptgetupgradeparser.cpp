#include "aptgetupgradeparser.h"

#include <sstream>

#include "tools.h"

using namespace std;

void AptGetUpgradeReport::Clear()
{
    obsoletePackages.clear();
    keptPackages.clear();
    upgradedPackages.clear();
    installedPackages.clear();
    removedPackages.clear();
    updateFileSize = "";
}

std::vector<string> *AptGetUpgradeReport::GetListPointerFromDescription(const string &line)
{
    if (line.find("kept back") != string::npos)
        return &keptPackages;
    else if (line.find("upgraded") != string::npos)
        return &upgradedPackages;
    else if (line.find("removed") != string::npos)
        return &removedPackages;
    else if (line.find("no longer required") != string::npos)
        return &obsoletePackages;
    else if (line.find("installed") != string::npos)
        return &installedPackages;
    else
        return NULL;
}


AptGetUpgradeParser::AptGetUpgradeParser()
    : fullDescription("")
{
}

AptGetUpgradeParser::~AptGetUpgradeParser()
{

}

bool AptGetUpgradeParser::ParseBuffer(const string &buffer)
{
    report.Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    vector<string> downloadLines;
    vector<string> otherLines;
    ParseLines(lines, downloadLines, otherLines);

    BuildFullDescriptionList(downloadLines, otherLines);

    return true;
}

string AptGetUpgradeParser::GetMiniDescription()
{
    std::stringstream description;
    description << report.upgradedPackages.size() << " packages upgraded, ";
    description << report.installedPackages.size() << " installed, ";
    description << report.removedPackages.size() << " removed and ";
    description << report.keptPackages.size() << " not upgraded. ";
    description << report.updateFileSize << " taken.";
    return description.str();
}

string AptGetUpgradeParser::GetFullDescription()
{
    return fullDescription;
}

void AptGetUpgradeParser::GetReport(AptGetUpgradeReport &_report)
{
    _report = report;
}

void AptGetUpgradeParser::ParseLines(const vector<string> &lines,
                                     vector<string> &downloadLines,
                                     vector<string> &otherLines)
{
    // TODO : find a more elegant way to make sure we go through each line and don't
    // have ugly while conditions. Look for something like going through line positions in
    // a memory buffer.
    vector<string>::const_iterator it=lines.begin();
    for (; it!=lines.end(); ++it)
    {
        if (IsPackageListLine(*it))
            BuildPackageList(it);
        else if (IsSizeLine(*it))
            ParseDownloadSizeInformation(*it);
        else if (IsPackageDownloadLine(*it))
            downloadLines.push_back(*it);
        else
            otherLines.push_back(*it);
    }
}

void AptGetUpgradeParser::BuildFullDescriptionList(std::vector<string> &downloadLines, std::vector<string> &otherLines)
{
    std::stringstream description;
    description << "\t--- Full System update report ---" << endl << endl;
    WriteListData(&report.installedPackages, "Newly installed packages", downloadLines, otherLines, description);
    WriteListData(&report.upgradedPackages, "Upgraded packages", downloadLines, otherLines, description);
    WriteListData(&report.removedPackages, "Removed packages", downloadLines, otherLines, description);
    WriteListData(&report.keptPackages, "Kept packages", downloadLines, otherLines, description);
    fullDescription = description.str();
}

bool AptGetUpgradeParser::IsPackageListLine(const string &line)
{
    return (line.find("The following package") == 0);
}

bool AptGetUpgradeParser::IsSizeLine(const string &line)
{
    return (line.find("Need to get") == 0);
}

bool AptGetUpgradeParser::IsPackageDownloadLine(const string &line)
{
    return (line.find("Get:") == 0);
}

void AptGetUpgradeParser::ParseDownloadSizeInformation(const string &line)
{
    size_t initPos = string("Need to get ").length();
    size_t finalSizePos = line.find_first_of(" ", initPos);
    size_t finalPos = line.find_first_of(" ", finalSizePos+1);

    report.updateFileSize = line.substr(initPos, finalPos-initPos);
}

void AptGetUpgradeParser::BuildPackageList(vector<string>::const_iterator& lineIterator)
{
    vector<string>* currentList = report.GetListPointerFromDescription(*lineIterator);
    if (currentList == NULL)
        return;

    for (; lineIterator->find(" ") == 0; ++lineIterator)
    {
        size_t currentPos = lineIterator->find_first_not_of(" ");
        while (true)
        {
            size_t newPos = lineIterator->find_first_of(' ', currentPos);
            string package = lineIterator->substr(currentPos,newPos-currentPos);
            currentList->push_back(package);

            if (newPos != string::npos)
                currentPos = newPos+1;
            else
                break;
        }
    }
}

void AptGetUpgradeParser::WriteListData(vector<string>* packageList, const string& listLabel,
                                        vector<string> &downloadLines, vector<string> &otherLines, stringstream& fileStream)
{
    if (packageList->size() > 0)
    {
        fileStream << listLabel << std::endl;
        vector<string>::const_iterator it=packageList->begin();
        vector<string>::const_iterator end=packageList->end();
        for (; it!=end; ++it)
        {
            fileStream << "\t" << *it << endl;

            string downloadLine = FindAndRemoveStringContaining(*it, &downloadLines);
            // Remove Get:XX header
            if (downloadLine != "")
            {
                size_t beginPos = downloadLine.find_first_of(" ");
                fileStream << "\t\t" << downloadLine.substr(beginPos) << endl;
            }

            string currentLine = FindAndRemoveStringContaining(*it, &otherLines);
            while (currentLine != "")
            {
                fileStream << "\t\t" << currentLine << endl;
                currentLine = FindAndRemoveStringContaining(*it, &otherLines);
            }
        }

        fileStream << endl;
    }
}

string AptGetUpgradeParser::FindAndRemoveStringContaining(const string& tag, vector<string>* lineList)
{
    // TODO : use a regext for a real and complete find here. It should be :
    // (Any non-AlphaNum char) + tag + (Any non-AlphaNum char).
    // The actual way is very conservative and discards some results.
    string tagToFind(" ");
    tagToFind += tag + " ";
    vector<string>::iterator it=lineList->begin();
    vector<string>::iterator end=lineList->end();
    for (; it!=end; ++it)
    {
        if (it->find(tagToFind) != string::npos)
            break;
    }

    if (it == end)
        return string("");

    string returnValue(*it);
    lineList->erase(it);
    return returnValue;
}
