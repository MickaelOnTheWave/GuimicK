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

bool GitReportParser::ParseUsingFiles(const string &inputFile, const string &outputFile, string &description)
{
    // TODO : read file on one go and store all contents into string buffer.

    string fileContents("");

    if (GetFileData(inputFile, fileContents) == false)
        return false;

    GitReportData reportData;
    if (Parse(fileContents, reportData) == false)
        return false;

    stringstream descriptionStream;
    descriptionStream << reportData.addedFileList.size() << " added, ";
    descriptionStream << reportData.modifiedFileList.size() << " modified and ";
    descriptionStream << reportData.removedFileList.size() << " removed.";
    description = descriptionStream.str();

    // TODO handle wrong file case here
    ofstream fileStream(outputFile.c_str());
    fileStream << "Data parsed and report created successfully :" << endl;
    WriteFileList(reportData.addedFileList, "added", fileStream);
    WriteFileList(reportData.modifiedFileList, "modified", fileStream);
    WriteFileList(reportData.removedFileList, "removed", fileStream);
    fileStream.close();

    return true;
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

bool GitReportParser::Parse(const std::string &buffer, GitReportData &reportData)
{
    // TODO : implement this parser really!!
    reportData.Clear();

    if (buffer == "Already up-to-date.")
        return true;

    // 1. Look for "Fast forward" tag
    //size_t beginPos = buffer.find(FAST_FORWARD_TAG);

    // 2. Each line is a file until "file changed"

    // 3. File change count is for all changes, insert and deletes are about data, not new or old files.
    return true;
}

bool GitReportParser::GetFileData(const std::string& fileName, string& fileContents)
{
    ifstream inFile(fileName.c_str());

    if (!inFile.is_open())
        return false;

    fileContents = "";

    string line;
    while (getline(inFile, line))
        fileContents += line + "\n";

    inFile.close();
    return true;
}
