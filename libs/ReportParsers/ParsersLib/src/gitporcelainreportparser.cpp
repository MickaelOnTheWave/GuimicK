#include "gitporcelainreportparser.h"

#include <sstream>

#include "stringtools.h"

using namespace std;

// TODO : check if there is code to be refactored here. Maybe put in AbstractFileBackupParser
// for shared usage with other parsers?

GitPorcelainReportParser::GitPorcelainReportParser()
    : AbstractFileBackupParser(new FileBackupReport())
{
}

GitPorcelainReportParser::~GitPorcelainReportParser()
{
}

bool GitPorcelainReportParser::ParseBuffer(const wstring &buffer)
{
    reportData->Clear();

    vector<wstring> lines;
    StringTools::Tokenize(buffer, '\n', lines);

    RemoveLinesWithoutStartingWhitespaces(lines);

    vector<wstring> fileSectionLines, informationSectionLines;
    SplitIntoSections(lines, fileSectionLines, informationSectionLines);

    vector<wstring> fileList;
    CreateFileList(fileSectionLines, fileList);

    FillReportData(fileList, informationSectionLines, *reportData);

    return true;
}

wstring GitPorcelainReportParser::GetFullDescription()
{
    wstringstream descriptionStream;
    descriptionStream << "Data parsed and report created successfully :" << endl;
    descriptionStream << reportData->GetFullDescription();
    return descriptionStream.str() + L"\n";
}

void GitPorcelainReportParser::GetReport(FileBackupReport& report)
{
    report = *reportData;
}

void GitPorcelainReportParser::WriteFileList(const vector<wstring>& fileList,
                                    const wstring& operation,
                                    wstringstream& stream)
{
    stream << fileList.size() << " files " << operation << ".";
    if (fileList.size() > 0)
    {
        stream << " Full list :" << endl;
        vector<wstring>::const_iterator it = fileList.begin();
        vector<wstring>::const_iterator end = fileList.end();
        for (; it!=end; ++it)
            stream << "\t" << *it << endl;
    }
    else
        stream << endl;
}

void GitPorcelainReportParser::RemoveLinesWithoutStartingWhitespaces(std::vector<wstring> &linesList)
{
   vector<wstring>::iterator it=linesList.begin();
    while (it != linesList.end())
    {
        if (*it == GetLeftTrimmed(*it))
            it = linesList.erase(it);
        else
            ++it;
    }
}

void GitPorcelainReportParser::SplitIntoSections(const std::vector<wstring> &linesList,
                                        std::vector<wstring> &fileLinesList,
                                        std::vector<wstring> &informationLinesList)
{
    std::vector<wstring>::const_iterator it=linesList.begin();
    for (; it!=linesList.end(); ++it)
    {
        if (it->find('|') != std::wstring::npos)
            fileLinesList.push_back(*it);
        else
            informationLinesList.push_back(*it);
    }
}

std::wstring GitPorcelainReportParser::GetLeftTrimmed(const std::wstring& input)
{
    size_t first = input.find_first_not_of(' ');
    return input.substr(first);
}

std::wstring GitPorcelainReportParser::GetRightTrimmed(const std::wstring& input)
{
    size_t last = input.find_last_not_of(' ');
    return input.substr(0, last+1);
}

void GitPorcelainReportParser::CreateFileList(const std::vector<wstring> &linesList, std::vector<wstring> &fileList)
{
    std::vector<wstring>::const_iterator it=linesList.begin();
    for (; it!=linesList.end(); ++it)
    {
        size_t pos = it->find('|');
        wstring rawFilename = it->substr(0, pos);
        fileList.push_back(GetLeftTrimmed(GetRightTrimmed(rawFilename)));
    }
}

void GitPorcelainReportParser::FillReportData(const vector<wstring> &files,
                                     const vector<wstring> &informationLines,
                                     FileBackupReport &report)
{
    const wstring createModewstring = L"create mode";
    const wstring deleteModewstring = L"delete mode";

    vector<wstring>::const_iterator it=files.begin();
    for (; it!=files.end(); ++it)
    {
        wstring informationLine = GetLineWithSubstring(*it, informationLines);
        if (informationLine.find(createModewstring) != wstring::npos)
           report.AddAsAdded(*it);
        else if (informationLine.find(deleteModewstring) != wstring::npos)
           report.AddAsRemoved(*it);
        else
           report.AddAsModified(*it);

    }
}

wstring GitPorcelainReportParser::GetLineWithSubstring(
   const wstring &wstringToFind, 
   const vector<wstring> &wstringList)
{
    vector<wstring>::const_iterator it=wstringList.begin();
    for(; it!=wstringList.end(); ++it)
    {
        if (it->find(wstringToFind) != wstring::npos)
            return *it;
    }

    return wstring(L"");
}
