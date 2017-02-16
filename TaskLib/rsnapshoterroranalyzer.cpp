#include "rsnapshoterroranalyzer.h"

#include <algorithm>
#include <stdlib.h>

#include "tools.h"

using namespace std;

RsnapshotErrorAnalyzer::RsnapshotErrorAnalyzer(const std::string &output)
    : commandOutput(output)
{
}

bool RsnapshotErrorAnalyzer::IsOutOfSpaceError(void) const
{
    const string rsyncErrorLineStart = "rsync: write failed";
    vector<string> lines;
    Tools::TokenizeString(commandOutput, '\n', lines);

    vector<string>::const_iterator it = lines.begin();
    for (; it != lines.end(); ++it)
    {
        if (it->find(rsyncErrorLineStart) == 0)
        {
            int errorCode = GetRsyncErrorCode(*it);
            if (errorCode == 28)
                return true;
        }
    }

    return false;
}

bool RsnapshotErrorAnalyzer::IsInvalidFolderError() const
{
    const string errorLineStart = "ERROR: ";
    vector<string> lines;
    Tools::TokenizeString(commandOutput, '\n', lines);

    string backupErrorLine = GetBackupErrorLine(lines);
    return IsInvalidFolderLine(backupErrorLine);
}

int RsnapshotErrorAnalyzer::GetRsyncErrorCode(const string &line) const
{
    int endPos = line.rfind(')');
    int startPos = line.rfind('(', endPos);
    string codeString = line.substr(startPos+1, endPos-1);
    return atoi(codeString.c_str());
}

bool startFinder(const string& line)
{
    const string backupErrorStart = "ERROR: backup";
    return (line.find(backupErrorStart) == 0);
}

bool endFinder(const string& line)
{
    const string backupErrorEnd = "ERROR: ---------";
    return (line.find(backupErrorEnd) == 0);
}

string RsnapshotErrorAnalyzer::GetBackupErrorLine(const std::vector<string> &lines) const
{
    std::vector<string>::const_iterator itStart = find_if(lines.begin(), lines.end(), startFinder);
    if (itStart == lines.end())
        return string("");
    std::vector<string>::const_iterator itEnd = find_if(lines.begin(), lines.end(), endFinder);
    if (itEnd == lines.end())
        return string("");

    string fullBackupString = "";
    for (std::vector<string>::const_iterator it=itStart; it!=itEnd; ++it)
        fullBackupString += *it;
    return fullBackupString;
}

bool RsnapshotErrorAnalyzer::IsInvalidFolderLine(const string &line) const
{
    bool sourceDir = (line.find("Source directory") != string::npos);
    bool doesntExist = (line.find("doesn't exist") != string::npos);
    return sourceDir && doesntExist;
}
