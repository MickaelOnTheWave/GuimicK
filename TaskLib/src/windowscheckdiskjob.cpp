#include "windowscheckdiskjob.h"

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
    const wstring parameters = wstring(L"-h ") + drive;
    return new ConsoleJob(L"df", parameters);
}

AbstractOutputParser* WindowsCheckdiskJob::CreateOutputParser() const
{
   //return new DfCommandParser();
   return nullptr;
}
