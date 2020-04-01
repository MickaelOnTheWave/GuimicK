#include "windowscheckdiskjob.h"

#include "chkdskcommanderrorparser.h"
#include "chkdskcommandparser.h"
#include "consolejob.h"
#include "ostools.h"

using namespace std;

JobStatus* CreateNoAdminRightsStatus()
{
   return new JobStatus(JobStatus::Error, L"Couldn't get admin rights");
}

//----------------------------------------------------------------------

WindowsCheckdiskJob::WindowsCheckdiskJob(const wstring& _drive)
   : DiskRelatedJob(L"HD Integrity Check", _drive)
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
   if (OsTools::IsRunningAsAdministrator())
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
    return new ConsoleJob(L"chkdsk", GetWindowsDrive());
}

AbstractOutputParser* WindowsCheckdiskJob::CreateOutputParser(const int jobStatus) const
{
   if (jobStatus == JobStatus::Ok)
    return new ChkdskCommandParser();
   else
    return new ChkdskCommandErrorParser();
}

std::wstring WindowsCheckdiskJob::GetWindowsDrive() const
{
   if (drive != L"")
      return drive.substr(0, drive.length()-1);
   else
      return L"";
}
