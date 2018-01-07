#include "jobeditdialogfactory.h"

#include "gitbackupjob.h"
#include "gitfsbackupjob.h"
#include "linuxfreespacecheckjob.h"
#include "linuxshutdownjob.h"
#include "rawcopyfsbackupjob.h"
#include "rsnapshotsmartbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "userconsolejob.h"
#include "wakejob.h"
#include "zipandcopyfsbackupjob.h"

#include "editbackupjobdialog.h"
#include "editconsolejobdialog.h"
#include "editdiskspacejobdialog.h"
#include "editgitfsbackupjobdialog.h"
#include "editrsnapshotbackupjobdialog.h"
#include "editrsyncfsbackupjobdialog.h"
#include "editshutdownjobdialog.h"
#include "editwakejobdialog.h"

AbstractEditJobDialog* JobEditDialogFactory::Create(AbstractJob* job)
{
   if (dynamic_cast<WakeJob*>(job))
      return new EditWakeJobDialog(job);
   else if (dynamic_cast<LinuxShutdownJob*>(job))
      return new EditShutdownJobDialog(job);
   else if (dynamic_cast<AbstractConsoleJob*>(job))
      return new EditConsoleJobDialog(job);
   else if (dynamic_cast<AbstractBackupJob*>(job))
   {
      if (dynamic_cast<RsnapshotSmartBackupJob*>(job))
         return new EditRsnapshotBackupJobDialog(job);
      else if (dynamic_cast<GitFsBackupJob*>(job))
         return new EditGitFsBackupJobDialog(job);
      else if (dynamic_cast<RsyncCopyFsBackupJob*>(job))
         return new EditRsyncFsBackupJobDialog(job);
      else
         return new EditBackupJobDialog(job);
   }
   else if (dynamic_cast<LinuxFreeSpaceCheckJob*>(job))
      return new EditDiskSpaceJobDialog(job);
   else
      return nullptr;
}
