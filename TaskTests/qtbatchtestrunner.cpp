#include "qtbatchtestrunner.h"

#include <iostream>
#include <QTest>

using namespace std;

static const char* tab = "\t";

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

    ShowTotalLine(totalOkTests, totalFailTests);
}

void QtBatchTestRunner::ShowSummarizedData() const
{
    cout << "Summarized results :" << endl;
    int totalOkTests = 0, totalFailTests = 0;
    for (auto it=lastResults.begin(); it!=lastResults.end(); ++it)
    {
        cout << tab << it->GetName() << " : " << (it->GetResult() ? "ok" : "FAIL");
        cout << " (" << it->GetTestCount() << " tests)" << endl;

        totalOkTests += it->GetOkTestCount();
        totalFailTests += it->GetFailTestCount();
    }

    ShowTotalLine(totalOkTests, totalFailTests);
}

void QtBatchTestRunner::ShowTotalLine(const int okCount, const int failCount) const
{
    const int allCount = okCount + failCount;
    const float successRatio = okCount / static_cast<float>(allCount);
    const int successPercent = static_cast<int>(successRatio * 100.0);
    cout << "Total : " << allCount <<  " tests, " << successPercent << "% success (";
    cout << okCount << " succeeded, " << failCount << " failed)" << endl;
}
