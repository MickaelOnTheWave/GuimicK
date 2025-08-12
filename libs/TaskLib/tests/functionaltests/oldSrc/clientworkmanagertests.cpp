/*
 * GuimmicK
 * Copyright (C) 2024 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch2/catch.hpp>

#include "clientworkmanagertestfixture.h"
#include "clientworkmanager.h"
#include "dummyjob.h"

using namespace std;

static const char* okMessage                = "Ok";
static const char* okWithWarningsMessage    = "Executed with minor errors";
static const char* errorMessage             = "Error";
static const char* notExecutedMessage       = "Not executed";

TEST_CASE_METHOD(ClientWorkManagerTestFixture, "ClientWorkManager - AddJob()/RemoveJob()")
{
   CheckJobList({});

   manager->AddJob(new DummyJob(L"Job 1"));
   manager->AddJob(new DummyJob(L"Job 2"));
   manager->AddJob(new DummyJob(L"Job 3"));

   CheckJobList({L"Job 1", L"Job 2", L"Job 3"});

   manager->RemoveJob(L"Job 1");
   manager->RemoveJob(L"Job 2");
   manager->RemoveJob(L"Job 3");

   CheckJobList({});
}

TEST_CASE_METHOD(ClientWorkManagerTestFixture, "ClientWorkManager - RemoveJob()")
{
   manager->AddJob(new DummyJob(L"Job 1"));
   manager->AddJob(new DummyJob(L"Job 2"));

   bool result = manager->RemoveJob(L"Invalid Job");
   CheckJobList({L"Job 1", L"Job 2"});
   REQUIRE(result == false);

   result = manager->RemoveJob(L"Job 1");
   CheckJobList({L"Job 2"});
   REQUIRE(result == true);
}

TEST_CASE_METHOD(ClientWorkManagerTestFixture, "ClientWorkManager - RemoveAllButJobs()")
{
   manager->AddJob(new DummyJob(L"Job 1"));
   manager->AddJob(new DummyJob(L"Job 2"));
   manager->AddJob(new DummyJob(L"Job 3"));

   bool result = manager->RemoveAllButJobs(L"Job 3");
   CheckJobList({L"Job 3"});
   REQUIRE(result == true);

   result = manager->RemoveAllButJobs(L"Job 3");
   CheckJobList({L"Job 3"});
   REQUIRE(result == false);

   result = manager->RemoveAllButJobs(L"Invalid Job");
   CheckJobList({});
   REQUIRE(result == true);
}

TEST_CASE_METHOD(ClientWorkManagerTestFixture, "ClientWorkManager - RunWorkList()")
{
   // TODO reimplement test with data here
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
