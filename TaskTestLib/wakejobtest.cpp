#include "wakejobtest.h"

#include <QTest>
#include "client.h"
#include "wakejob.h"

WakeJobTest::WakeJobTest()
   : QtTestSuite("", "")
{
}

void WakeJobTest::testWake_FailsWakingInvalidMachine()
{
   // TODO : once wake job has its own code, activate this test.
   QSKIP("once wake job has its own code, activate this test");

/*   auto wakeJob = new WakeJob();
   wakeJob->SetMaxRetries(1);
   wakeJob->SetTimeout(1);

   Client* invalidClient = CreateInvalidClient();
   bool ok = wakeJob->InitializeFromClient(invalidClient);
   QCOMPARE(ok, true);

   JobStatus* status = wakeJob->Run();
   QCOMPARE(status->GetCode(), JobStatus::ERROR);
   QCOMPARE(status->GetDescription().c_str(), "");

   delete invalidClient;
   delete wakeJob;*/
}

Client* WakeJobTest::CreateInvalidClient()
{
   auto client = new Client("Invalid Client");
   client->AddProperty("mac", "00:11:22:33:44:55");
   client->AddProperty("broadcast", "0.0.0.255");
   client->AddProperty("ip", "0.0.0.1");
   return client;
}
