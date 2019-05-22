#include "windowscheckdiskjobconfiguration.h"

#include <string>
#include "windowscheckdiskjob.h"

using namespace std;

const wstring defaultJobName = L"DiskCheck";

WindowsCheckDiskJobConfiguration::WindowsCheckDiskJobConfiguration()
    : DiskRelatedJobConfiguration(defaultJobName)
{
}

bool WindowsCheckDiskJobConfiguration::IsRightJob(AbstractJob *job)
{
    return (dynamic_cast<WindowsCheckdiskJob*>(job) != NULL);
}

AbstractJob *WindowsCheckDiskJobConfiguration::CreateJob()
{
    return new WindowsCheckdiskJob();
}
