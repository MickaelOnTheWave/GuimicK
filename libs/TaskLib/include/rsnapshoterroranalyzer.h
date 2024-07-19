#ifndef RSNAPSHOTERRORANALYZER_H
#define RSNAPSHOTERRORANALYZER_H

#include <string>
#include <vector>

class RsnapshotErrorAnalyzer
{
public:
    RsnapshotErrorAnalyzer(const std::wstring& output);

    bool IsOutOfSpaceError(void) const;
    bool IsInvalidFolderError(void) const;

private:
    bool IsOutOfSpaceError(const std::wstring& commandOutput);
    int GetRsyncErrorCode(const std::wstring& line) const;

    std::wstring GetBackupErrorLine(const std::vector<std::wstring>& lines) const;
    bool IsInvalidFolderLine(const std::wstring& line) const;

    std::vector<std::wstring> outputLines;
};

#endif // RSNAPSHOTERRORANALYZER_H
