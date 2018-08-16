#ifndef PROFILEDJOB_H
#define PROFILEDJOB_H

#include <ctime>

#include "abstractjob.h"

/**
	This class uses the decorator pattern to implement its features.
	It is designed to be used with another job class to provide its
	profiling feature on top of it.
*/
class ProfiledJob : public AbstractJob
{
public:    
	ProfiledJob(AbstractJob* _baseJob);
	virtual ~ProfiledJob();

   virtual std::string GetName() const;

   virtual AbstractJob* Clone();

	virtual bool InitializeFromClient(Client* job);

    virtual bool IsInitialized(void);

	/**
		 Runs the job and profiles its time required to be ran.
	*/
	virtual JobStatus* Run();

	std::time_t GetElapstedTime();

protected:
	AbstractJob* baseJob;
	std::time_t elapsedTime;
};

#endif // PROFILEDJOB_H
