#ifndef DUMMYJOB_H
#define DUMMYJOB_H

#include <string>
#include "abstractjob.h"

class DummyJob : public AbstractJob
{
public:
	DummyJob(const std::string& resultAfterRun);

	virtual std::string GetName() { return "DummyJob"; }

	virtual JobStatus* Run();

protected:
	std::string resultString;
};

#endif // DUMMYJOB_H
