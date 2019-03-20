#ifndef TARBACKUPJOB_H
#define TARBACKUPJOB_H

#include "archivefsbackupjob.h"

class TarBackupJob : public ArchiveFsBackupJob
{
public:
   TarBackupJob();
   TarBackupJob(const TarBackupJob& other);
   virtual ~TarBackupJob();

   virtual AbstractJob* Clone();

   virtual std::wstring GetTypeName() const;
};

#endif // TARBACKUPJOB_H
