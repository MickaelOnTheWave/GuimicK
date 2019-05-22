#include "commandbasedjob.h"

using namespace std;

CommandBasedJob::CommandBasedJob(const wstring& jobName)
    : AbstractJob(jobName)
{
}

CommandBasedJob::CommandBasedJob(const CommandBasedJob& other)
    : AbstractJob(other)
{
}

JobStatus *CommandBasedJob::Run()
{
    AbstractConsoleJob* job = CreateJobInstance();

    debugManager->AddDataLine<wstring>(L"Command", job->GetCommand());
    debugManager->AddDataLine<wstring>(L"Parameters", job->GetCommandParameters());

    JobStatus* status = job->Run();
    if (status->GetCode() != JobStatus::NotExecuted)
        AddJobOutputToStatus(status, job);

    delete job;
    return debugManager->UpdateStatus(status);
}

void CommandBasedJob::AddJobOutputToStatus(JobStatus* status, AbstractConsoleJob* job)
{
    AbstractOutputParser* parser = CreateOutputParser(status->GetCode());
    if (parser)
        AddParsedOutputToStatus(status, parser, job);
    else
        AddRawOutputToStatus(status, job);
    delete parser;
}

void CommandBasedJob::AddParsedOutputToStatus(JobStatus* status,
                                                    AbstractOutputParser* parser,
                                                    AbstractConsoleJob* job)
{
    wstring reportContent = L"";
    bool ok = parser->ParseBuffer(job->GetCommandOutput());
    if (ok)
    {
        status->SetDescription(parser->GetMiniDescription());
        reportContent = parser->GetFullDescription();
    }
    else
    {
        if (status->GetCode() == JobStatus::Ok)
            status->SetCode(JobStatus::OkWithWarnings);

        status->SetDescription(L"Failed to parse output");
        reportContent = job->GetCommandOutput();
    }

    if (reportContent != L"")
       status->AddFileBuffer(GetAttachmentName(), reportContent);
}

void CommandBasedJob::AddRawOutputToStatus(JobStatus* status,
                                                 AbstractConsoleJob* job)
{
   const wstring rawOutput = job->GetCommandOutput();
   if (rawOutput != L"")
      status->AddFileBuffer(GetAttachmentName(), rawOutput);
}
