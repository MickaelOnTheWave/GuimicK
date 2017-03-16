#ifndef ABSTRACTJOB_H
#define ABSTRACTJOB_H

#include <string>

#include "client.h"
#include "jobstatus.h"

class AbstractJob
{
public:
	virtual ~AbstractJob() {}

	virtual std::string GetName() = 0;

    virtual AbstractJob* Clone() = 0;

	/**
		Performs initial setup and gets needed data from client if
		needed.
		@return true on sucess, false on failure.
		@note failure is considered fatal, the job should not be ran if
		it wasn't sucessfully initialized.
	  */
    virtual bool InitializeFromClient(Client* client) = 0;

    virtual bool IsInitialized(void) = 0;

	/**
		@note It is caller's responsibility to delete the JobStatus object.
	  */
	virtual JobStatus* Run() = 0;
};

#endif // ABSTRACTJOB_H
