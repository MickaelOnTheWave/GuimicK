#ifndef WAKEJOBTEST_H
#define WAKEJOBTEST_H

#include "client.h"
#include "qttestsuite.h"

class WakeJobTest : public QtTestSuite
{
   Q_OBJECT
public:
   WakeJobTest();
   virtual ~WakeJobTest() = default;

private Q_SLOTS:

    void testWake_FailsWakingInvalidMachine();

private:
    Client* CreateInvalidClient();
};

#endif // WAKEJOBTEST_H
