#include "dummyjob.h"

DummyJob::DummyJob(const std::string &resultAfterRun)
	: resultString(resultAfterRun)
{
}

JobStatus *DummyJob::Run()
{
	JobStatus* status = new JobStatus();

	status->SetCode(JobStatus::OK);
	status->SetDescription(resultString);
	return status;
}


