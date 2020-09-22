#include "rsnapshotconfigurationbuilder.h"

#include <sstream>

#include "filetools.h"
#include "pathtools.h"
#include "rsnapshotsmartbackupjob.h"

using namespace std;

const wstring defaultConfigurationFile = L"rsnapshotConfigurationFile.conf";

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
       return CreateTemplateConfiguration();
}

wstring RsnapshotConfigurationBuilder::CreateTemplateConfiguration() const
{
   wstring templateConfiguration =
           L"config_version	1.2\n"
           L"cmd_rm\t" + GetCommandPath(L"rm") + L"\n"
           L"cmd_rsync\t" + GetCommandPath(L"rsync") + L"\n"
           L"cmd_ssh\t" + GetCommandPath(L"ssh") + L"\n"
           L"verbose		3\n"
           L"loglevel	4\n"
           L"logfile	/var/log/rsnapshot\n"
           L"lockfile	/var/run/rsnapshot.pid\n"
           L"rsync_long_args	--delete --numeric-ids --delete-excluded\n";
   return templateConfiguration;
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

wstring RsnapshotConfigurationBuilder::GetCommandPath(const wstring& command) const
{
   wstring absoluteCommand = PathTools::GetCommandPath(command, ConsoleJob::appSearchPaths);
   if (absoluteCommand == L"")
   {
      vector<wstring> standardPaths;
      standardPaths.push_back(L"/bin");
      standardPaths.push_back(L"/opt/bin");
      standardPaths.push_back(L"/usr/local/bin");
      absoluteCommand = PathTools::GetCommandPath(command, standardPaths);
   }
   return absoluteCommand;
}
