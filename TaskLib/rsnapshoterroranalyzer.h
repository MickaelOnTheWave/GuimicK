#ifndef RSNAPSHOTERRORANALYZER_H
#define RSNAPSHOTERRORANALYZER_H

#include <string>

class RsnapshotErrorAnalyzer
{
public:
    RsnapshotErrorAnalyzer(const std::string& output);

    bool IsOutOfSpaceError(void) const;

private:
    bool IsOutOfSpaceError(const std::string& commandOutput);
    int GetRsyncErrorCode(const std::string& line) const;

    std::string commandOutput;
};

#endif // RSNAPSHOTERRORANALYZER_H
