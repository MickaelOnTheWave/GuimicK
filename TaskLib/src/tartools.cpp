#include "tartools.h"

#include <algorithm>

#include "consolejob.h"
#include "jobdebuginformationmanager.h"
#include "sshconsolejob.h"
#include "tarcommandparser.h"
#include "tools.h"

using namespace std;

static const string reportCreationError = "Failed creating report";
static const string tarCommandError = "tar command failed";

TarTools::TarTools(JobExecutionTarget* _target,
                   JobDebugInformationManager* _parentDebugManager)
   : target(_target),
     parentDebugManager(_parentDebugManager)
{
}

bool TarTools::CreateArchive(const string& commandLineParameters,
                             AbstractBackupJob::ResultCollection& results)
{
   JobStatus* status = new JobStatus();

   AbstractConsoleJob* commandJob = CreateBackupConsoleJob(commandLineParameters);
   if (parentDebugManager)
      commandJob->SetParentDebugManager(parentDebugManager);

   JobStatus* unusedStatus = commandJob->Run();
   delete unusedStatus;

   bool returnValue = false;
   if (commandJob->IsRunOk())
   {
       TarCommandParser parser(commandJob->GetCommand());
       bool ok = parser.ParseBuffer(commandJob->GetCommandOutput());
       if (ok)
       {
           FileBackupReport* report = new FileBackupReport();
           parser.GetReport(*report);

           status->SetCode(JobStatus::Ok);
           results.push_back(pair<JobStatus*, FileBackupReport*>(status, report));
       }
       else
       {
           status->SetCode(JobStatus::OkWithWarnings);
           status->SetDescription(reportCreationError);
           results.push_back(pair<JobStatus*, FileBackupReport*>(status, NULL));
       }
       returnValue = true;
   }
   else
   {
       status->SetCode(JobStatus::Error);
       status->SetDescription(tarCommandError);
       parentDebugManager->AddDataLine<string>("tar output", commandJob->GetCommandOutput());
       parentDebugManager->AddDataLine<int>("tar code", commandJob->GetCommandReturnCode());
       results.push_back(pair<JobStatus*, FileBackupReport*>(status, NULL));
       returnValue = false;
   }

   delete commandJob;
   return returnValue;
}

bool TarTools::CreateIncrementalArchive(const string& commandLineParameters,
                                        const string& currentArchive,
                                        const string& referenceArchive,
                                        AbstractBackupJob::ResultCollection& results)
{
   JobStatus* status = new JobStatus();
   AbstractBackupJob::ResultCollection unusedResult;
   bool ok = CreateArchive(commandLineParameters, unusedResult);

   FileBackupReport* finalReport = NULL;
   if (ok)
   {
      finalReport = CreateReportFromArchives(referenceArchive, currentArchive);
      status->SetCode(JobStatus::Ok);
   }
   else
   {
      status->SetCode(JobStatus::Error);
      status->SetDescription("Incremental Archive not created");
   }

   results.push_back(pair<JobStatus*, FileBackupReport*>(status, finalReport));
   return ok;
}

bool TarTools::ExtractArchive(const string& archiveName, const string& destination)
{
   string parameters = "xvf " + archiveName + " -C " + destination;
   parameters += " --strip-components=1";

   ConsoleJob* job = new ConsoleJob("tar", parameters);
   if (parentDebugManager)
      job->SetParentDebugManager(parentDebugManager);

   JobStatus* unusedStatus = job->Run();
   delete unusedStatus;

   return job->IsRunOk();
}

bool TarTools::ExtractIncrementalArchive(const string& baseArchiveName,
                                         const int archiveIndex,
                                         const string& destination)
{
   bool result = ExtractArchive(baseArchiveName, destination);
   if (result)
   {
      for (int i=0; i<=archiveIndex; ++i)
      {
         stringstream archiveName;
         archiveName << baseArchiveName << "." << i;
         result = ExtractArchive(archiveName.str(), destination);
         if (result == false)
            break;
      }
   }
   return result;
}

AbstractConsoleJob *TarTools::CreateBackupConsoleJob(const string &parameters)
{
    return new ConsoleJob("tar", parameters);
}

FileBackupReport* TarTools::CreateReportFromArchives(const string& referenceArchive,
                                                    const string& currentArchive)
{
   vector<string> previousFileList;
   GetArchiveFileList(referenceArchive, previousFileList);

   vector<string> currentFileList;
   GetArchiveFileList(currentArchive, currentFileList);

   return CreateReportFromFileLists(previousFileList, currentFileList);
}

void TarTools::GetArchiveFileList(const string& archive, vector<string>& fileList)
{
   const string params = string("tf ") + archive;
   ConsoleJob* tarJob = new ConsoleJob("tar", params);

   tarJob->RunWithoutStatus();
   if (tarJob->IsRunOk())
   {
      vector<string> rawFileList;
      Tools::TokenizeString(tarJob->GetCommandOutput(), '\n', rawFileList);
      RemovePathHeaders(rawFileList);
      RemoveCurrentDirTag(rawFileList);
      copy(rawFileList.begin(), rawFileList.end(), back_inserter(fileList));
   }

   delete tarJob;
}

void TarTools::RemovePathHeaders(vector<string>& fileList)
{
   if (fileList.size() < 2)
      return;

   vector<string>::iterator previousValue = fileList.begin();
   vector<string>::iterator it = fileList.begin()+1;
   while (it != fileList.end())
   {
      if (it->find(previousValue->c_str()) != string::npos)
      {
         previousValue = fileList.erase(previousValue);
         it = previousValue+1;
      }
      else
      {
         previousValue = it;
         ++it;
      }
   }
}

void TarTools::RemoveCurrentDirTag(vector<string>& fileList)
{
   const string currentDirTag = "./";
   vector<string>::iterator it=fileList.begin();
   for (; it!=fileList.end(); ++it)
   {
      if (it->find(currentDirTag) == 0)
         *it = it->substr(currentDirTag.length());
   }
}

FileBackupReport* TarTools::CreateReportFromFileLists(
      const vector<string>& baseFileList, const vector<string>& newFileList)
{
   vector<string> addedFiles, modifiedFiles, removedFiles;

   set_difference(newFileList.begin(), newFileList.end(), baseFileList.begin(), baseFileList.end(),
                  back_inserter(addedFiles));
   set_intersection(newFileList.begin(), newFileList.end(), baseFileList.begin(), baseFileList.end(),
                  back_inserter(modifiedFiles));
   //set_difference(baseFileList.begin(), baseFileList.end(), newFileList.begin(), newFileList.end(),
   //               back_inserter(removedFiles));

   FileBackupReport* report = new FileBackupReport();
   report->AddAsAdded(addedFiles);
   report->AddAsModified(modifiedFiles);
   report->AddAsRemoved(removedFiles);
   return report;
}
