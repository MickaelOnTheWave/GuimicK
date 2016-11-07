#include "gitreportparser.h"

#include <sstream>

using namespace std;

static const string FAST_FORWARD_TAG = "\nFast-forward";

void GitReportData::Clear()
{
    addedFileList.clear();
    modifiedFileList.clear();
    removedFileList.clear();
}

bool GitReportParser::ParseBuffer(const string &buffer)
{
    reportData.Clear();

    vector<string> lines;
    TokenizeString(buffer, '\n', lines);

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
    descriptionStream << reportData.addedFileList.size() << " added, ";
    descriptionStream << reportData.modifiedFileList.size() << " modified and ";
    descriptionStream << reportData.removedFileList.size() << " removed.";
    return descriptionStream.str();
}

string GitReportParser::GetFullDescription()
{

}

bool GitReportParser::ParseUsingFiles(const string &inputFile, const string &outputFile, string &description)
{
    // TODO : read file on one go and store all contents into string buffer.
    if (ParseFile(inputFile) == false)
        return false;

    description = GetMiniDescription();
    CreateFullFileDescriptionFromData(outputFile);
    return true;
}

void GitReportParser::GetData(GitReportData &data) const
{
    data = reportData;
}

void GitReportParser::WriteFileList(const vector<string>& fileList, const string& operation, ofstream& fileStream)
{
    fileStream << fileList.size() << " files " << operation << ".";
    if (fileList.size() > 0)
    {
        fileStream << " Full list :" << endl;
        vector<string>::const_iterator it = fileList.begin();
        vector<string>::const_iterator end = fileList.end();
        for (; it!=end; ++it)
            fileStream << "\t" << *it << endl;
    }
    else
        fileStream << endl;
}

void GitReportParser::CreateFullFileDescriptionFromData(const string &file)
{
    // TODO handle wrong file case here
    ofstream fileStream(file.c_str());
    fileStream << "Data parsed and report created successfully :" << endl;
    WriteFileList(reportData.addedFileList, "added", fileStream);
    WriteFileList(reportData.modifiedFileList, "modified", fileStream);
    WriteFileList(reportData.removedFileList, "removed", fileStream);
    fileStream.close();
}

void GitReportParser::TokenizeString(const string &input, const char separator, std::vector<string> &tokenList) const
{
    tokenList.clear();
    istringstream is(input);
    string token;
    while (getline(is, token, separator))
        tokenList.push_back(token);
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
                                     GitReportData &reportData)
{
    const string createModeString = "create mode";
    const string deleteModeString = "delete mode";

    vector<string>::const_iterator it=files.begin();
    for (; it!=files.end(); ++it)
    {
        string informationLine = GetLineWithSubstring(*it, informationLines);
        if (informationLine.find(createModeString) != string::npos)
            reportData.addedFileList.push_back(*it);
        else if (informationLine.find(deleteModeString) != string::npos)
            reportData.removedFileList.push_back(*it);
        else
            reportData.modifiedFileList.push_back(*it);

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
