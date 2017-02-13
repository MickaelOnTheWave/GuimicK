#include "qtbatchtestrunner.h"

#include <iostream>
#include <QTest>

using namespace std;

QtBatchTestRunner::QtBatchTestRunner(const QString &_applicationName)
    : applicationName(_applicationName)
{
}

void QtBatchTestRunner::Run(const std::vector<QObject *> &tests)
{
    const char* tempResultFile = "results.xml";
    QStringList arguments;
    arguments << applicationName << "-xml" << "-o" << tempResultFile;

    lastResults.clear();
    for (auto it=tests.begin(); it!=tests.end(); ++it)
    {
        QTest::qExec(*it, arguments);
        TestUnitData testUnit(tempResultFile);
        lastResults.push_back(testUnit);
    }

    remove(tempResultFile);
}

void QtBatchTestRunner::ShowFullData() const
{
    const char* tab = "\t";
    cout << "Full results :" << endl;
    int totalOkTests = 0, totalFailTests = 0;
    for (auto it=lastResults.begin(); it!=lastResults.end(); ++it)
    {
        cout << tab << it->GetName() << endl;
        for (auto itFunc=it->FunctionsBegin(); itFunc!=it->FunctionsEnd(); ++itFunc)
        {
            cout << tab << tab << itFunc->first << tab << ": ";
            cout << ((itFunc->second) ? "ok" : "fail") << endl;
        }
        totalOkTests += it->GetOkTestCount();
        totalFailTests += it->GetFailTestCount();
    }

    cout << "Total : " << totalOkTests << " tests OK, " << totalFailTests << " tests failed." << endl;

}

void QtBatchTestRunner::ShowSummarizedData() const
{
    const char* tab = "\t";
    cout << "Summarized results :" << endl;
    int totalOkTests = 0, totalFailTests = 0;
    for (auto it=lastResults.begin(); it!=lastResults.end(); ++it)
    {
        cout << tab << it->GetName() << " : " << (it->GetResult() ? "ok" : "FAIL");
        cout << " (" << it->GetTestCount() << " tests)" << endl;
        totalOkTests += it->GetOkTestCount();
        totalFailTests += it->GetFailTestCount();
    }

    cout << "Total : " << totalOkTests << " tests OK, " << totalFailTests << " tests failed." << endl;

}
