#include "windowscheckdiskjob.h"

#include "chkdskcommanderrorparser.h"
#include "chkdskcommandparser.h"
#include "consolejob.h"

using namespace std;

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

AbstractConsoleJob* WindowsCheckdiskJob::CreateJobInstance() const
{
    const wstring parameters = drive + wstring(L" /f");
    return new ConsoleJob(L"chkdsk", parameters);
}

AbstractOutputParser* WindowsCheckdiskJob::CreateOutputParser(const int jobStatus) const
{
   if (jobStatus == JobStatus::Ok)
    return new ChkdskCommandParser();
   else
    return new ChkdskCommandErrorParser();
}
