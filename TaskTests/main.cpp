#include <QTest>

#include "consolejobtest.h"
#include "gitjobtest.h"
#include "rsnapshoterroranalyzertest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"
#include "userconsolejobtest.h"

using namespace std;

int main(int argc, char* argv[])
{
    //QObject* testSuite = new RsnapshotJobTest();
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest();
    //QObject* testSuite = new TaskFeatureTest();
    //QObject* testSuite = new ConsoleJobTest();
    //QObject* testSuite = new GitJobTest();
    QObject* testSuite = new UserConsoleJobTest();

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}

