#ifndef WINDOWSCHECKDISKJOB_H
#define WINDOWSCHECKDISKJOB_H

#include "diskrelatedjob.h"

class WindowsCheckdiskJob : public DiskRelatedJob
{
public:
   WindowsCheckdiskJob(const std::wstring& _drive = L"");
   WindowsCheckdiskJob(const WindowsCheckdiskJob& other);

   virtual AbstractJob* Clone();

   virtual bool InitializeFromClient(Client* client);

   virtual bool IsInitialized(void);

   virtual JobStatus* Run();

   virtual bool NeedsAdminRights() const;

private:
   virtual AbstractConsoleJob *CreateJobInstance() const;
   virtual AbstractOutputParser* CreateOutputParser(const int jobStatus) const;

   std::wstring GetWindowsDrive() const;
};

#endif // WINDOWSCHECKDISKJOB_H
