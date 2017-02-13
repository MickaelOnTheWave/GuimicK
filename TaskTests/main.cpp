#include <QTest>

#include "gitjobtest.h"
#include "rsnapshoterroranalyzertest.h"
#include "rsnapshotjobtest.h"
#include "taskfeaturetest.h"

using namespace std;

int main(int argc, char* argv[])
{
    QObject* testSuite = new RsnapshotJobTest();

    QTest::qExec(testSuite, argc, argv);

    delete testSuite;
}

