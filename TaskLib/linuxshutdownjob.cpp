#include "linuxshutdownjob.h"

#include <unistd.h>

#include "jobdebuginformationmanager.h"
#include "tools.h"

using namespace std;

static const int DEFAULT_TIMEOUT = 120;

LinuxShutdownJob::LinuxShutdownJob()
    : computer(""), jobTimeoutInSeconds(DEFAULT_TIMEOUT),
      outputDebugInformation(false)
{
    shutdownJob = new SshConsoleJob("", "shutdown -h now");
}

LinuxShutdownJob::~LinuxShutdownJob()
{
    delete shutdownJob;
}

std::string LinuxShutdownJob::GetName()
{
    return "Shutdown";
}

AbstractJob *LinuxShutdownJob::Clone()
{
    LinuxShutdownJob* clone = new LinuxShutdownJob();
    clone->computer = computer;
    clone->jobTimeoutInSeconds = jobTimeoutInSeconds;
    return clone;
}

bool LinuxShutdownJob::InitializeFromClient(Client *client)
{
    computer = client->GetProperty("ip");
    bool jobInitialized = shutdownJob->InitializeFromClient(client);
    return (jobInitialized && computer != "");
}

bool LinuxShutdownJob::IsInitialized()
{
    return shutdownJob->IsInitialized() && computer != "";
}

void LinuxShutdownJob::SetOutputDebugInformation(const bool value)
{
    outputDebugInformation = value;

    // TODO : reactivate this when [044] is implemented
    //shutdownJob->SetOutputDebugInformation(outputDebugInformation);
}

JobStatus *LinuxShutdownJob::Run()
{
    JobDebugInformationManager debugInfo(outputDebugInformation, GetName());
    JobStatus* status = shutdownJob->Run();

    // TODO : reactivate this when [044] is implemented
    //debugInfo.AddIntDataLine("Return code", shutdownJob->GetCommandReturnCode());
    //debugInfo.AddStringDataLine("Output", shutdownJob->GetCommandOutput());


    if (status->GetCode() == JobStatus::ERROR)
        return debugInfo.UpdateStatus(status);

    int secondsToShutdown = WaitForComputerToGoDown();
    if (secondsToShutdown > jobTimeoutInSeconds)
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription("Machine still running");
    }

    debugInfo.AddDataLine<int>("Seconds counter", secondsToShutdown);
    debugInfo.AddDataLine<int>("Timeout", jobTimeoutInSeconds);
    return debugInfo.UpdateStatus(status);
}

int LinuxShutdownJob::WaitForComputerToGoDown() const
{
    int secondsCounter = 0;
    while (Tools::IsComputerAlive(computer) && secondsCounter < jobTimeoutInSeconds)
    {
        sleep(1);
        ++secondsCounter;
    }

    return secondsCounter;
}
