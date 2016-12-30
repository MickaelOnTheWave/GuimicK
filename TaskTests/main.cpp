#include "gitjobtest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"

#include <vector>
#include <QCoreApplication>
#include <QTest>

void PopulateTestList(std::vector<QObject*>& tests)
{
    tests.push_back(new RsnapshotJobTest());
    tests.push_back(new GitJobTest());
    tests.push_back(new TaskFeatureTest());
}

void FreeTestList(std::vector<QObject*>& tests)
{
    for (auto it=tests.begin(); it!=tests.end(); ++it)
        delete *it;
    tests.clear();

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    std::vector<QObject*> tests;
    PopulateTestList(tests);

    for (auto it=tests.begin(); it!=tests.end(); ++it)
        QTest::qExec(*it, argc, argv);

    FreeTestList(tests);
}

