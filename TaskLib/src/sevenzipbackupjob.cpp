#include "sevenzipbackupjob.h"

#include "sevenziptool.h"

using namespace std;

const wstring defaultName = L"7zip Backup";

SevenZipBackupJob::SevenZipBackupJob()
   : ArchiveFsBackupJob(defaultName, new SevenZipTool())
{
}

SevenZipBackupJob::SevenZipBackupJob(const SevenZipBackupJob& other)
 : ArchiveFsBackupJob(other)
{
}

SevenZipBackupJob::~SevenZipBackupJob()
{
}

AbstractJob* SevenZipBackupJob::Clone()
{
   return new SevenZipBackupJob(*this);
}

std::wstring SevenZipBackupJob::GetTypeName() const
{
   return defaultName;
}

bool SevenZipBackupJob::InitializeFromClient(Client* client)
{
   bool ok = ArchiveFsBackupJob::InitializeFromClient(client);
   if (ok)
   {
      const wstring manualSevenZip = client->GetProperty(L"7zipExecutable");
      if (manualSevenZip != L"")
      {
         SevenZipTool* tool = static_cast<SevenZipTool*>(archiveTool);
         tool->SetExecutablePath(manualSevenZip);
      }
   }
   return ok;
}
