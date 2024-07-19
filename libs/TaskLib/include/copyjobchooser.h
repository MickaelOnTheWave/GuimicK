#ifndef COPYJOBCHOOSER_H
#define COPYJOBCHOOSER_H

#include "abstractcopyfsbackupjob.h"

namespace CopyJobChooser
{
    AbstractCopyFsBackupJob* GetBestAvailable();
};

#endif // COPYJOBCHOOSER_H
