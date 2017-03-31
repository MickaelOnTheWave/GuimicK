#include "clientworkmanager.h"

#include <utility>

using namespace std;

ClientWorkManager::ClientWorkManager(Client *_client)
	: client(_client)
{
}

ClientWorkManager::~ClientWorkManager()
{
    delete client;

    list<AbstractJob*>::iterator it=jobList.begin();
    for (; it!=jobList.end(); ++it)
        delete (*it);
    jobList.clear();
}

void ClientWorkManager::AddJob(AbstractJob *newJob)
{
	newJob->InitializeFromClient(client);
	jobList.push_back(newJob);
}

bool ClientWorkManager::RemoveJob(const string &jobName)
{
	list<AbstractJob*>::iterator it=jobList.begin();
	list<AbstractJob*>::iterator end=jobList.end();
	list<AbstractJob*>::iterator lastFoundIt=jobList.begin();
	unsigned int jobCount = 0;
	for (; it!=end; it++)
	{
		if ((*it)->GetName() == jobName)
		{
			lastFoundIt = it;
			jobCount++;
		}
	}

	if (jobCount != 1)
		 return false;

	jobList.erase(lastFoundIt);
	return true;
}

bool ClientWorkManager::RemoveAllButJobs(const string &jobName)
{
	list<AbstractJob*>::iterator it=jobList.begin();
	unsigned int removedJobs = 0;
    while (it!=jobList.end())
	{
		if ((*it)->GetName() != jobName)
		{
			it = jobList.erase(it);
			removedJobs++;
		}
        else
            ++it;
	}

	return (removedJobs > 0);
}

WorkResultData *ClientWorkManager::RunWorkList()
{
	ClientJobResults* jobResults = new ClientJobResults();

	list<AbstractJob*>::iterator it=jobList.begin();
	list<AbstractJob*>::iterator end=jobList.end();
	for (; it!=end; it++)
	{
		AbstractJob* currentJob = *it;
        JobStatus* status;
        if (currentJob->IsInitialized())
            status = currentJob->Run();
        else
        {
            status = new JobStatus(JobStatus::NOT_EXECUTED, "Initialization failed");
            status->SetDuration(0);
        }

		jobResults->push_back(make_pair(currentJob->GetName(), status));
	}

	WorkResultData* resultData = new WorkResultData();
	resultData->AddClientData(client->GetName(), jobResults);
    return resultData;
}

void ClientWorkManager::GetJobList(list<AbstractJob *> &_jobs)
{
    copy(jobList.begin(), jobList.end(), back_inserter(_jobs));
}
