#include "wakejob.h"

#include <unistd.h>

#include "consolejob.h"
#include "jobdebuginformationmanager.h"
#include "tools.h"

using namespace std;

static const int DEFAULT_TIMEOUT = 120;

WakeJob::WakeJob()
    : macAddress(""), broadcastIp(""), expectedIp(""),
      wakelanPath(""), outputDebugInformation(false)
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
    WakeJob* cloned = new WakeJob();
    cloned->macAddress = macAddress;
    cloned->broadcastIp = broadcastIp;
    cloned->expectedIp = expectedIp;
    cloned->wakelanPath = wakelanPath;
    cloned->outputDebugInformation = outputDebugInformation;
    return cloned;
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

void WakeJob::SetOutputDebugInformation(const bool value)
{
    outputDebugInformation = value;
}

JobStatus *WakeJob::Run()
{
    const string wakeCommand = wakelanPath + " -m " + macAddress + " -b " + broadcastIp;
    JobDebugInformationManager debugInfo(GetName(), outputDebugInformation);
    debugInfo.AddStringDataLine("Wake command", wakeCommand);

    const int maxRetries = 3;
    for (int i=0; i<maxRetries; ++i)
    {
        std::string wakeCommandOutput;
        Tools::RunExternalCommandToBuffer(wakeCommand, wakeCommandOutput, true);
        debugInfo.AddStringDataLine("Output", wakeCommandOutput);

        int secondsToWake = WaitForComputerToGoUp();
        if (secondsToWake < DEFAULT_TIMEOUT)
        {
            debugInfo.AddIntDataLine("retries", i);
            debugInfo.AddIntDataLine("maxRetries", maxRetries);
            debugInfo.AddIntDataLine("seconds counter", secondsToWake);
            debugInfo.AddIntDataLine("timeout", DEFAULT_TIMEOUT);
            return debugInfo.CreateStatus(JobStatus::OK, "");
        }
    }

    debugInfo.AddIntDataLine("maxRetries", maxRetries);
    debugInfo.AddIntDataLine("timeout", DEFAULT_TIMEOUT);
    return debugInfo.CreateStatus(JobStatus::ERROR, "Machine still not awake");
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
