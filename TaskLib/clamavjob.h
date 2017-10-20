#ifndef CLAMAVJOB_H
#define CLAMAVJOB_H

#include "abstractjob.h"
#include "userconsolejob.h"

class ClamAvJob : public AbstractJob
{
public:
   ClamAvJob();

   ~ClamAvJob();

   virtual AbstractJob* Clone();

	virtual bool InitializeFromClient(Client *client);

   virtual bool IsInitialized(void);

	virtual JobStatus* Run();

protected:
   UserConsoleJob* virusDefinitionUpdateJob;
   UserConsoleJob* virusFullScanJob;
	std::string scanDir;
	bool blockOnFailingUpdate;
};

#endif // CLAMAVJOB_H
