#include "tartools.h"

#include "consolejob.h"
#include "jobdebuginformationmanager.h"
#include "sshconsolejob.h"
#include "tarcommandparser.h"

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

           status->SetCode(JobStatus::OK);
           results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, report));
       }
       else
       {
           status->SetCode(JobStatus::OK_WITH_WARNINGS);
           status->SetDescription(reportCreationError);
           results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
       }
       returnValue = true;
   }
   else
   {
       status->SetCode(JobStatus::ERROR);
       status->SetDescription(tarCommandError);
       parentDebugManager->AddDataLine<string>("tar output", commandJob->GetCommandOutput());
       parentDebugManager->AddDataLine<int>("tar code", commandJob->GetCommandReturnCode());
       results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, NULL));
       returnValue = false;
   }

   delete commandJob;
   return returnValue;
}

bool TarTools::CreateIncrementalArchive(const string& commandLineParameters,
                                        const string& referenceArchive,
                                        AbstractBackupJob::ResultCollection& results)
{
   JobStatus* status = new JobStatus();
   AbstractBackupJob::ResultCollection unusedResult;
   bool ok = CreateArchive(commandLineParameters, unusedResult);

   FileBackupReport* finalReport = NULL;
   if (ok)
   {
      // TODO : first list incremental archive, and diff on that.
      const string params = string("-tf ") + referenceArchive;
      ConsoleJob listJob("tar", params);
      listJob.RunWithoutStatus();
      if (listJob.IsRunOk())
      {

         TarCommandParser parser(listJob.GetCommand());
         bool ok = parser.ParseBuffer(listJob.GetCommandOutput());
         if (ok)
         {
            FileBackupReport referenceReport;
            parser.GetReport(referenceReport);
            finalReport = new FileBackupReport();
            //*finalReport = *latestResult.front().second - referenceReport;
         }
      }
   }
   else
   {
      status->SetCode(JobStatus::ERROR);
      status->SetDescription("Incremental Archive not created");
   }

   results.push_back(make_pair<JobStatus*, FileBackupReport*>(status, finalReport));
   return ok;
}

bool TarTools::ExtractArchive(const string& archiveName, const string& destination)
{
   string parameters = "xzvf " + archiveName + " -C " + destination;
   parameters += " --strip-components=1";

   ConsoleJob* job = new ConsoleJob("tar", parameters);
   if (parentDebugManager)
      job->SetParentDebugManager(parentDebugManager);

   JobStatus* unusedStatus = job->Run();
   delete unusedStatus;

   return job->IsRunOk();
}

AbstractConsoleJob *TarTools::CreateBackupConsoleJob(const string &parameters)
{
    ConsoleJob* job = new ConsoleJob("tar", parameters);
    if (target->isLocal)
        return job;
    else
    {
        SshConsoleJob* remoteJob = new SshConsoleJob(job);
        remoteJob->SetTarget(target->sshUser, target->sshHost);
        return remoteJob;
    }
}
