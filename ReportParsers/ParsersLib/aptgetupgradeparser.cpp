#include "aptgetupgradeparser.h"

#include <sstream>

using namespace std;

AptGetUpgradeParser::AptGetUpgradeParser() : updateFileSize("")
{
}

AptGetUpgradeParser::~AptGetUpgradeParser()
{

}

bool AptGetUpgradeParser::ParseBuffer(const string &buffer)
{

}

string AptGetUpgradeParser::GetMiniDescription()
{

}

string AptGetUpgradeParser::GetFullDescription()
{

}

bool AptGetUpgradeParser::ParseUsingFiles(const std::string &inputFile, const std::string &outputFile, std::string &description)
{
    obsoletePackages.clear();
    keptPackages.clear();
    upgradedPackages.clear();
    installedPackages.clear();
    removedPackages.clear();

    list<string> downloadLines;
    list<string> otherLines;

    ifstream inFile(inputFile.c_str());

    if (inFile.is_open() == false)
        return false;

    // TODO : find a more elegant way to make sure we go through each line and don't
    // have ugly while conditions. Look for something like going through line positions in
    // a memory buffer.
    string currentLine("");
    string nextLine("");
    while (nextLine != "" || getline(inFile, currentLine))
    {
        // In order to prevent jumping one line because of two line loops.
        if (nextLine != "")
        {
            currentLine = nextLine;
            nextLine = "";
        }

        if (IsPackageListLine(currentLine))
            nextLine = BuildPackageList(currentLine, inFile);
        else if (IsSizeLine(currentLine))
            ParseDownloadSizeInformation(currentLine);
        else if (IsPackageDownloadLine(currentLine))
            downloadLines.push_back(currentLine);
        else
            otherLines.push_back(currentLine);
    }

    inFile.close();

    description = BuildShortDescription();
    BuildFullDescriptionList(outputFile, downloadLines, otherLines);


    return true;
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

    updateFileSize = line.substr(initPos, finalPos-initPos);
}

string AptGetUpgradeParser::BuildPackageList(const string &line, ifstream& inputStream)
{
    list<string>* currentList = NULL;

    if (line.find("kept back") != string::npos)
        currentList = &keptPackages;
    else if (line.find("upgraded") != string::npos)
        currentList = &upgradedPackages;
    else if (line.find("removed") != string::npos)
        currentList = &removedPackages;
    else if (line.find("no longer required") != string::npos)
        currentList = &obsoletePackages;
    else if (line.find("installed") != string::npos)
        currentList = &installedPackages;

    // This should not happen : unknown package list ??
    if (currentList == NULL)
        return string("");


    string currentLine;
    while (getline(inputStream, currentLine) && currentLine.find(" ") == 0)
    {
        size_t currentPos = currentLine.find_first_not_of(" ");
        while (true)
        {
            size_t newPos = currentLine.find_first_of(' ', currentPos);
            string package = currentLine.substr(currentPos,newPos-currentPos);
            currentList->push_back(package);

            if (newPos != string::npos)
                currentPos = newPos+1;
            else
                break;
        }
    }
    return currentLine;
}

string AptGetUpgradeParser::BuildShortDescription()
{
    std::stringstream description;
    description << upgradedPackages.size() << " packages upgraded, ";
    description << installedPackages.size() << " installed, ";
    description << removedPackages.size() << " removed and ";
    description << keptPackages.size() << " not upgraded. ";
    description <<  updateFileSize << " taken.";
    return description.str();

}

void AptGetUpgradeParser::BuildFullDescriptionList(const string &file, std::list<string> &downloadLines, std::list<string> &otherLines)
{
    std::ofstream outFile(file.c_str());
    if (outFile.is_open() == false)
        return;

    outFile << "\t--- Full System update report ---" << endl << endl;

    WriteListData(&installedPackages, "Newly installed packages", downloadLines, otherLines, outFile);
    WriteListData(&upgradedPackages, "Upgraded packages", downloadLines, otherLines, outFile);
    WriteListData(&removedPackages, "Removed packages", downloadLines, otherLines, outFile);
    WriteListData(&keptPackages, "Kept packages", downloadLines, otherLines, outFile);

    outFile.close();
}

void AptGetUpgradeParser::WriteListData(list<string>* packageList, const string& listLabel,
                                        std::list<string> &downloadLines, std::list<string> &otherLines, std::ofstream& fileStream)
{
    if (packageList->size() > 0)
    {
        fileStream << listLabel << std::endl;
        list<string>::const_iterator it=packageList->begin();
        list<string>::const_iterator end=packageList->end();
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

string AptGetUpgradeParser::FindAndRemoveStringContaining(const string& tag, list<string>* lineList)
{
    // TODO : use a regext for a real and complete find here. It should be :
    // (Any non-AlphaNum char) + tag + (Any non-AlphaNum char).
    // The actual way is very conservative and discards some results.
    string tagToFind(" ");
    tagToFind += tag + " ";
    list<string>::iterator it=lineList->begin();
    list<string>::iterator end=lineList->end();
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
