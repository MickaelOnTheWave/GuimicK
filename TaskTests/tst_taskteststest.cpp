#include <QString>
#include <QtTest>

class TaskTestsTest : public QObject
{
    Q_OBJECT

public:
    TaskTestsTest();

private Q_SLOTS:
    void testCase1();
};

TaskTestsTest::TaskTestsTest()
{
}

void TaskTestsTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TaskTestsTest)

#include "tst_taskteststest.moc"
