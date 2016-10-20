#ifndef SLEEPJOB_H
#define SLEEPJOB_H

#include "abstractjob.h"

class SleepJob : public AbstractJob
{
public:
	SleepJob(int _sleepTime);

	virtual std::string GetName() { return "SleepJob"; }

	virtual JobStatus* Run();

protected:
	int sleepTime;
};

#endif // SLEEPJOB_H
