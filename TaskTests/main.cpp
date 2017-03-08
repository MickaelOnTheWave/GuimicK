#include <QTest>

#include "consolejobtest.h"
#include "gitjobtest.h"
#include "rsnapshoterroranalyzertest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"

using namespace std;

int main(int argc, char* argv[])
{
    //QObject* testSuite = new RsnapshotJobTest();
    //QObject* testSuite = new RsnapshotErrorAnalyzerTest();
    //QObject* testSuite = new TaskFeatureTest();
    //QObject* testSuite = new ConsoleJobTest();
    QObject* testSuite = new GitJobTest();

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}

