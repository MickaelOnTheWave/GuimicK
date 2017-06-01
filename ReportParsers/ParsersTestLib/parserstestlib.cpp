#include "parserstestlib.h"
#include "parserstestsuites.h"

void GetParserLibTests(std::vector<QObject*>& tests, const std::string &dataPathPrefix)
{
    tests.push_back(new AbstractOutputParserTest(dataPathPrefix));
    tests.push_back(new GitReportParserTest(dataPathPrefix));
    tests.push_back(new RsnapshotReportParserTest(dataPathPrefix));
    tests.push_back(new AptUpgradeParserTest(dataPathPrefix));
    tests.push_back(new DfCommandParserTest(dataPathPrefix));
    tests.push_back(new GitCommitReportParserTest(dataPathPrefix));
    tests.push_back(new RsyncCommandParserTest(dataPathPrefix));
}
