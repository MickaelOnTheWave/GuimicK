#ifndef TARTOOLS_H
#define TARTOOLS_H

#include <string>

#include "abstractbackupjob.h"
#include "AbstractConsoleJob.h"
#include "jobexecutiontarget.h"

class TarTools
{
public:
   TarTools(JobExecutionTarget* _target = NULL,
            JobDebugInformationManager* _parentDebugManager = NULL);

   bool CreateArchive(const std::string& commandLineParameters,
                      AbstractBackupJob::ResultCollection &results);

   bool ExtractArchive(const std::string& archiveName,
                       const std::string& destination);

private:
   AbstractConsoleJob *CreateBackupConsoleJob(const std::string& parameters);

   JobExecutionTarget* target;
   JobDebugInformationManager* parentDebugManager;
};

#endif // TARTOOLS_H
