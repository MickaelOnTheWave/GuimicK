#include "diskrelatedjob.h"

using namespace std;

DiskRelatedJob::DiskRelatedJob(const wstring& jobName,
                               const wstring& _drive)
    : CommandBasedJob(jobName),
      drive(_drive)
{
}

DiskRelatedJob::DiskRelatedJob(const DiskRelatedJob& other)
    : CommandBasedJob(other),
      drive(other.drive)
{
}

wstring DiskRelatedJob::GetDrive() const
{
    return drive;
}

void DiskRelatedJob::SetDrive(const wstring &value)
{
    drive = value;
}
