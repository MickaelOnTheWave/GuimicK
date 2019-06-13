#include "windowscheckdiskjob.h"

#include "chkdskcommanderrorparser.h"
#include "chkdskcommandparser.h"
#include "consolejob.h"

using namespace std;

JobStatus* CreateNoAdminRightsStatus()
{
   return new JobStatus(JobStatus::Error, L"Couldn't get admin rights");
}

bool GetAdminRights()
{
   return false;
}

//----------------------------------------------------------------------

WindowsCheckdiskJob::WindowsCheckdiskJob(const wstring& _drive)
   : DiskRelatedJob(L"Disk checking", _drive)
{
}

WindowsCheckdiskJob::WindowsCheckdiskJob(const WindowsCheckdiskJob& other)
   : DiskRelatedJob(other)
{
}

AbstractJob* WindowsCheckdiskJob::Clone()
{
   return new WindowsCheckdiskJob(*this);
}

bool WindowsCheckdiskJob::InitializeFromClient(Client *client)
{
    return DiskRelatedJob::InitializeFromClient(client);
}

bool WindowsCheckdiskJob::IsInitialized()
{
   return (drive != L"");
}

JobStatus* WindowsCheckdiskJob::Run()
{
   const bool adminRightsObtained = GetAdminRights();
   if (adminRightsObtained)
      return DiskRelatedJob::Run();
   else
      return CreateNoAdminRightsStatus();
}

bool WindowsCheckdiskJob::NeedsAdminRights() const
{
   return true;
}

AbstractConsoleJob* WindowsCheckdiskJob::CreateJobInstance() const
{
    return new ConsoleJob(L"chkdsk", drive);
}

AbstractOutputParser* WindowsCheckdiskJob::CreateOutputParser(const int jobStatus) const
{
   if (jobStatus == JobStatus::Ok)
    return new ChkdskCommandParser();
   else
    return new ChkdskCommandErrorParser();
}
