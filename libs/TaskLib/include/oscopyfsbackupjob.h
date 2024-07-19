#ifndef OSCOPYFSBACKUPJOB_H
#define OSCOPYFSBACKUPJOB_H

#ifdef _WIN32
   #include "WindowsCopyFsBackupJob.h"
   typedef WindowsCopyFsBackupJob OsCopyFsBackupJob;
#else
   #include "linuxcopyfsbackupjob.h"
   typedef LinuxCopyFsBackupJob OsCopyFsBackupJob;
#endif

#endif // OSCOPYFSBACKUPJOB_H
