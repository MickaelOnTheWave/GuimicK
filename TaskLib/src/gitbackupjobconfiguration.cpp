#include "gitbackupjobconfiguration.h"
#include "gitbackupjob.h"

using namespace std;

const wstring GitBackupJobConfiguration::WriteLogProperty = L"writeLogsToFiles";

GitBackupJobConfiguration::GitBackupJobConfiguration()
    : AbstractBackupJobConfiguration(L"GitBackup")
{
}

bool GitBackupJobConfiguration::IsRightJob(AbstractJob* job)
{
   return (dynamic_cast<GitBackupJob*>(job) != NULL);
}

AbstractJob *GitBackupJobConfiguration::CreateJob()
{
    return new GitBackupJob();
}

void GitBackupJobConfiguration::ConfigureJob(AbstractJob *job,
                                             ConfigurationObject *confObject,
                                             std::vector<wstring> &errorMessages)
{
    AbstractBackupJobConfiguration::ConfigureJob(job, confObject, errorMessages);

    GitBackupJob* castJob = static_cast<GitBackupJob*>(job);
    const wstring writeLogsToFiles(confObject->GetProperty(WriteLogProperty));
    if (writeLogsToFiles == L"true")
        castJob->SetWriteLogsToFiles(true);

}

void GitBackupJobConfiguration::FillKnownProperties(std::vector<std::wstring> &properties)
{
    AbstractBackupJobConfiguration::FillKnownProperties(properties);
    properties.push_back(WriteLogProperty);
}

wstring GitBackupJobConfiguration::GetBackupItemName() const
{
    return wstring(L"Repository");
}
