#include "abstractoutputparsertest.h"
#include "gitreportparsertest.h"
#include "rsnapshotreportparsertest.h"

#include <vector>

void PopulateTestList(std::vector<QObject*>& tests)
{
    tests.push_back(new AbstractOutputParserTest());
    tests.push_back(new GitReportParserTest());
    tests.push_back(new RsnapshotReportParserTest());
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

