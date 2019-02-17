#ifndef SEVENZIPBACKUPJOB_H
#define SEVENZIPBACKUPJOB_H

#include "archivefsbackupjob.h"

class SevenZipBackupJob : public ArchiveFsBackupJob
{
public:
   SevenZipBackupJob();
   SevenZipBackupJob(const SevenZipBackupJob& other);
   virtual ~SevenZipBackupJob();

   virtual AbstractJob* Clone();

   virtual std::string GetTypeName() const;
};

#endif // SEVENZIPBACKUPJOB_H
