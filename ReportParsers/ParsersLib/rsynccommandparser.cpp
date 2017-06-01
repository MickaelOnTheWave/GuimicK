#include "rsynccommandparser.h"

#include "tools.h"

using namespace std;

RsyncCommandParser::RsyncCommandParser()
    : AbstractFileBackupParser(new FileBackupReport())
{
}

RsyncCommandParser::~RsyncCommandParser()
{
}

bool RsyncCommandParser::ParseBuffer(const std::string &buffer)
{
    vector<string> lines;
    Tools::TokenizeString(buffer, '\n', lines);

    RawFileChangeList changeList;
    FillChangeList(lines, changeList);

    FillReport(changeList);

    return true;
}

void RsyncCommandParser::GetReport(FileBackupReport &_reportData)
{
    _reportData = *reportData;
}

void RsyncCommandParser::FillChangeList(const std::vector<string> &reportLines,
                                        RawFileChangeList &changeList)
{
    vector<string>::const_iterator it = reportLines.begin();
    for (; it != reportLines.end(); ++it)
    {
        if (IsFileStatusChar((*it)[0]))
        {
            size_t pos = it->find_first_of(' ');
            const string fileTag = it->substr(0, pos);
            const string fileName = it->substr(pos+1);
            changeList.push_back(make_pair(fileTag, Tools::LeftTrim(fileName)));
        }
    }
}

void RsyncCommandParser::FillReport(const RawFileChangeList &changeList)
{
    RawFileChangeList::const_iterator it = changeList.begin();
    for (; it != changeList.end(); ++it)
    {
        if (it->first == "*deleting")
            reportData->AddAsRemoved(it->second);
        else if ((it->first.find(">f++") == 0) ||
                 (it->first.find("cd++") == 0) )
            reportData->AddAsAdded(it->second);
        else
            reportData->AddAsModified(it->second);
    }
}

bool RsyncCommandParser::IsFileStatusChar(const char charToTest) const
{
    return (charToTest == '*' || charToTest == 'c' || charToTest == '>');
}
