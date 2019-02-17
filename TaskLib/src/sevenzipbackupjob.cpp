#include "sevenzipbackupjob.h"

#include "sevenziptool.h"

using namespace std;

const string defaultName = "7zip Backup";

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

std::string SevenZipBackupJob::GetTypeName() const
{
   return defaultName;
}
