#include "workresultdata.h"

#include "filetools.h"

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

size_t WorkResultData::GetClientCount() const
{
    return allClientsResults.size();
}

void WorkResultData::GetAttachmentContents(vector<string> &attachments) const
{
    std::vector<std::pair<std::string, ClientJobResults*> >::const_iterator itClients = allClientsResults.begin();
    for (; itClients!=allClientsResults.end(); ++itClients)
    {
        std::vector<std::pair<std::string, JobStatus*> >::const_iterator itJobs = itClients->second->begin();
        for (; itJobs != itClients->second->end(); ++itJobs)
        {
            JobStatus* status = itJobs->second;
            GetBufferAttachments(status, attachments);
            GetFileAttachments(status, attachments);
        }
    }
}

void WorkResultData::GetBufferAttachments(JobStatus *status, std::vector<string> &attachments) const
{
    std::vector<std::pair<string, string> > buffers;
    status->GetFileBuffers(buffers);
    std::vector<std::pair<string, string> >::const_iterator it=buffers.begin();
    for (; it != buffers.end(); ++it)
        attachments.push_back(it->second);
}

void WorkResultData::GetFileAttachments(JobStatus *status, std::vector<string> &attachments) const
{
    std::vector<string> filenames;
    status->GetExternalFilenames(filenames);
    std::vector<string>::const_iterator it=filenames.begin();
    for (; it != filenames.end(); ++it)
        attachments.push_back(FileTools::GetTextFileContent(*it));
}
