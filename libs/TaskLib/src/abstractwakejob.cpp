#include "abstractwakejob.h"

#include "consolejob.h"
#include "tools.h"

using namespace std;

static const int defaultTimeout = 120;
static const int defaultRetries = 3;

AbstractWakeJob::AbstractWakeJob()
   : AbstractJob(L"Wake"),
    macAddress(L""), broadcastIp(L""), expectedIp(L""),
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

    if (macAddress == L"")
        macAddress = client->GetProperty(L"mac");
    if (broadcastIp == L"")
        broadcastIp = client->GetProperty(L"broadcast");
    if (expectedIp == L"")
        expectedIp = client->GetProperty(L"ip");

    return IsInitialized();
}

bool AbstractWakeJob::IsInitialized()
{
    return HasMandatoryParameters();
}

JobStatus* AbstractWakeJob::Run()
{
   debugManager->AddDataLine<int>(L"maxRetries", maxRetries);
   debugManager->AddDataLine<int>(L"timeout", timeout);

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
         debugManager->AddDataLine<int>(L"Retry count", i);
         debugManager->AddDataLine<int>(L"seconds counter", secondsToWake);
         return debugManager->CreateStatus(JobStatus::Ok, L"");
     }
   }

   return debugManager->CreateStatus(JobStatus::Error, L"Machine still not awake");
}

int AbstractWakeJob::GetTimeout() const
{
    return timeout;
}

void AbstractWakeJob::SetTimeout(const int value)
{
   debugManager->AddDataLine<int>(L"Setting timeout", value);
   timeout = value;
}

int AbstractWakeJob::GetMaxRetries() const
{
    return maxRetries;
}

void AbstractWakeJob::SetMaxRetries(const int value)
{
   debugManager->AddDataLine<int>(L"Setting max retries", value);
   maxRetries = value;
}

bool AbstractWakeJob::HasMandatoryParameters() const
{
    return (macAddress != L"" && broadcastIp != L"" && expectedIp != L"");
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
