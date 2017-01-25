#include "wakejob.h"

#include <unistd.h>
#include <sstream>
#include "consolejob.h"
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
    const int maxRetries = 3;
    const string wakeCommand = wakelanPath + " -m " + macAddress + " -b " + broadcastIp;
    string debugBuffer = string("wakeCommand : <") + wakeCommand + ">\n";

    for (int i=0; i<maxRetries; ++i)
    {
        std::string wakeCommandOutput;
        Tools::RunExternalCommandToBuffer(wakeCommand, wakeCommandOutput, true);
        debugBuffer += string("output :\n") + wakeCommandOutput + "\n";

        int secondsToWake = WaitForComputerToGoUp();
        if (secondsToWake < DEFAULT_TIMEOUT)
        {
            debugBuffer += CreateValueInformationLine("retries", i);
            debugBuffer += CreateValueInformationLine("maxRetries", maxRetries);
            debugBuffer += CreateValueInformationLine("seconds counter", secondsToWake);
            debugBuffer += CreateValueInformationLine("timeout", DEFAULT_TIMEOUT);
            return CreateStatus(JobStatus::OK, "", debugBuffer);
        }
    }

    debugBuffer += CreateValueInformationLine("maxRetries", maxRetries);
    debugBuffer += CreateValueInformationLine("timeout", DEFAULT_TIMEOUT);
    return CreateStatus(JobStatus::ERROR, "Machine still not awake", debugBuffer);
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

JobStatus *WakeJob::CreateStatus(const int code,
                                 const string &description,
                                 const string &debugInformation) const
{
    JobStatus* status = new JobStatus(code, description);
    if (outputDebugInformation)
        status->AddFileBuffer("WakeInformation.txt", debugInformation);
    return status;
}

string WakeJob::CreateValueInformationLine(const string &label, const int value) const
{
    stringstream line;
    line << label << " : " << value << endl;
    return line.str();
}
