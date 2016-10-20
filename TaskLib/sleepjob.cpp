#include "sleepjob.h"

#ifdef __WIN32__
#include <windows.h>
#else
#include <unistd.h>
#endif

SleepJob::SleepJob(int _sleepTime)
	: sleepTime(_sleepTime)
{
}

JobStatus *SleepJob::Run()
{
#ifdef __WIN32__
	Sleep(sleepTime*1000);
#else
	usleep(static_cast<useconds_t>(sleepTime)*1000*1000);
#endif

	return new JobStatus(JobStatus::OK);
}
