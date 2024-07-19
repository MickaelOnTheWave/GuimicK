#include "dummyjob.h"

DummyJob::DummyJob(const std::wstring& _name, const int expectedCode,
                   const std::wstring& expectedDescription)
    : AbstractJob(_name),
      statusCode(expectedCode), statusDescription(expectedDescription),
      initializationValue(true)
{
}

DummyJob::DummyJob(const DummyJob& other)
   : AbstractJob(other),
     statusCode(other.statusCode), statusDescription(other.statusDescription),
     initializationValue(other.initializationValue)
{
}

AbstractJob *DummyJob::Clone()
{
    return new DummyJob(*this);
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

    status->SetCode(statusCode);
    status->SetDescription(statusDescription);
    return status;
}

void DummyJob::SetStatusDescription(const std::wstring &value)
{
    statusDescription = value;
}

void DummyJob::SetInitialization(const bool value)
{
    initializationValue = value;
}


