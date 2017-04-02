#include "clientworkmanagertest.h"

#include <QTest>
#include "dummyjob.h"
#include "testutils.h"

// TODO : remove this and find a way to use JobStatus values outside of library
#define JobStatus_NOT_EXECUTED      0
#define JobStatus_OK                1
#define JobStatus_OK_WITH_WARNINGS  2
#define JobStatus_ERROR             3

using namespace std;

const string clientName = "Test Client";

ClientWorkManagerTest::ClientWorkManagerTest()
    : QtTestSuite("")
{
}

void ClientWorkManagerTest::init()
{
    manager = new ClientWorkManager(new Client(clientName));
}

void ClientWorkManagerTest::cleanup()
{
    delete manager;
}

void ClientWorkManagerTest::testAddRemoveJob()
{
    CheckJobList(QStringList());

    manager->AddJob(new DummyJob("Job 1"));
    manager->AddJob(new DummyJob("Job 2"));
    manager->AddJob(new DummyJob("Job 3"));

    CheckJobList(QStringList({"Job 1", "Job 2", "Job 3"}));

    manager->RemoveJob("Job 1");
    manager->RemoveJob("Job 2");
    manager->RemoveJob("Job 3");

    CheckJobList(QStringList());
}

void ClientWorkManagerTest::testRemoveJob()
{
    manager->AddJob(new DummyJob("Job 1"));
    manager->AddJob(new DummyJob("Job 2"));

    bool result = manager->RemoveJob("Invalid Job");
    CheckJobList(QStringList({"Job 1", "Job 2"}));
    QCOMPARE(result, false);

    result = manager->RemoveJob("Job 1");
    CheckJobList(QStringList({"Job 2"}));
    QCOMPARE(result, true);
}

void ClientWorkManagerTest::testRemoveAllButJobs()
{
    manager->AddJob(new DummyJob("Job 1"));
    manager->AddJob(new DummyJob("Job 2"));
    manager->AddJob(new DummyJob("Job 3"));

    bool result = manager->RemoveAllButJobs("Job 3");
    CheckJobList(QStringList({"Job 3"}));
    QCOMPARE(result, true);

    result = manager->RemoveAllButJobs("Job 3");
    CheckJobList(QStringList({"Job 3"}));
    QCOMPARE(result, false);

    result = manager->RemoveAllButJobs("Invalid Job");
    CheckJobList(QStringList());
    QCOMPARE(result, true);
}


void ClientWorkManagerTest::testRunWorkList_data()
{
    QTest::addColumn<QStringList>("expectedNames");
    QTest::addColumn<QStringList>("expectedCodes");
    QTest::addColumn<QStringList>("expectedDescriptions");
    QTest::addColumn<QStringList>("failingInitializations");

    QTest::newRow("Empty list") << QStringList() << QStringList() << QStringList() << QStringList();
    QTest::newRow("Simple list") << QStringList({"Job 1","Job 2", "Job 3", "Job 4"})
                                   << QStringList({"Ok","Executed with minor errors", "Error", "Not executed"})
                                   << QStringList({"first description", "second description",
                                                   "third description", "last description"})
                                   << QStringList({"", "", "", ""});

    QTest::newRow("Mixed list") << QStringList({"Simple job 1","Simple Job 2", "Warning job",
                                                "Error job", "No init job"})
                                   << QStringList({"Ok","Ok","Executed with minor errors", "Error", "Not executed"})
                                   << QStringList({"", "", "", "", "Initialization failed"})
                                   << QStringList({"", "", "", "", "fail"});
}

void ClientWorkManagerTest::testRunWorkList()
{
    SetupJobList();

    WorkResultData* globalResult = manager->RunWorkList();

    CheckGlobalResult(globalResult);
    CheckClientResult(globalResult->allClientsResults.front().second);

    delete globalResult;
}

void ClientWorkManagerTest::CheckJobList(const QStringList &expectedList)
{
    std::list<AbstractJob*> jobList;
    manager->GetJobList(jobList);

    const bool isSameSize = (expectedList.size() == static_cast<int>(jobList.size()));
    if (isSameSize == false)
    {
        QCOMPARE(static_cast<int>(jobList.size()), expectedList.size());
        return;
    }

    auto itExpected = expectedList.begin();
    auto itResult = jobList.begin();
    for (; itExpected != expectedList.end(); ++itExpected, ++itResult)
        QCOMPARE((*itResult)->GetName(), itExpected->toStdString());
}

void ClientWorkManagerTest::SetupJobList()
{
    QFETCH(QStringList, expectedNames);
    QFETCH(QStringList, expectedCodes);
    QFETCH(QStringList, expectedDescriptions);
    QFETCH(QStringList, failingInitializations);

    for (int i=0; i<expectedNames.size(); ++i)
    {
        DummyJob* newJob = new DummyJob(expectedNames.at(i).toStdString(),
                                        JobStatus::GetCodeFromDescription(expectedCodes.at(i).toStdString()),
                                        expectedDescriptions.at(i).toStdString());
        if (failingInitializations.at(i) != "")
            newJob->SetInitialization(false);
        manager->AddJob(newJob);
    }
}

void ClientWorkManagerTest::CheckGlobalResult(WorkResultData *result)
{
    QCOMPARE(result->allClientsResults.size(), 1ul);
    QCOMPARE(result->allClientsResults.front().first, clientName);
}

void ClientWorkManagerTest::CheckClientResult(ClientJobResults *results)
{
    QFETCH(QStringList, expectedNames);
    QFETCH(QStringList, expectedCodes);
    QFETCH(QStringList, expectedDescriptions);

    vector<string> jobNames;
    GetJobNames(results, jobNames);
    TestUtils::CheckListsAreEqual(expectedNames, jobNames);

    vector<string> descriptions;
    GetJobDescriptions(results, descriptions);
    TestUtils::CheckListsAreEqual(expectedDescriptions, descriptions);

    vector<string> codes;
    GetJobCodes(results, codes);
    TestUtils::CheckListsAreEqual(expectedCodes, codes);
}

void ClientWorkManagerTest::GetJobNames(ClientJobResults *results, std::vector<string> &values)
{
    for (auto it = results->begin(); it != results->end(); ++it)
        values.push_back(it->first);
}

void ClientWorkManagerTest::GetJobCodes(ClientJobResults *results, std::vector<string> &values)
{
    for (auto it = results->begin(); it != results->end(); ++it)
        values.push_back(it->second->GetCodeDescription());
}

void ClientWorkManagerTest::GetJobDescriptions(ClientJobResults *results, std::vector<string> &values)
{
    for (auto it = results->begin(); it != results->end(); ++it)
        values.push_back(it->second->GetDescription());
}
