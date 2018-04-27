#include "wakejobtest.h"

#include <QTest>
#include "client.h"
#include "libwakejob.h"

WakeJobTest::WakeJobTest()
   : QtTestSuite("", "")
{
}

void WakeJobTest::testWake_OkOnSelf()
{
   TestWake(CreateSelfClient(),
            new JobStatus(JobStatus::OK, ""));
}

void WakeJobTest::testWake_FailsWakingInvalidMachine()
{
   TestWake(CreateInvalidClient(),
            new JobStatus(JobStatus::ERROR, "Machine still not awake"));
}

void WakeJobTest::TestWake(Client* client, JobStatus* expectedStatus)
{
   auto wakeJob = new LibWakeJob();
   wakeJob->SetMaxRetries(1);
   wakeJob->SetTimeout(1);

   bool ok = wakeJob->InitializeFromClient(client);
   QCOMPARE(ok, true);

   JobStatus* status = wakeJob->Run();
   QCOMPARE(status->GetCode(), expectedStatus->GetCode());
   QCOMPARE(status->GetDescription().c_str(),
            expectedStatus->GetDescription().c_str());

   delete wakeJob;
   delete client;
   delete expectedStatus;
}

Client* WakeJobTest::CreateSelfClient()
{
   auto client = new Client("Self Client");
   client->AddProperty("mac", "00:11:22:33:44:55");
   client->AddProperty("broadcast", "0.0.0.255");
   client->AddProperty("ip", "127.0.0.1");
   return client;
}

Client* WakeJobTest::CreateInvalidClient()
{
   auto client = new Client("Invalid Client");
   client->AddProperty("mac", "00:11:22:33:44:55");
   client->AddProperty("broadcast", "0.0.0.255");
   client->AddProperty("ip", "0.0.0.1");
   return client;
}
