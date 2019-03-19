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

   bool CreateArchive(const std::wstring& commandLineParameters,
                      AbstractBackupJob::ResultCollection &results);

   bool CreateIncrementalArchive(const std::wstring& commandLineParameters,
                                 const std::wstring& currentArchive,
                                 const std::wstring& referenceArchive,
                                 AbstractBackupJob::ResultCollection &results);

   bool ExtractArchive(const std::wstring& archiveName,
                       const std::wstring& destination);

   bool ExtractIncrementalArchive(const std::wstring& baseArchiveName,
                                  const int archiveIndex,
                                  const std::wstring& destination);

private:
   AbstractConsoleJob *CreateBackupConsoleJob(const std::wstring& parameters);

   FileBackupReport* CreateReportFromArchives(const std::wstring& referenceArchive,
                                              const std::wstring& currentArchive);

   void GetArchiveFileList(const std::wstring& archive,
                           std::vector<std::wstring>& fileList);

   void RemovePathHeaders(std::vector<std::wstring>& fileList);

   void RemoveCurrentDirTag(std::vector<std::wstring>& fileList);

   FileBackupReport* CreateReportFromFileLists(
         const std::vector<std::wstring>& baseFileList,
         const std::vector<std::wstring>& newFileList);


   JobExecutionTarget* target;
   JobDebugInformationManager* parentDebugManager;
};

#endif // TARTOOLS_H
