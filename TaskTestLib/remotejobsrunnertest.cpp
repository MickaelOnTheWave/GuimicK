#include "remotejobsrunnertest.h"

#include <QTest>

#include "clientworkmanager.h"
#include "dummyjob.h"
#include "remotejobsrunner.h"

using namespace std;

static const string testingHost = "127.0.0.1";
static const string testingUser = "mickael";

RemoteJobsRunnerTest::RemoteJobsRunnerTest()
   : QtTestSuite()
{
}

void RemoteJobsRunnerTest::testRun()
{
/*   Client* client = CreateInitializedClient();
   InsertFilledRemoteJob(client, 3);

   ClientWorkManager* workManager = new ClientWorkManager(client, false);
   WorkResultData* resultData = workManager->RunWorkList();
   CheckResultJobList(resultData, 4);

   delete workManager;
   delete client;*/

   QFAIL("Check if there is a relevant test to be made");
}

Client* RemoteJobsRunnerTest::CreateInitializedClient()
{
   Client* client = new Client("Testing Client");
   client->AddProperty("ip", testingHost);
   client->AddProperty("sshuser", testingUser);
   client->AddJob(new DummyJob("Server Job", JobStatus::OK, "server desc"));
   return client;
}

void RemoteJobsRunnerTest::InsertFilledRemoteJob(Client* client, const int remoteJobsCount)
{
/*   RemoteJobsRunner* remoteJob = new RemoteJobsRunner();

   for (int i=0; i<remoteJobsCount; ++i)
      remoteJob->AddJob(CreateDummyJob("Remote", i));

   client->AddJob(remoteJob);*/
}

AbstractJob* RemoteJobsRunnerTest::CreateDummyJob(const std::string& tag, const int index)
{
   stringstream title;
   title << tag << " job " << index;

   stringstream description;
   description << tag << " description " << index;
   return new DummyJob(title.str(), JobStatus::OK, description.str());
}

void RemoteJobsRunnerTest::CheckResultJobList(WorkResultData* result,
                                              const unsigned long expectedCount)
{
   QCOMPARE(result->allClientsResults.size(), 1ul);
   ClientJobResults* clientResults = result->allClientsResults.front().second;
   QCOMPARE(clientResults->size(), expectedCount);
}
