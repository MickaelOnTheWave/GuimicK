#include "linuxshutdownjob.h"

#include <unistd.h>
#include "tools.h"

static const int DEFAULT_TIMEOUT = 120;

LinuxShutdownJob::LinuxShutdownJob()
    : computer(""), jobTimeoutInSeconds(DEFAULT_TIMEOUT)
{
    shutdownJob = new SshConsoleJob("", "shutdown", "-h now");
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

JobStatus *LinuxShutdownJob::Run()
{
    JobStatus* status = shutdownJob->Run();
    if (status->GetCode() == JobStatus::ERROR)
        return status;

    int secondsToShutdown = WaitForComputerToGoDown();

    if (secondsToShutdown > jobTimeoutInSeconds)
    {
        status->SetCode(JobStatus::ERROR);
        status->SetDescription("Machine still running");
    }

    return status;
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
