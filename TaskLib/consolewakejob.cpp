#include "consolewakejob.h"

using namespace std;

ConsoleWakeJob::ConsoleWakeJob()
   : AbstractWakeJob(), wakeCommand("wakelan", "")
{
}

ConsoleWakeJob::ConsoleWakeJob(const ConsoleWakeJob &other)
    : AbstractWakeJob(other),
      wakeCommand(other.wakeCommand)
{
}

ConsoleWakeJob::~ConsoleWakeJob()
{
}

AbstractJob *ConsoleWakeJob::Clone()
{
    return new ConsoleWakeJob(*this);
}

JobStatus* ConsoleWakeJob::SetupWaker()
{
   const string parameters = string("-m ") + macAddress + " -b " + broadcastIp;
   wakeCommand.SetCommandParameters(parameters);
   wakeCommand.SetParentDebugManager(debugManager);

   if (wakeCommand.IsCommandAvailable())
      return new JobStatus(JobStatus::OK);
   else
      return new JobStatus(JobStatus::ERROR, "wakelan not installed");
}

JobStatus* ConsoleWakeJob::RunWaker()
{
   JobStatus* status = wakeCommand.Run();
   if (status->GetCode() != JobStatus::OK)
       return status;
   else
       delete status;

   debugManager->AddDataLine<string>("Wake Output", wakeCommand.GetCommandOutput());
   return new JobStatus(JobStatus::OK);
}
