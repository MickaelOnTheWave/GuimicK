#include "parserstestlib.h"
#include "parserstestsuites.h"

void GetParserLibTests(std::vector<QObject*>& tests, const std::wstring &dataPathPrefix)
{
    tests.push_back(new AbstractOutputParserTest(dataPathPrefix));
    tests.push_back(new GitPorcelainReportParserTest(dataPathPrefix));
    tests.push_back(new RsnapshotReportParserTest(dataPathPrefix));
    tests.push_back(new AptUpgradeParserTest(dataPathPrefix));
    tests.push_back(new DfCommandParserTest(dataPathPrefix));
    tests.push_back(new GitPlumbingReportParserTest(dataPathPrefix));
    tests.push_back(new RsyncCommandParserTest(dataPathPrefix));
    tests.push_back(new TarCommandParserTest(dataPathPrefix));
}
