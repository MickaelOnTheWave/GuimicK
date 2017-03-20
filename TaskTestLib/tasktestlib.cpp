#include "tasktestlib.h"

#include "tasktestsuites.h"

void GetTaskLibTests(std::vector<QObject*>& tests)
{
    tests.push_back(new RsnapshotErrorAnalyzerTest());
    tests.push_back(new RsnapshotJobTest());
    tests.push_back(new GitJobTest());
    tests.push_back(new TaskFeatureTest());
    tests.push_back(new ConsoleJobTest());
    tests.push_back(new UserConsoleJobTest());
    tests.push_back(new SshConsoleJobTest());
}


