#ifndef WORKRESULTDATA_H
#define WORKRESULTDATA_H

#include <utility>
#include <vector>

#include "jobstatus.h"

/*
	There is no map here because we DON'T want a map! A map handle unique keys
	and we don't want this type of container, entries can be "duplicated"...
  */
typedef std::vector<std::pair<std::string, JobStatus*> > ClientJobResults;

class WorkResultData
{
public:
	WorkResultData();
	~WorkResultData();

	void AddClientData(const std::string &clientName, ClientJobResults* clientData);

	std::vector<std::pair<std::string, ClientJobResults*> > allClientsResults;
};

#endif // WORKRESULTDATA_H
