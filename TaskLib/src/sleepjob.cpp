#include "sleepjob.h"
#include "tools.h"

SleepJob::SleepJob(int _sleepTime)
   : AbstractJob("SleepJob"), sleepTime(_sleepTime)
{
}

JobStatus *SleepJob::Run()
{
   Tools::Wait(sleepTime);
	return new JobStatus(JobStatus::Ok);
}
