#ifndef ABSTRACTJOB_H
#define ABSTRACTJOB_H

#include <string>

#include "client.h"
#include "jobstatus.h"

namespace DebugOutput
{
    static const int NEVER = 0;
    static const int ON_ERROR = 1;
    static const int ALWAYS = 2;
}

class AbstractJob
{
public:
    AbstractJob();
    virtual ~AbstractJob();

	virtual std::string GetName() = 0;
    std::string GetAttachmentName();

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

    virtual void SetOutputDebugInformation(const int value);

};

#endif // ABSTRACTJOB_H
