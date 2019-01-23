#include "abstractwakejob.h"

#include "consolejob.h"
#include "tools.h"

using namespace std;

static const int defaultTimeout = 120;
static const int defaultRetries = 3;

AbstractWakeJob::AbstractWakeJob()
   : AbstractJob("Wake"),
    macAddress(""), broadcastIp(""), expectedIp(""),
    timeout(defaultTimeout), maxRetries(defaultRetries)
{
}

AbstractWakeJob::AbstractWakeJob(const AbstractWakeJob &other)
    : AbstractJob(other),
      macAddress(other.macAddress),
      broadcastIp(other.broadcastIp),
      expectedIp(other.expectedIp),
      timeout(other.timeout),
      maxRetries(other.maxRetries)
{
}

AbstractWakeJob::~AbstractWakeJob()
{
}

bool AbstractWakeJob::InitializeFromClient(Client *client)
{
    bool ok = AbstractJob::InitializeFromClient(client);
    if (!ok)
        return false;

    if (macAddress == "")
        macAddress = client->GetProperty("mac");
    if (broadcastIp == "")
        broadcastIp = client->GetProperty("broadcast");
    if (expectedIp == "")
        expectedIp = client->GetProperty("ip");

    return IsInitialized();
}

bool AbstractWakeJob::IsInitialized()
{
    return HasMandatoryParameters();
}

JobStatus* AbstractWakeJob::Run()
{
   debugManager->AddDataLine<int>("maxRetries", maxRetries);
   debugManager->AddDataLine<int>("timeout", timeout);

   JobStatus* setupStatus = SetupWaker();
   if (setupStatus->GetCode() == JobStatus::Error)
      return debugManager->UpdateStatus(setupStatus);

   for (int i=0; i<maxRetries; ++i)
   {
      JobStatus* wakeStatus = RunWaker();
      if (wakeStatus->GetCode() == JobStatus::Error)
         return debugManager->UpdateStatus(wakeStatus);

     int secondsToWake = WaitForComputerToGoUp();
     if (secondsToWake < timeout)
     {
         debugManager->AddDataLine<int>("Retry count", i);
         debugManager->AddDataLine<int>("seconds counter", secondsToWake);
         return debugManager->CreateStatus(JobStatus::Ok, "");
     }
   }

   return debugManager->CreateStatus(JobStatus::Error, "Machine still not awake");
}

int AbstractWakeJob::GetTimeout() const
{
    return timeout;
}

void AbstractWakeJob::SetTimeout(const int value)
{
   debugManager->AddDataLine<int>("Setting timeout", value);
   timeout = value;
}

int AbstractWakeJob::GetMaxRetries() const
{
    return maxRetries;
}

void AbstractWakeJob::SetMaxRetries(const int value)
{
   debugManager->AddDataLine<int>("Setting max retries", value);
   maxRetries = value;
}

bool AbstractWakeJob::HasMandatoryParameters() const
{
    return (macAddress != "" && broadcastIp != "" && expectedIp != "");
}

int AbstractWakeJob::WaitForComputerToGoUp() const
{
   int secondsElapsed = 0;
   while (!Tools::IsComputerAlive(expectedIp) && secondsElapsed < timeout)
   {
      Tools::Wait(1);
      ++secondsElapsed;
   }
   return secondsElapsed;
}
