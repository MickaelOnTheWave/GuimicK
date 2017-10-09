#ifndef CLIENTWORKMANAGER_H
#define CLIENTWORKMANAGER_H

#include <list>

#include "abstractjob.h"
#include "client.h"
#include "workresultdata.h"

class ClientWorkManager
{
public:
    ClientWorkManager(Client* _client, const bool timedWorkList = false);
    ~ClientWorkManager();

    void AddJob(AbstractJob* newJob);	 

	 /**
		 Removes a job from loaded job list.
		 @param[in] jobName Job name.
		 @note This method will only work if there is only
		 one job with this name. In case of several jobs, none
         of them will be removed.
		*/
	 bool RemoveJob(const std::string& jobName);

	 /**
		 Removes all jobs except those with specified name
		 @param[in] jobName Job name.
		*/

	 bool RemoveAllButJobs(const std::string& jobName);

	 WorkResultData* RunWorkList();

     void GetJobList(std::vector<AbstractJob*> &_jobs);

private:
   void AddJobsFromClient(const bool timedWorkList);

   Client* client;
   std::list<AbstractJob*> jobList;
};

#endif // CLIENTWORKMANAGER_H
