#include "linuxshutdownjob.h"

#include <unistd.h>

#include "jobdebuginformationmanager.h"
#include "tools.h"

using namespace std;

static const int DEFAULT_TIMEOUT = 120;

LinuxShutdownJob::LinuxShutdownJob()
    : AbstractJob(),
      computer(""), jobTimeoutInSeconds(DEFAULT_TIMEOUT)
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
    bool ok = AbstractJob::InitializeFromClient(client);
    if (ok)
    {
        computer = client->GetProperty("ip");
        bool jobInitialized = shutdownJob->InitializeFromClient(client);
        return (jobInitialized && computer != "");
    }
    else
        return false;
}

bool LinuxShutdownJob::IsInitialized()
{
    return shutdownJob->IsInitialized() && computer != "";
}

JobStatus *LinuxShutdownJob::Run()
{
    JobStatus* status = shutdownJob->Run();

    debugManager->AddDataLine<int>("Return code", shutdownJob->GetCommandReturnCode());
    debugManager->AddDataLine<string>("Output", shutdownJob->GetCommandOutput());
    if (status->GetCode() != JobStatus::ERROR)
    {
        int secondsToShutdown = WaitForComputerToGoDown();
        if (secondsToShutdown > jobTimeoutInSeconds)
        {
            status->SetCode(JobStatus::ERROR);
            status->SetDescription("Machine still running");
        }

        debugManager->AddDataLine<int>("Seconds counter", secondsToShutdown);
        debugManager->AddDataLine<int>("Timeout", jobTimeoutInSeconds);
    }

    return debugManager->UpdateStatus(status);
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
