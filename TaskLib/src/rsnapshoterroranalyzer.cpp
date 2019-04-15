#include "rsnapshoterroranalyzer.h"

#include <algorithm>
#include <stdlib.h>

#include "stringtools.h"

using namespace std;

RsnapshotErrorAnalyzer::RsnapshotErrorAnalyzer(const std::wstring &output)
{
    StringTools::Tokenize(output, '\n', outputLines);
}

bool RsnapshotErrorAnalyzer::IsOutOfSpaceError(void) const
{
    const wstring rsyncErrorLineStart = L"rsync: write failed";
    vector<wstring>::const_iterator it = outputLines.begin();
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
    wstring backupErrorLine = GetBackupErrorLine(outputLines);
    return IsInvalidFolderLine(backupErrorLine);
}

int RsnapshotErrorAnalyzer::GetRsyncErrorCode(const wstring &line) const
{
    const size_t endPos = line.rfind(')');
    const size_t startPos = line.rfind('(', endPos);
    wstring codeString = line.substr(startPos+1, endPos-1);
    return StringTools::ToInt(codeString);
}

bool startFinder(const wstring& line)
{
    const wstring backupErrorStart = L"ERROR: backup";
    return (line.find(backupErrorStart) == 0);
}

bool endFinder(const wstring& line)
{
    const wstring backupErrorEnd = L"ERROR: ---------";
    return (line.find(backupErrorEnd) == 0);
}

wstring RsnapshotErrorAnalyzer::GetBackupErrorLine(const std::vector<wstring> &lines) const
{
    std::vector<wstring>::const_iterator itStart = find_if(lines.begin(), lines.end(), startFinder);
    if (itStart == lines.end())
        return wstring(L"");
    std::vector<wstring>::const_iterator itEnd = find_if(lines.begin(), lines.end(), endFinder);
    if (itEnd == lines.end())
        return wstring(L"");

    wstring fullBackupString = L"";
    for (std::vector<wstring>::const_iterator it=itStart; it!=itEnd; ++it)
       fullBackupString += *it;
    return fullBackupString;
}

bool RsnapshotErrorAnalyzer::IsInvalidFolderLine(const wstring &line) const
{
    vector<wstring> wordsToFind;
    wordsToFind.push_back(L"Source");
    wordsToFind.push_back(L"directory");
    wordsToFind.push_back(L"doesn't");
    wordsToFind.push_back(L"exist");

    vector<wstring>::const_iterator it = wordsToFind.begin();
    for (; it!=wordsToFind.end(); ++it)
    {
        if (line.find(*it) == wstring::npos)
            return false;
    }

    return true;
}
