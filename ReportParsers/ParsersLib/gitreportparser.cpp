#include "gitreportparser.h"

#include <sstream>

#include "tools.h"

using namespace std;

static const string FAST_FORWARD_TAG = "\nFast-forward";

// TODO : check if there is code to be refactored here. Maybe put in AbstractFileBackupParser
// for shared usage with other parsers?

bool GitReportParser::ParseBuffer(const string &buffer)
{
    reportData.Clear();

    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    RemoveLinesWithoutStartingWhitespaces(lines);

    vector<string> fileSectionLines, informationSectionLines;
    SplitIntoSections(lines, fileSectionLines, informationSectionLines);

    vector<string> fileList;
    CreateFileList(fileSectionLines, fileList);

    FillReportData(fileList, informationSectionLines, reportData);

    return true;
}

string GitReportParser::GetMiniDescription()
{
    stringstream descriptionStream;
    descriptionStream << reportData.added.size() << " added, ";
    descriptionStream << reportData.modified.size() << " modified and ";
    descriptionStream << reportData.removed.size() << " removed.";
    return descriptionStream.str();
}

string GitReportParser::GetFullDescription()
{
    stringstream descriptionStream;
    descriptionStream << "Data parsed and report created successfully :" << endl;
    WriteFileList(reportData.added, "added", descriptionStream);
    WriteFileList(reportData.modified, "modified", descriptionStream);
    WriteFileList(reportData.removed, "removed", descriptionStream);
    return descriptionStream.str() + "\n";
}

void GitReportParser::GetReport(FileBackupReport& report)
{
    report = reportData;
}

void GitReportParser::WriteFileList(const vector<string>& fileList,
                                    const string& operation,
                                    stringstream& stream)
{
    stream << fileList.size() << " files " << operation << ".";
    if (fileList.size() > 0)
    {
        stream << " Full list :" << endl;
        vector<string>::const_iterator it = fileList.begin();
        vector<string>::const_iterator end = fileList.end();
        for (; it!=end; ++it)
            stream << "\t" << *it << endl;
    }
    else
        stream << endl;
}

void GitReportParser::RemoveLinesWithoutStartingWhitespaces(std::vector<string> &linesList)
{
   vector<string>::iterator it=linesList.begin();
    while (it != linesList.end())
    {
        if (*it == GetLeftTrimmed(*it))
            it = linesList.erase(it);
        else
            ++it;
    }
}

void GitReportParser::SplitIntoSections(const std::vector<string> &linesList,
                                        std::vector<string> &fileLinesList,
                                        std::vector<string> &informationLinesList)
{
    std::vector<string>::const_iterator it=linesList.begin();
    for (; it!=linesList.end(); ++it)
    {
        if (it->find('|') != std::string::npos)
            fileLinesList.push_back(*it);
        else
            informationLinesList.push_back(*it);
    }
}

std::string GitReportParser::GetLeftTrimmed(const std::string& input)
{
    size_t first = input.find_first_not_of(' ');
    return input.substr(first);
}

std::string GitReportParser::GetRightTrimmed(const std::string& input)
{
    size_t last = input.find_last_not_of(' ');
    return input.substr(0, last+1);
}

void GitReportParser::CreateFileList(const std::vector<string> &linesList, std::vector<string> &fileList)
{
    std::vector<string>::const_iterator it=linesList.begin();
    for (; it!=linesList.end(); ++it)
    {
        size_t pos = it->find('|');
        string rawFilename = it->substr(0, pos);
        fileList.push_back(GetLeftTrimmed(GetRightTrimmed(rawFilename)));
    }
}

void GitReportParser::FillReportData(const std::vector<string> &files,
                                     const std::vector<string> &informationLines,
                                     FileBackupReport &reportData)
{
    const string createModeString = "create mode";
    const string deleteModeString = "delete mode";

    vector<string>::const_iterator it=files.begin();
    for (; it!=files.end(); ++it)
    {
        string informationLine = GetLineWithSubstring(*it, informationLines);
        if (informationLine.find(createModeString) != string::npos)
            reportData.added.push_back(*it);
        else if (informationLine.find(deleteModeString) != string::npos)
            reportData.removed.push_back(*it);
        else
            reportData.modified.push_back(*it);

    }
}

string GitReportParser::GetLineWithSubstring(const string &stringToFind, const std::vector<string> &stringList)
{
    vector<string>::const_iterator it=stringList.begin();
    for(; it!=stringList.end(); ++it)
    {
        if (it->find(stringToFind) != string::npos)
            return *it;
    }

    return string("");
}
