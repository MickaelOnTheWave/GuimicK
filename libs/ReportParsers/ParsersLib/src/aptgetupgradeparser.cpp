#include "aptgetupgradeparser.h"

#include <algorithm>
#include <sstream>

#include "stringtools.h"

using namespace std;

const wstring space = L" ";

bool AptGetUpgradeReport::operator==(const AptGetUpgradeReport &other) const
{
   return  (AreEqual(installedPackages, other.installedPackages) &&
            AreEqual(keptPackages, other.keptPackages) &&
            AreEqual(obsoletePackages, other.obsoletePackages) &&
            AreEqual(removedPackages, other.removedPackages) &&
            AreEqual(upgradedPackages, other.upgradedPackages) &&
            updateFileSize == other.updateFileSize);
}

void AptGetUpgradeReport::Clear()
{
    obsoletePackages.clear();
    keptPackages.clear();
    upgradedPackages.clear();
    installedPackages.clear();
    removedPackages.clear();
    updateFileSize = L"";
}

vector<wstring> *AptGetUpgradeReport::GetListPointerFromDescription(const wstring &line)
{
    if (line.find(L"kept back") != wstring::npos)
        return &keptPackages;
    else if (line.find(L"upgraded") != wstring::npos)
        return &upgradedPackages;
    else if (line.find(L"removed") != wstring::npos)
        return &removedPackages;
    else if (line.find(L"no longer required") != wstring::npos)
        return &obsoletePackages;
    else if (line.find(L"installed") != wstring::npos)
        return &installedPackages;
    else
        return NULL;
}

bool AptGetUpgradeReport::AreEqual(const StringVec &list1, const StringVec &list2) const
{
   if (list1.size() != list2.size())
      return false;

   for (const auto& str1 : list1)
   {
      auto it = std::find(list2.begin(), list2.end(), str1);
      if (it == list2.end())
         return false;
   }
   return true;
}


AptGetUpgradeParser::AptGetUpgradeParser()
    : fullDescription(L"")
{
}

AptGetUpgradeParser::~AptGetUpgradeParser()
{

}

bool AptGetUpgradeParser::ParseBuffer(const wstring &buffer)
{
    // TODO : find a way to make report not rely on language. So far, it can
    // only work with english reports.
    report.Clear();

    vector<wstring> lines;
    StringTools::Tokenize(buffer, '\n', lines);

    vector<wstring> downloadLines;
    vector<wstring> otherLines;
    ParseLines(lines, downloadLines, otherLines);

    BuildFullDescriptionList(downloadLines, otherLines);

    return true;
}

wstring AptGetUpgradeParser::GetMiniDescription()
{
    wstringstream description;
    description << report.upgradedPackages.size() << L" packages upgraded, ";
    description << report.installedPackages.size() << L" installed, ";
    description << report.removedPackages.size() << L" removed and ";
    description << report.keptPackages.size() << L" not upgraded. ";
    description << report.updateFileSize << L" taken.";
    return description.str();
}

wstring AptGetUpgradeParser::GetFullDescription()
{
    return fullDescription;
}

void AptGetUpgradeParser::GetReport(AptGetUpgradeReport &_report)
{
    _report = report;
}

void AptGetUpgradeParser::ParseLines(const vector<wstring> &lines,
                                     vector<wstring> &downloadLines,
                                     vector<wstring> &otherLines)
{
    // TODO : find a more elegant way to make sure we go through each line and don't
    // have ugly while conditions. Look for something like going through line positions in
    // a memory buffer.
    vector<wstring>::const_iterator it=lines.begin();
    for (; it!=lines.end(); ++it)
    {
        if (IsPackageListLine(*it))
        {
            BuildPackageList(it);
            --it;
        }
        else if (IsSizeLine(*it))
            ParseDownloadSizeInformation(*it);
        else if (IsPackageDownloadLine(*it))
            downloadLines.push_back(*it);
        else
            otherLines.push_back(*it);
    }
}

void AptGetUpgradeParser::BuildFullDescriptionList(std::vector<wstring> &downloadLines, std::vector<wstring> &otherLines)
{
    std::wstringstream description;
    description << "\t--- Full System update report ---" << endl << endl;
    WriteListData(&report.installedPackages, L"Newly installed packages", downloadLines, otherLines, description);
    WriteListData(&report.upgradedPackages, L"Upgraded packages", downloadLines, otherLines, description);
    WriteListData(&report.removedPackages, L"Removed packages", downloadLines, otherLines, description);
    WriteListData(&report.keptPackages, L"Kept packages", downloadLines, otherLines, description);
    fullDescription = description.str();
}

bool AptGetUpgradeParser::IsPackageListLine(const wstring &line)
{
    return (line.find(L"The following package") == 0);
}

bool AptGetUpgradeParser::IsSizeLine(const wstring &line)
{
    return (line.find(L"Need to get") == 0);
}

bool AptGetUpgradeParser::IsPackageDownloadLine(const wstring &line)
{
    return (line.find(L"Get:") == 0);
}

void AptGetUpgradeParser::ParseDownloadSizeInformation(const wstring &line)
{
   size_t initPos = wstring(L"Need to get ").length();
   size_t finalSizePos = line.find_first_of(space, initPos);
   size_t finalPos = line.find_first_of(space, finalSizePos+1);

   report.updateFileSize = line.substr(initPos, finalPos-initPos);
}

void AptGetUpgradeParser::BuildPackageList(vector<wstring>::const_iterator& lineIterator)
{
    vector<wstring>* currentList = report.GetListPointerFromDescription(*lineIterator);
    if (currentList == NULL)
        return;

    ++lineIterator;

    for (; lineIterator->find(space) == 0; ++lineIterator)
    {
        size_t currentPos = lineIterator->find_first_not_of(space);
        while (true)
        {
            size_t newPos = lineIterator->find_first_of(' ', currentPos);
            wstring package = lineIterator->substr(currentPos,newPos-currentPos);
            currentList->push_back(package);

            if (newPos != wstring::npos)
                currentPos = newPos+1;
            else
                break;
        }
    }
}

void AptGetUpgradeParser::WriteListData(vector<wstring>* packageList, const wstring& listLabel,
                                        vector<wstring> &downloadLines, vector<wstring> &otherLines, wstringstream& fileStream)
{
    if (packageList->size() > 0)
    {
        fileStream << listLabel << std::endl;
        vector<wstring>::const_iterator it=packageList->begin();
        vector<wstring>::const_iterator end=packageList->end();
        for (; it!=end; ++it)
        {
            fileStream << "\t" << *it << endl;

            wstring downloadLine = FindAndRemoveStringContaining(*it, &downloadLines);
            // Remove Get:XX header
            if (downloadLine != L"")
            {
                size_t beginPos = downloadLine.find_first_of(space);
                fileStream << "\t\t" << downloadLine.substr(beginPos) << endl;
            }

            wstring currentLine = FindAndRemoveStringContaining(*it, &otherLines);
            while (currentLine != L"")
            {
                fileStream << "\t\t" << currentLine << endl;
                currentLine = FindAndRemoveStringContaining(*it, &otherLines);
            }
        }

        fileStream << endl;
    }
}

wstring AptGetUpgradeParser::FindAndRemoveStringContaining(const wstring& tag, vector<wstring>* lineList)
{
    // TODO : use a regext for a real and complete find here. It should be :
    // (Any non-AlphaNum char) + tag + (Any non-AlphaNum char).
    // The actual way is very conservative and discards some results.
    wstring tagToFind(space);
    tagToFind += tag + space;
    vector<wstring>::iterator it=lineList->begin();
    vector<wstring>::iterator end=lineList->end();
    for (; it!=end; ++it)
    {
        if (it->find(space) != wstring::npos)
            break;
    }

    if (it == end)
        return wstring(L"");

    wstring returnValue(*it);
    lineList->erase(it);
    return returnValue;
}
