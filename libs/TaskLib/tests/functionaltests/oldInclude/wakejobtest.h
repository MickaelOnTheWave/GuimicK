#ifndef WAKEJOBTEST_H
#define WAKEJOBTEST_H

#include "client.h"
#include "jobstatus.h"
#include "qttestsuite.h"

class WakeJobTest : public QtTestSuite
{
   Q_OBJECT
public:
   WakeJobTest();
   virtual ~WakeJobTest() = default;

private Q_SLOTS:

   void testWake_OkOnSelf();
   void testWake_FailsWakingInvalidMachine();

private:
   void TestWake(Client* client,
                 JobStatus* expectedStatus);
   Client* CreateSelfClient();
   Client* CreateInvalidClient();
};

#endif // WAKEJOBTEST_H
