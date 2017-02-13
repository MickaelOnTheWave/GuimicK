#include "tasktestlib.h"

#include "gitjobtest.h"
#include "rsnapshoterroranalyzertest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"

void GetAllTests(std::vector<QObject*>& tests)
{
    tests.push_back(new RsnapshotErrorAnalyzerTest());
    tests.push_back(new RsnapshotJobTest());
    tests.push_back(new GitJobTest());
    tests.push_back(new TaskFeatureTest());
}

void FreeTests(std::vector<QObject*>& tests)
{
    for (auto it=tests.begin(); it!=tests.end(); ++it)
        delete *it;
    tests.clear();
}
