#include "workresultdata.h"

using namespace std;

WorkResultData::WorkResultData()
{
}

WorkResultData::~WorkResultData()
{
	vector<pair<string, ClientJobResults*> >::iterator it = allClientsResults.begin();
	vector<pair<string, ClientJobResults*> >::iterator end = allClientsResults.end();
	for (; it!=end; it++)
	{
		ClientJobResults* clientResult = it->second;
		vector<pair<string, JobStatus*> >::iterator itStat = clientResult->begin();
		vector<pair<string, JobStatus*> >::iterator endStat = clientResult->end();
		for (; itStat!=endStat; itStat++)
		{
			delete itStat->second;
		}
	}
}

void WorkResultData::AddClientData(const std::string& clientName, ClientJobResults *clientData)
{
	allClientsResults.push_back(make_pair(clientName, clientData));
}
