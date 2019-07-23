#include "linuxshutdownjob.h"

#include "jobdebuginformationmanager.h"
#include "tools.h"

using namespace std;

static const int defaultTimeout = 120;

LinuxShutdownJob::LinuxShutdownJob()
    : AbstractJob(L"Shutdown"),
      computer(L""), jobTimeoutInSeconds(defaultTimeout)
{
    shutdownJob = new SshConsoleJob(L"Shutdown", L"sudo shutdown -h now");
}

LinuxShutdownJob::LinuxShutdownJob(const LinuxShutdownJob &other)
    : AbstractJob(other),
      computer(other.computer),
      jobTimeoutInSeconds(other.jobTimeoutInSeconds)
{
    shutdownJob = static_cast<SshConsoleJob*>(other.shutdownJob->Clone());
}

LinuxShutdownJob::~LinuxShutdownJob()
{
    delete shutdownJob;
}

AbstractJob *LinuxShutdownJob::Clone()
{
    return new LinuxShutdownJob(*this);
}

bool LinuxShutdownJob::InitializeFromClient(Client *client)
{
    bool ok = AbstractJob::InitializeFromClient(client);
    if (ok)
    {
        computer = client->GetProperty(L"ip");
        bool jobInitialized = shutdownJob->InitializeFromClient(client);
        return (jobInitialized && computer != L"");
    }
    else
        return false;
}

bool LinuxShutdownJob::IsInitialized()
{
    return shutdownJob->IsInitialized() && computer != L"";
}

JobStatus *LinuxShutdownJob::Run()
{
    JobStatus* status = shutdownJob->Run();

    debugManager->AddDataLine<int>(L"Return code", shutdownJob->GetCommandReturnCode());
    debugManager->AddDataLine<wstring>(L"Output", shutdownJob->GetCommandOutput());
    if (status->GetCode() != JobStatus::Error)
    {
        int secondsToShutdown = WaitForComputerToGoDown();
        if (secondsToShutdown > jobTimeoutInSeconds)
        {
            status->SetCode(JobStatus::Error);
            status->SetDescription(L"Machine still running");
        }

        debugManager->AddDataLine<int>(L"Seconds counter", secondsToShutdown);
        debugManager->AddDataLine<int>(L"Timeout", jobTimeoutInSeconds);
    }

    return debugManager->UpdateStatus(status);
}

int LinuxShutdownJob::GetTimeout() const
{
   return jobTimeoutInSeconds;
}

void LinuxShutdownJob::SetTimeout(const int value)
{
   jobTimeoutInSeconds = value;
}

int LinuxShutdownJob::WaitForComputerToGoDown() const
{
    int secondsCounter = 0;
    while (Tools::IsComputerAlive(computer) && secondsCounter < jobTimeoutInSeconds)
    {
        Tools::Wait(1);
        ++secondsCounter;
    }

    return secondsCounter;
}
