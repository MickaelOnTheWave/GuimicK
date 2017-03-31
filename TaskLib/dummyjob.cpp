#include "dummyjob.h"

DummyJob::DummyJob(const std::string &_name, const int expectedCode,
                   const std::string& expectedDescription)
    : name(_name), statusCode(expectedCode), statusDescription(expectedDescription),
      initializationValue(true)
{
}

std::string DummyJob::GetName()
{
    return name;
}

AbstractJob *DummyJob::Clone()
{
    return new DummyJob(name, statusCode, statusDescription);
}

bool DummyJob::InitializeFromClient(Client *)
{
    return initializationValue;
}

bool DummyJob::IsInitialized()
{
    return initializationValue;
}

JobStatus *DummyJob::Run()
{
	JobStatus* status = new JobStatus();

	status->SetCode(JobStatus::OK);
    status->SetDescription(statusDescription);
    return status;
}

void DummyJob::SetStatusDescription(const std::string &value)
{
    statusDescription = value;
}

void DummyJob::SetInitialization(const bool value)
{
    initializationValue = value;
}


