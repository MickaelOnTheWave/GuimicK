#include "wakejob.h"

#include <unistd.h>

#include "consolejob.h"
#include "tools.h"

using namespace std;

static const int DEFAULT_TIMEOUT = 120;

WakeJob::WakeJob() : AbstractJob(),
    macAddress(""), broadcastIp(""), expectedIp(""),
    wakelanPath("")
{
}

WakeJob::WakeJob(const WakeJob &other)
    : AbstractJob(other),
      macAddress(other.macAddress),
      broadcastIp(other.broadcastIp),
      expectedIp(other.expectedIp),
      wakelanPath(other.wakelanPath)
{
}

WakeJob::~WakeJob()
{
}

std::string WakeJob::GetName()
{
    return "Wake";
}

AbstractJob *WakeJob::Clone()
{
    return new WakeJob(*this);
}

bool WakeJob::InitializeFromClient(Client *client)
{
    if (macAddress == "")
        macAddress = client->GetProperty("mac");
    if (broadcastIp == "")
        broadcastIp = client->GetProperty("broadcast");
    if (expectedIp == "")
        expectedIp = client->GetProperty("ip");

    wakelanPath = Tools::GetCommandPath("wakelan", ConsoleJob::appSearchPaths);
    return IsInitialized();
}

bool WakeJob::IsInitialized()
{
    return wakelanPath != "" && HasMandatoryParameters();
}

JobStatus *WakeJob::Run()
{
    const string wakeCommand = wakelanPath + " -m " + macAddress + " -b " + broadcastIp;
    debugManager->AddDataLine<string>("Wake command", wakeCommand);

    const int maxRetries = 3;
    for (int i=0; i<maxRetries; ++i)
    {
        std::string wakeCommandOutput;
        Tools::RunExternalCommandToBuffer(wakeCommand, wakeCommandOutput, true);
        debugManager->AddDataLine<string>("Output", wakeCommandOutput);

        int secondsToWake = WaitForComputerToGoUp();
        if (secondsToWake < DEFAULT_TIMEOUT)
        {
            debugManager->AddDataLine<int>("retries", i);
            debugManager->AddDataLine<int>("maxRetries", maxRetries);
            debugManager->AddDataLine<int>("seconds counter", secondsToWake);
            debugManager->AddDataLine<int>("timeout", DEFAULT_TIMEOUT);
            return debugManager->CreateStatus(JobStatus::OK, "");
        }
    }

    debugManager->AddDataLine<int>("maxRetries", maxRetries);
    debugManager->AddDataLine<int>("timeout", DEFAULT_TIMEOUT);
    return debugManager->CreateStatus(JobStatus::ERROR, "Machine still not awake");
}

bool WakeJob::HasMandatoryParameters() const
{
    return (macAddress != "" && broadcastIp != "" && expectedIp != "");
}

int WakeJob::WaitForComputerToGoUp() const
{
    int secondsElapsed = 0;
    while (!Tools::IsComputerAlive(expectedIp) && secondsElapsed < DEFAULT_TIMEOUT)
    {
        sleep(1);
        ++secondsElapsed;
    }
    return secondsElapsed;
}
