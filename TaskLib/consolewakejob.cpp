#include "consolewakejob.h"

#include <fstream>
#include <sstream>
#include <tools.h>
#include <filetools.h>

using namespace std;

const string WAKE_SCRIPT = "wakeComputer.sh";

ConsoleWakeJob::ConsoleWakeJob()
    : ConsoleJob("Wake", "sh", WAKE_SCRIPT, 1),
	  macAddress(""), broadcastIp(""), expectedIp("")
{
}

void ConsoleWakeJob::SetTarget(const string &_macAddress, const string &_broadcastIp, const string &_expectedIp)
{
    macAddress = _macAddress;
    broadcastIp = _broadcastIp;
    expectedIp = _expectedIp;
}

bool ConsoleWakeJob::InitializeFromClient(Client *client)
{
    if (macAddress == "")
        macAddress = client->GetProperty("mac");
    if (broadcastIp == "")
        broadcastIp = client->GetProperty("broadcast");
    if (expectedIp == "")
        expectedIp = client->GetProperty("ip");

    return true;
}

bool ConsoleWakeJob::IsInitialized()
{
    if (!ConsoleJob::IsInitialized())
        return false;
    if (!FileTools::FileExists(WAKE_SCRIPT))
        return false;

    return (macAddress != "" && broadcastIp != "" && expectedIp != "");
}

JobStatus *ConsoleWakeJob::Run()
{
	JobStatus* status = ConsoleJob::Run();
	if (status->GetCode() != JobStatus::OK)
	{
        string description(standardOutput);
        if (description == "")
        {
            description = "No output. Received code : ";
            stringstream stream;
            stream << receivedReturnCode;
            description += stream.str();
        }
        status->SetDescription(description);
	}
	else
        remove(outputFileName.c_str());

    return status;
}

string ConsoleWakeJob::CreateFullCommand()
{
    string command = ConsoleJob::CreateFullCommand();
    command += " -m " + macAddress;
    command += " -b " + broadcastIp;
    command += " -i " + expectedIp;
    return command;
}
