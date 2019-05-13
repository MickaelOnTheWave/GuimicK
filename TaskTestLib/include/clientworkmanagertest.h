#ifndef CLIENTWORKMANAGERTEST_H
#define CLIENTWORKMANAGERTEST_H

#include "qttestsuite.h"

#include "abstractjob.h"
#include "clientworkmanager.h"

class ClientWorkManagerTest : public QtTestSuite
{
    Q_OBJECT
public:
    ClientWorkManagerTest();
    virtual ~ClientWorkManagerTest() {}

private Q_SLOTS:
    void init();
    void cleanup();

    void testAddRemoveJob();
    void testRemoveJob();
    void testRemoveAllButJobs();

    void testRunWorkList_data();
    void testRunWorkList();

private:
    void CheckJobList(const QStringList& expectedList);

    void SetupJobList();
    void CheckGlobalResult(WorkResultData* result);
    void CheckClientResult(ClientJobResults* results);

    void GetJobNames(ClientJobResults* results, std::vector<std::wstring>& values);
    void GetJobCodes(ClientJobResults* results, std::vector<std::wstring>& values);
    void GetJobDescriptions(ClientJobResults* results, std::vector<std::wstring>& values);

    ClientWorkManager* manager;
};

#endif // CLIENTWORKMANAGERTEST_H
