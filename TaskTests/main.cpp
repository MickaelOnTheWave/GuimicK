#include "gitjobtest.h"
#include "rsnapshoterroranalyzertest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"

#include <iostream>
#include <vector>
#include <QCoreApplication>
#include <QTest>

#include "testunitdata.h"

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

void ShowSummarizedData(const vector<TestUnitData>& testData)
{
    const char* tab = "\t";
    cout << "Summarized results :" << endl;
    int totalOkTests = 0, totalFailTests = 0;
    for (auto it=testData.begin(); it!=testData.end(); ++it)
    {
        cout << tab << it->GetName() << " : " << (it->GetResult() ? "ok" : "FAIL");
        cout << " (" << it->GetTestCount() << " tests)" << endl;
        totalOkTests += it->GetOkTestCount();
        totalFailTests += it->GetFailTestCount();
    }

    cout << "Total : " << totalOkTests << " tests OK, " << totalFailTests << " tests failed." << endl;
}

void ShowFullData(const vector<TestUnitData>& testData)
{
    const char* tab = "\t";
    cout << "Full results :" << endl;
    int totalOkTests = 0, totalFailTests = 0;
    for (auto it=testData.begin(); it!=testData.end(); ++it)
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

int main()
{
    vector<QObject*> tests;
    PopulateTestList(tests);


    vector<TestUnitData> testData;

    const char* tempResultFile = "results.xml";
    QStringList arguments;
    arguments << "TaskTests" << "-xml" << "-o" << tempResultFile;

    for (auto it=tests.begin(); it!=tests.end(); ++it)
    {
        QTest::qExec(*it, arguments);
        TestUnitData testUnit(tempResultFile);
        testData.push_back(testUnit);
    }

    //ShowSummarizedData(testData);
    ShowFullData(testData);

    FreeTestList(tests);
    remove(tempResultFile);
}

