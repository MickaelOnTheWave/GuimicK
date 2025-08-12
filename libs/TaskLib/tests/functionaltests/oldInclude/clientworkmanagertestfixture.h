#ifndef CLIENTWORKMANAGERTESTFIXTURE_H
#define CLIENTWORKMANAGERTESTFIXTURE_H

#include "tools.h"
#include "clientworkmanager.h"

class ClientWorkManagerTestFixture
{
public:
   ClientWorkManagerTestFixture();
   virtual ~ClientWorkManagerTestFixture();

protected:
   void CheckJobList(const Tools::WStringVec& expectedNames);

   void SetupJobList();
   void CheckGlobalResult(WorkResultData* result);
   void CheckClientResult(ClientJobResults* results);

   void GetJobNames(ClientJobResults* results, std::vector<std::wstring>& values);
   void GetJobCodes(ClientJobResults* results, std::vector<std::wstring>& values);
   void GetJobDescriptions(ClientJobResults* results, std::vector<std::wstring>& values);

   ClientWorkManager* manager;
};

#endif // CLIENTWORKMANAGERTESTFIXTURE_H
