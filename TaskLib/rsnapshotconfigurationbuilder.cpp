#include "rsnapshotconfigurationbuilder.h"

#include <sstream>
#include <unistd.h>

#include "filetools.h"
#include "rsnapshotsmartbackupjob.h"

using namespace std;

const string defaultConfigurationFile = "rsnapshotConfigurationFile.conf";

static const string defaultTemplateConfiguration =
        "config_version	1.2\n"
        "cmd_rm		/opt/bin/rm\n"
        "cmd_rsync	/opt/bin/rsync\n"
        "cmd_ssh	/opt/bin/ssh\n"
        "interval	weekly	500\n"
        "interval	monthly	12\n"
        "interval	yearly	2\n"
        "verbose		3\n"
        "loglevel	4\n"
        "logfile	/var/log/rsnapshot\n"
        "lockfile	/var/run/rsnapshot.pid\n"
        "rsync_long_args	--delete --numeric-ids --delete-excluded\n";

RsnapshotConfigurationBuilder::RsnapshotConfigurationBuilder(const string &templateConfigurationFile)
    : templateFile(templateConfigurationFile), configurationFile(defaultConfigurationFile),
      repository("")
{
}

void RsnapshotConfigurationBuilder::SetRepository(const string &value)
{
    repository = value;
}

void RsnapshotConfigurationBuilder::SetTemplateConfigurationFile(const string &file)
{
    templateFile = file;
}

void RsnapshotConfigurationBuilder::SetGeneratedConfigurationFile(const string &file)
{
    configurationFile = file;
}

string RsnapshotConfigurationBuilder::CreateConfigurationFile(
        const AbstractBackupJob::BackupCollection &dataToBackup)
{
    BuildConfigurationFile(dataToBackup);
    return configurationFile;
}

void RsnapshotConfigurationBuilder::BuildConfigurationFile(
        const AbstractBackupJob::BackupCollection &dataToBackup)
{
    string configurationContent = GetTemplateConfiguration();
    AppendBackupData(configurationContent, dataToBackup);
    CheckAndFixConfigurationFile();
    FileTools::WriteBufferToFile(configurationFile, configurationContent);
}

string RsnapshotConfigurationBuilder::GetTemplateConfiguration() const
{
    if (templateFile != "")
        return FileTools::GetTextFileContent(templateFile);
    else
        return defaultTemplateConfiguration;
}

void RsnapshotConfigurationBuilder::AppendBackupData(
        string &configurationData,
        const AbstractBackupJob::BackupCollection &dataToBackup) const
{
    configurationData += string("\nsnapshot_root\t") + repository + "\n";

    vector<pair<string,string> >::const_iterator it=dataToBackup.begin();
    for (; it!=dataToBackup.end(); ++it)
    {
        configurationData += string("\nbackup\t") + it->first;
        configurationData += "\t" + it->second + "\n";
    }
}

void RsnapshotConfigurationBuilder::CheckAndFixConfigurationFile()
{
    string newConfigurationFile = configurationFile;
    int counter = 0;
    while (FileTools::FileExists(newConfigurationFile))
    {
        stringstream stream;
        stream << configurationFile << counter++;
        newConfigurationFile = stream.str();
    }

    configurationFile = newConfigurationFile;
}
