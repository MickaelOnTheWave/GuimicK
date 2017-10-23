#include "wakejob.h"

#include <unistd.h>

#include "consolejob.h"
#include "tools.h"

using namespace std;

static const int defaultTimeout = 120;
static const int defaultRetries = 3;

WakeJob::WakeJob()
   : AbstractJob("Wake"),
    macAddress(""), broadcastIp(""), expectedIp(""),
    timeout(defaultTimeout), maxRetries(defaultRetries)
{
}

WakeJob::WakeJob(const WakeJob &other)
    : AbstractJob(other),
      macAddress(other.macAddress),
      broadcastIp(other.broadcastIp),
      expectedIp(other.expectedIp),
      timeout(other.timeout),
      maxRetries(other.maxRetries)
{
}

WakeJob::~WakeJob()
{
}

AbstractJob *WakeJob::Clone()
{
    return new WakeJob(*this);
}

bool WakeJob::InitializeFromClient(Client *client)
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

bool WakeJob::IsInitialized()
{
    return HasMandatoryParameters();
}

JobStatus *WakeJob::Run()
{
    const string parameters = string("-m ") + macAddress + " -b " + broadcastIp;
    ConsoleJob wakeCommand("wakelan", parameters);
    wakeCommand.SetParentDebugManager(debugManager);

    if (!wakeCommand.IsCommandAvailable())
        return debugManager->CreateStatus(JobStatus::ERROR, "wakelan not installed");

    debugManager->AddDataLine<int>("maxRetries", maxRetries);
    debugManager->AddDataLine<int>("timeout", timeout);

    for (int i=0; i<maxRetries; ++i)
    {
        JobStatus* status = wakeCommand.Run();
        if (status->GetCode() != JobStatus::OK)
            return status;
        else
            delete status;

        debugManager->AddDataLine<string>("Wake Output", wakeCommand.GetCommandOutput());

        int secondsToWake = WaitForComputerToGoUp();
        if (secondsToWake < timeout)
        {
            debugManager->AddDataLine<int>("Retry count", i);
            debugManager->AddDataLine<int>("seconds counter", secondsToWake);
            return debugManager->CreateStatus(JobStatus::OK, "");
        }
    }

    return debugManager->CreateStatus(JobStatus::ERROR, "Machine still not awake");
}

int WakeJob::GetTimeout() const
{
    return timeout;
}

void WakeJob::SetTimeout(const int value)
{
    timeout = value;
}

int WakeJob::GetMaxRetries() const
{
    return maxRetries;
}

void WakeJob::SetMaxRetries(const int value)
{
    maxRetries = value;
}

bool WakeJob::HasMandatoryParameters() const
{
    return (macAddress != "" && broadcastIp != "" && expectedIp != "");
}

int WakeJob::WaitForComputerToGoUp() const
{
    int secondsElapsed = 0;
    while (!Tools::IsComputerAlive(expectedIp) && secondsElapsed < timeout)
    {
        sleep(1);
        ++secondsElapsed;
    }
    return secondsElapsed;
}
