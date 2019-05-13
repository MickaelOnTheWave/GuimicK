#include "clientworkmanagertest.h"

#include <QTest>
#include "dummyjob.h"
#include "testutils.h"

using namespace std;

const wstring clientName = L"Test Client";
static const char* okMessage                = "Ok";
static const char* okWithWarningsMessage    = "Executed with minor errors";
static const char* errorMessage             = "Error";
static const char* notExecutedMessage       = "Not executed";

ClientWorkManagerTest::ClientWorkManagerTest()
    : QtTestSuite(L"")
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

    manager->AddJob(new DummyJob(L"Job 1"));
    manager->AddJob(new DummyJob(L"Job 2"));
    manager->AddJob(new DummyJob(L"Job 3"));

    CheckJobList(QStringList({"Job 1", "Job 2", "Job 3"}));

    manager->RemoveJob(L"Job 1");
    manager->RemoveJob(L"Job 2");
    manager->RemoveJob(L"Job 3");

    CheckJobList(QStringList());
}

void ClientWorkManagerTest::testRemoveJob()
{
    manager->AddJob(new DummyJob(L"Job 1"));
    manager->AddJob(new DummyJob(L"Job 2"));

    bool result = manager->RemoveJob(L"Invalid Job");
    CheckJobList(QStringList({"Job 1", "Job 2"}));
    QCOMPARE(result, false);

    result = manager->RemoveJob(L"Job 1");
    CheckJobList(QStringList({"Job 2"}));
    QCOMPARE(result, true);
}

void ClientWorkManagerTest::testRemoveAllButJobs()
{
    manager->AddJob(new DummyJob(L"Job 1"));
    manager->AddJob(new DummyJob(L"Job 2"));
    manager->AddJob(new DummyJob(L"Job 3"));

    bool result = manager->RemoveAllButJobs(L"Job 3");
    CheckJobList(QStringList({"Job 3"}));
    QCOMPARE(result, true);

    result = manager->RemoveAllButJobs(L"Job 3");
    CheckJobList(QStringList({"Job 3"}));
    QCOMPARE(result, false);

    result = manager->RemoveAllButJobs(L"Invalid Job");
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
                                   << QStringList({okMessage, okWithWarningsMessage, errorMessage, notExecutedMessage})
                                   << QStringList({"first description", "second description",
                                                   "third description", "last description"})
                                   << QStringList({"", "", "", ""});

    QTest::newRow("Mixed list") << QStringList({"Simple job 1","Simple Job 2", "Warning job",
                                                "Error job", "No init job"})
                                   << QStringList({okMessage,okMessage,okWithWarningsMessage,errorMessage,notExecutedMessage})
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
    std::vector<AbstractJob*> jobList;
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
        QCOMPARE(QString::fromStdWString((*itResult)->GetName()), *itExpected);
}

void ClientWorkManagerTest::SetupJobList()
{
    QFETCH(QStringList, expectedNames);
    QFETCH(QStringList, expectedCodes);
    QFETCH(QStringList, expectedDescriptions);
    QFETCH(QStringList, failingInitializations);

    for (int i=0; i<expectedNames.size(); ++i)
    {
        DummyJob* newJob = new DummyJob(
                 expectedNames.at(i).toStdWString(),
                 JobStatus::GetCodeFromDescription(expectedCodes.at(i).toStdWString()),
                 expectedDescriptions.at(i).toStdWString()
        );
        if (failingInitializations.at(i) != "")
            newJob->SetInitialization(false);
        manager->AddJob(newJob);
    }
}

void ClientWorkManagerTest::CheckGlobalResult(WorkResultData *result)
{
    QCOMPARE(result->allClientsResults.size(), 1ul);
    QCOMPARE(
      QString::fromStdWString(result->allClientsResults.front().first),
      QString::fromStdWString(clientName)
    );
}

void ClientWorkManagerTest::CheckClientResult(ClientJobResults *results)
{
    QFETCH(QStringList, expectedNames);
    QFETCH(QStringList, expectedCodes);
    QFETCH(QStringList, expectedDescriptions);

    vector<wstring> jobNames;
    GetJobNames(results, jobNames);
    TestUtils::CheckListsAreEqual(expectedNames, jobNames);

    vector<wstring> descriptions;
    GetJobDescriptions(results, descriptions);
    TestUtils::CheckListsAreEqual(expectedDescriptions, descriptions);

    vector<wstring> codes;
    GetJobCodes(results, codes);
    TestUtils::CheckListsAreEqual(expectedCodes, codes);
}

void ClientWorkManagerTest::GetJobNames(ClientJobResults *results, std::vector<wstring>& values)
{
    for (auto it = results->begin(); it != results->end(); ++it)
        values.push_back(it->first);
}

void ClientWorkManagerTest::GetJobCodes(ClientJobResults *results, std::vector<wstring>& values)
{
    for (auto it = results->begin(); it != results->end(); ++it)
        values.push_back(it->second->GetCodeDescription());
}

void ClientWorkManagerTest::GetJobDescriptions(ClientJobResults *results, std::vector<wstring>& values)
{
    for (auto it = results->begin(); it != results->end(); ++it)
        values.push_back(it->second->GetDescription());
}
