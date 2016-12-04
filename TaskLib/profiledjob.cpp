#include "profiledjob.h"

using namespace std;

ProfiledJob::ProfiledJob(AbstractJob *_baseJob)
    : baseJob(_baseJob), elapsedTime(0)
{
}

ProfiledJob::~ProfiledJob()
{
	delete baseJob;
}

string ProfiledJob::GetName()
{
    return baseJob->GetName();
}

AbstractJob *ProfiledJob::Clone()
{
    ProfiledJob* clone = new ProfiledJob(baseJob->Clone());
    clone->elapsedTime = elapsedTime;
    return clone;
}

bool ProfiledJob::InitializeFromClient(Client* job)
{
    return baseJob->InitializeFromClient(job);
}

bool ProfiledJob::IsInitialized()
{
    return baseJob->IsInitialized();
}

JobStatus* ProfiledJob::Run()
{
	time_t startTime = time(NULL);
	JobStatus* status = baseJob->Run();
	elapsedTime = time(NULL) - startTime;
	status->SetDuration(elapsedTime);
	return status;
}

time_t ProfiledJob::GetElapstedTime()
{
	 return elapsedTime;
}


