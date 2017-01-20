#include "wakejob.h"

#include <unistd.h>
#include "tools.h"

using namespace std;

static const int DEFAULT_TIMEOUT = 120;

WakeJob::WakeJob() : macAddress(""), broadcastIp(""), expectedIp("")
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

    return (macAddress != "" && broadcastIp != "" && expectedIp != "");
}

bool WakeJob::IsInitialized()
{
    return (macAddress != "" && broadcastIp != "" && expectedIp != "");
}

JobStatus *WakeJob::Run()
{
    const int maxRetries = 3;
    const string wakeCommand = string("wakelan -m ") + macAddress + " -b " + broadcastIp;

    for (int i=0; i<maxRetries; ++i)
    {
        std::string unusedBuffer;
        Tools::RunExternalCommandToBuffer(wakeCommand, unusedBuffer, true);

        int secondsToWake = WaitForComputerToGoUp();
        if (secondsToWake < DEFAULT_TIMEOUT)
            return new JobStatus(JobStatus::OK, "");
    }

    return new JobStatus(JobStatus::ERROR, "Machine still not awake");
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
