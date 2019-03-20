#include "rsnapshotconfigurationbuilder.h"

#include <sstream>

#include "filetools.h"
#include "pathtools.h"
#include "rsnapshotsmartbackupjob.h"

using namespace std;

const wstring defaultConfigurationFile = L"rsnapshotConfigurationFile.conf";

static const wstring defaultTemplateConfiguration =
        L"config_version	1.2\n"
        L"cmd_rm		/opt/bin/rm\n"
        L"cmd_rsync	/opt/bin/rsync\n"
        L"cmd_ssh	/opt/bin/ssh\n"
        L"verbose		3\n"
        L"loglevel	4\n"
        L"logfile	/var/log/rsnapshot\n"
        L"lockfile	/var/run/rsnapshot.pid\n"
        L"rsync_long_args	--delete --numeric-ids --delete-excluded\n";

RsnapshotConfigurationBuilder::RsnapshotConfigurationBuilder(const wstring &templateConfigurationFile)
    : templateFile(templateConfigurationFile), configurationFile(defaultConfigurationFile),
      repository(L"")
{
}

void RsnapshotConfigurationBuilder::SetRepository(const wstring &value)
{
    repository = value;
}

void RsnapshotConfigurationBuilder::SetTemplateConfigurationFile(const wstring &file)
{
    templateFile = file;
}

void RsnapshotConfigurationBuilder::SetGeneratedConfigurationFile(const wstring &file)
{
    configurationFile = file;
}

wstring RsnapshotConfigurationBuilder::CreateConfigurationFile(
        const AbstractBackupJob::BackupCollection &dataToBackup,
        const int maxBackups)
{
    BuildConfigurationFile(dataToBackup, maxBackups);
    return configurationFile;
}

void RsnapshotConfigurationBuilder::BuildConfigurationFile(
        const AbstractBackupJob::BackupCollection &dataToBackup,
        const int maxBackups)
{
    wstring configurationContent = GetTemplateConfiguration();
    AppendMaxBackups(configurationContent, maxBackups);
    AppendBackupData(configurationContent, dataToBackup);
    CheckAndFixConfigurationFile();
    FileTools::WriteBufferToFile(configurationFile, configurationContent);
}

wstring RsnapshotConfigurationBuilder::GetTemplateConfiguration() const
{
    if (templateFile != L"")
        return FileTools::GetTextFileContent(templateFile);
    else
        return defaultTemplateConfiguration;
}

void RsnapshotConfigurationBuilder::AppendMaxBackups(wstring &configurationData, const int maxBackups)
{
    wstringstream stream;
    stream << "interval	weekly	" << maxBackups << endl;
    configurationData +=  stream.str();
}

void RsnapshotConfigurationBuilder::AppendBackupData(
        wstring &configurationData,
        const AbstractBackupJob::BackupCollection &dataToBackup) const
{
   const wstring absoluteRepository = PathTools::BuildFullPathIfRelative(repository);
    configurationData += wstring(L"\nsnapshot_root\t") + absoluteRepository + L"\n";

    vector<pair<wstring,wstring> >::const_iterator it=dataToBackup.begin();
    for (; it!=dataToBackup.end(); ++it)
    {
        configurationData += wstring(L"\nbackup\t") + it->first;
        configurationData += L"\t" + PathTools::RelativePath(it->second) + L"\n";
    }
}

void RsnapshotConfigurationBuilder::CheckAndFixConfigurationFile()
{
    wstring newConfigurationFile = configurationFile;
    int counter = 0;
    while (FileTools::FileExists(newConfigurationFile))
    {
        wstringstream stream;
        stream << configurationFile << counter++;
        newConfigurationFile = stream.str();
    }

    configurationFile = newConfigurationFile;
}
