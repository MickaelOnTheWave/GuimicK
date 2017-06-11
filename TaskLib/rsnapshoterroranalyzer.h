#ifndef RSNAPSHOTERRORANALYZER_H
#define RSNAPSHOTERRORANALYZER_H

#include <string>
#include <vector>

class RsnapshotErrorAnalyzer
{
public:
    RsnapshotErrorAnalyzer(const std::string& output);

    bool IsOutOfSpaceError(void) const;
    bool IsInvalidFolderError(void) const;

private:
    bool IsOutOfSpaceError(const std::string& commandOutput);
    int GetRsyncErrorCode(const std::string& line) const;

    std::string GetBackupErrorLine(const std::vector<std::string>& lines) const;
    bool IsInvalidFolderLine(const std::string& line) const;

    std::vector<std::string> outputLines;
};

#endif // RSNAPSHOTERRORANALYZER_H
