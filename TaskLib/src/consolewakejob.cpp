#include "consolewakejob.h"

using namespace std;

ConsoleWakeJob::ConsoleWakeJob()
   : AbstractWakeJob(), wakeCommand(L"wakelan", L"")
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
   const wstring parameters = wstring(L"-m ") + macAddress + L" -b " + broadcastIp;
   wakeCommand.SetCommandParameters(parameters);
   wakeCommand.SetParentDebugManager(debugManager);

   if (wakeCommand.IsCommandAvailable())
      return new JobStatus(JobStatus::Ok);
   else
      return new JobStatus(JobStatus::Error, L"wakelan not installed");
}

JobStatus* ConsoleWakeJob::RunWaker()
{
   JobStatus* status = wakeCommand.Run();
   if (status->GetCode() != JobStatus::Ok)
       return status;
   else
       delete status;

   debugManager->AddDataLine<wstring>(L"Wake Output", wakeCommand.GetCommandOutput());
   return new JobStatus(JobStatus::Ok);
}
