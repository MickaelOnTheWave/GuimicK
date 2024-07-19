#include "tarbackupjob.h"

#include "tartool.h"

using namespace std;

const wstring defaultName = L"Tar Backup";

TarTool* CreateConfiguredTool()
{
   TarTool* tool = new TarTool();
   tool->SetGzipCompression(false);
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

std::wstring TarBackupJob::GetTypeName() const
{
   return defaultName;
}
