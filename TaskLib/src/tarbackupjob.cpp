#include "tarbackupjob.h"

#include "tartool.h"

using namespace std;

const string defaultName = "Tar Backup";

TarTool* CreateConfiguredTool()
{
   TarTool* tool = new TarTool();
   tool->SetGzipCompression(true);
   return tool;
}

TarBackupJob::TarBackupJob()
   : ArchiveFsBackupJob(defaultName, CreateConfiguredTool())
{
}

TarBackupJob::TarBackupJob(const TarBackupJob& other)
 : ArchiveFsBackupJob(other)
{
}

TarBackupJob::~TarBackupJob()
{
}

AbstractJob* TarBackupJob::Clone()
{
   return new TarBackupJob(*this);
}

std::string TarBackupJob::GetTypeName() const
{
   return defaultName;
}
