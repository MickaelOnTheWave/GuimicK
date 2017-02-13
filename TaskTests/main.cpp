#include "gitjobtest.h"
#include "rsnapshoterroranalyzertest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"

#include <iostream>
#include <vector>
#include <QCoreApplication>
#include <QTest>

#include "qtbatchtestrunner.h"

using namespace std;

void PopulateTestList(vector<QObject*>& tests)
{
    tests.push_back(new RsnapshotErrorAnalyzerTest());
    tests.push_back(new RsnapshotJobTest());
    tests.push_back(new GitJobTest());
    tests.push_back(new TaskFeatureTest());
}

void FreeTestList(vector<QObject*>& tests)
{
    for (auto it=tests.begin(); it!=tests.end(); ++it)
        delete *it;
    tests.clear();
}

int main(int, char* argv[])
{
    vector<QObject*> tests;
    PopulateTestList(tests);

    QtBatchTestRunner runner(argv[0]);
    runner.Run(tests);
    runner.ShowFullData();

    FreeTestList(tests);
}

