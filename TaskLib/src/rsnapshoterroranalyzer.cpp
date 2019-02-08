#include "rsnapshoterroranalyzer.h"

#include <algorithm>
#include <stdlib.h>

#include "tools.h"

using namespace std;

RsnapshotErrorAnalyzer::RsnapshotErrorAnalyzer(const std::string &output)
{
    Tools::TokenizeString(output, '\n', outputLines);
}

bool RsnapshotErrorAnalyzer::IsOutOfSpaceError(void) const
{
    const string rsyncErrorLineStart = "rsync: write failed";
    vector<string>::const_iterator it = outputLines.begin();
    for (; it != outputLines.end(); ++it)
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
    string backupErrorLine = GetBackupErrorLine(outputLines);
    return IsInvalidFolderLine(backupErrorLine);
}

int RsnapshotErrorAnalyzer::GetRsyncErrorCode(const string &line) const
{
    const size_t endPos = line.rfind(')');
    const size_t startPos = line.rfind('(', endPos);
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
    vector<string> wordsToFind;
    wordsToFind.push_back("Source");
    wordsToFind.push_back("directory");
    wordsToFind.push_back("doesn't");
    wordsToFind.push_back("exist");

    vector<string>::const_iterator it = wordsToFind.begin();
    for (; it!=wordsToFind.end(); ++it)
    {
        if (line.find(*it) == string::npos)
            return false;
    }

    return true;
}
