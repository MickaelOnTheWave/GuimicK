#ifndef TARTOOLS_H
#define TARTOOLS_H

#include <string>

#include "abstractbackupjob.h"
#include "AbstractConsoleJob.h"
#include "jobexecutiontarget.h"

class TarTools
{
public:
   TarTools(JobDebugInformationManager* _parentDebugManager,
            JobExecutionTarget* _target);

   bool CreateArchive(const std::string& commandLineParameters,
                      AbstractBackupJob::ResultCollection &results);

private:
   AbstractConsoleJob *CreateBackupConsoleJob(const std::string& parameters);

   JobDebugInformationManager* parentDebugManager;
   JobExecutionTarget* target;
};

#endif // TARTOOLS_H
