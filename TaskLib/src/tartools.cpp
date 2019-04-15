#include "tartools.h"

#include <algorithm>

#include "consolejob.h"
#include "jobdebuginformationmanager.h"
#include "sshconsolejob.h"
#include "tarcommandparser.h"
#include "stringtools.h"

using namespace std;

static const wstring reportCreationError = L"Failed creating report";
static const wstring tarCommandError = L"tar command failed";

TarTools::TarTools(JobExecutionTarget* _target,
                   JobDebugInformationManager* _parentDebugManager)
   : target(_target),
     parentDebugManager(_parentDebugManager)
{
}

bool TarTools::CreateArchive(const wstring& commandLineParameters,
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
       parentDebugManager->AddDataLine<wstring>(L"tar output", commandJob->GetCommandOutput());
       parentDebugManager->AddDataLine<int>(L"tar code", commandJob->GetCommandReturnCode());
       results.push_back(pair<JobStatus*, FileBackupReport*>(status, NULL));
       returnValue = false;
   }

   delete commandJob;
   return returnValue;
}

bool TarTools::CreateIncrementalArchive(const wstring& commandLineParameters,
                                        const wstring& currentArchive,
                                        const wstring& referenceArchive,
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
      status->SetDescription(L"Incremental Archive not created");
   }

   results.push_back(pair<JobStatus*, FileBackupReport*>(status, finalReport));
   return ok;
}

bool TarTools::ExtractArchive(const wstring& archiveName, const wstring& destination)
{
   wstring parameters = L"xvf " + archiveName + L" -C " + destination;
   parameters += L" --strip-components=1";

   ConsoleJob* job = new ConsoleJob(L"tar", parameters);
   if (parentDebugManager)
      job->SetParentDebugManager(parentDebugManager);

   JobStatus* unusedStatus = job->Run();
   delete unusedStatus;

   return job->IsRunOk();
}

bool TarTools::ExtractIncrementalArchive(const wstring& baseArchiveName,
                                         const int archiveIndex,
                                         const wstring& destination)
{
   bool result = ExtractArchive(baseArchiveName, destination);
   if (result)
   {
      for (int i=0; i<=archiveIndex; ++i)
      {
         wstringstream archiveName;
         archiveName << baseArchiveName << "." << i;
         result = ExtractArchive(archiveName.str(), destination);
         if (result == false)
            break;
      }
   }
   return result;
}

AbstractConsoleJob *TarTools::CreateBackupConsoleJob(const wstring &parameters)
{
    return new ConsoleJob(L"tar", parameters);
}

FileBackupReport* TarTools::CreateReportFromArchives(const wstring& referenceArchive,
                                                    const wstring& currentArchive)
{
   vector<wstring> previousFileList;
   GetArchiveFileList(referenceArchive, previousFileList);

   vector<wstring> currentFileList;
   GetArchiveFileList(currentArchive, currentFileList);

   return CreateReportFromFileLists(previousFileList, currentFileList);
}

void TarTools::GetArchiveFileList(const wstring& archive, vector<wstring>& fileList)
{
   const wstring params = wstring(L"tf ") + archive;
   ConsoleJob* tarJob = new ConsoleJob(L"tar", params);

   tarJob->RunWithoutStatus();
   if (tarJob->IsRunOk())
   {
      vector<wstring> rawFileList;
      StringTools::Tokenize(tarJob->GetCommandOutput(), L'\n', rawFileList);
      RemovePathHeaders(rawFileList);
      RemoveCurrentDirTag(rawFileList);
      copy(rawFileList.begin(), rawFileList.end(), back_inserter(fileList));
   }

   delete tarJob;
}

void TarTools::RemovePathHeaders(vector<wstring>& fileList)
{
   if (fileList.size() < 2)
      return;

   vector<wstring>::iterator previousValue = fileList.begin();
   vector<wstring>::iterator it = fileList.begin()+1;
   while (it != fileList.end())
   {
      if (it->find(previousValue->c_str()) != wstring::npos)
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

void TarTools::RemoveCurrentDirTag(vector<wstring>& fileList)
{
   const wstring currentDirTag = L"./";
   vector<wstring>::iterator it=fileList.begin();
   for (; it!=fileList.end(); ++it)
   {
      if (it->find(currentDirTag) == 0)
         *it = it->substr(currentDirTag.length());
   }
}

FileBackupReport* TarTools::CreateReportFromFileLists(
      const vector<wstring>& baseFileList, const vector<wstring>& newFileList)
{
   vector<wstring> addedFiles, modifiedFiles, removedFiles;

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
