#include "jobeditdialogfactory.h"

#include "archivefsbackupjob.h"
#include "gitbackupjob.h"
#include "gitfsbackupjob.h"
#include "linuxfreespacecheckjob.h"
#include "linuxshutdownjob.h"
#include "rawcopyfsbackupjob.h"
#include "rsnapshotsmartbackupjob.h"
#include "rsynccopyfsbackupjob.h"
#include "userconsolejob.h"
#include "abstractwakejob.h"

#include "editbackupjobdialog.h"
#include "editconsolejobdialog.h"
#include "editdiskspacejobdialog.h"
#include "editgitfsbackupjobdialog.h"
#include "editrsnapshotbackupjobdialog.h"
#include "editrsyncfsbackupjobdialog.h"
#include "editshutdownjobdialog.h"
#include "editwakejobdialog.h"
#include "editzipcopybackupdialog.h"

AbstractEditJobDialog* JobEditDialogFactory::Create(AbstractJob* job)
{
   if (dynamic_cast<AbstractWakeJob*>(job))
      return new EditWakeJobDialog(job);
   else if (dynamic_cast<LinuxShutdownJob*>(job))
      return new EditShutdownJobDialog(job);
   else if (dynamic_cast<AbstractConsoleJob*>(job))
      return new EditConsoleJobDialog(job);
   else if (dynamic_cast<AbstractBackupJob*>(job))
      return CreateBackupDialog(job);
   else if (dynamic_cast<LinuxFreeSpaceCheckJob*>(job))
      return new EditDiskSpaceJobDialog(job);
   else
      return nullptr;
}

AbstractEditJobDialog* JobEditDialogFactory::CreateBackupDialog(AbstractJob* job)
{
   if (dynamic_cast<RsnapshotSmartBackupJob*>(job))
      return new EditRsnapshotBackupJobDialog(job);
   else if (dynamic_cast<GitFsBackupJob*>(job))
      return new EditGitFsBackupJobDialog(job);
   else if (dynamic_cast<RsyncCopyFsBackupJob*>(job))
      return new EditRsyncFsBackupJobDialog(job);
   else if (dynamic_cast<ArchiveFsBackupJob*>(job))
      return new EditZipCopyBackupDialog(job);
   else
      return new EditBackupJobDialog(job);
}
