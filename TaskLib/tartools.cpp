#include "tartools.h"

#include "consolejob.h"
#include "jobdebuginformationmanager.h"
#include "sshconsolejob.h"
#include "tarcommandparser.h"

using namespace std;

static const string reportCreationError = "Failed creating report";
static const string tarCommandError = "tar command failed";

TarTools::TarTools(JobDebugInformationManager* _parentDebugManager,
                   JobExecutionTarget* _target)
   : parentDebugManager(_parentDebugManager),
     target(_target)
{
}

bool TarTools::CreateArchive(const string& commandLineParameters,
                             AbstractBackupJob::ResultCollection& results)
{
   JobStatus* status = new JobStatus();

   AbstractConsoleJob* commandJob = CreateBackupConsoleJob(commandLineParameters);
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
