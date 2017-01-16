#include "rsnapshoterroranalyzer.h"

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

int RsnapshotErrorAnalyzer::GetRsyncErrorCode(const string &line) const
{
    int endPos = line.rfind(')');
    int startPos = line.rfind('(', endPos);
    string codeString = line.substr(startPos+1, endPos-1);
    return atoi(codeString.c_str());
}
