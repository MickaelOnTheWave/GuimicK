#include "rsnapshotsmartcreator.h"

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

RsnapshotSmartCreator::RsnapshotSmartCreator(const string& repositoryPath)
    : repository(repositoryPath), templateFile(""),
      configurationFile(defaultConfigurationFile)
{
}

void RsnapshotSmartCreator::SetTemplateConfigurationFile(const string &file)
{
    templateFile = file;
}

void RsnapshotSmartCreator::SetGeneratedConfigurationFile(const string &file)
{
    configurationFile = file;
}

void RsnapshotSmartCreator::AddFolderToBackup(const string &folder,
                                              const string &destination)
{
    dataToBackup.push_back(make_pair(folder, destination));
}

RsnapshotBackupJob *RsnapshotSmartCreator::CreateConfiguredJob()
{
    BuildConfigurationFile();
    return new RsnapshotSmartBackupJob(repository, configurationFile);
}

void RsnapshotSmartCreator::BuildConfigurationFile()
{
    string configurationContent = GetTemplateConfiguration();
    AppendBackupData(configurationContent);
    CheckAndFixConfigurationFile();
    FileTools::WriteBufferToFile(configurationFile, configurationContent);
}

string RsnapshotSmartCreator::GetTemplateConfiguration() const
{
    if (templateFile != "")
        return FileTools::GetTextFileContent(templateFile);
    else
        return defaultTemplateConfiguration;
}

void RsnapshotSmartCreator::AppendBackupData(string &configurationData) const
{
    configurationData += string("\nsnapshot_root\t") + repository + "\n";

    vector<pair<string,string> >::const_iterator it=dataToBackup.begin();
    for (; it!=dataToBackup.end(); ++it)
    {
        configurationData += string("\nbackup\t") + it->first;
        configurationData += "\t" + it->second + "\n";
    }
}

void RsnapshotSmartCreator::CheckAndFixConfigurationFile()
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
