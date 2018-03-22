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

   bool CreateIncrementalArchive(const std::string& commandLineParameters,
                                 const std::string& currentArchive,
                                 const std::string& referenceArchive,
                                 AbstractBackupJob::ResultCollection &results);

   bool ExtractArchive(const std::string& archiveName,
                       const std::string& destination);

private:
   AbstractConsoleJob *CreateBackupConsoleJob(const std::string& parameters);

   FileBackupReport* CreateReportFromArchives(const std::string& referenceArchive,
                                              const std::string& currentArchive);

   void GetArchiveFileList(const std::string& archive,
                           std::vector<std::string>& fileList);

   void RemovePathHeaders(std::vector<std::string>& fileList);

   void RemoveCurrentDirTag(std::vector<std::string>& fileList);

   FileBackupReport* CreateReportFromFileLists(
         const std::vector<std::string>& baseFileList,
         const std::vector<std::string>& newFileList);


   JobExecutionTarget* target;
   JobDebugInformationManager* parentDebugManager;
};

#endif // TARTOOLS_H
