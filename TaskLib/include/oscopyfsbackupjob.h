#ifndef OSCOPYFSBACKUPJOB_H
#define OSCOPYFSBACKUPJOB_H

#ifdef _WIN32
   #include "WindowsCopyFsBackupJob.h"
#else
   #include "linuxcopyfsbackupjob.h"
#endif

#endif // OSCOPYFSBACKUPJOB_H
