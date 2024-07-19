#include "workresultdata.h"

#include "filetools.h"

using namespace std;

WorkResultData::WorkResultData()
{
}

WorkResultData::~WorkResultData()
{
	vector<pair<wstring, ClientJobResults*> >::iterator it = allClientsResults.begin();
	vector<pair<wstring, ClientJobResults*> >::iterator end = allClientsResults.end();
	for (; it!=end; it++)
	{
		ClientJobResults* clientResult = it->second;
		vector<pair<wstring, JobStatus*> >::iterator itStat = clientResult->begin();
		vector<pair<wstring, JobStatus*> >::iterator endStat = clientResult->end();
		for (; itStat!=endStat; itStat++)
		{
			delete itStat->second;
		}
	}
}

void WorkResultData::AddClientData(const wstring& clientName, ClientJobResults *clientData)
{
    allClientsResults.push_back(make_pair(clientName, clientData));
}

size_t WorkResultData::GetClientCount() const
{
    return allClientsResults.size();
}

void WorkResultData::GetAttachmentContents(vector<wstring> &attachments) const
{
    vector<pair<wstring, ClientJobResults*> >::const_iterator itClients = allClientsResults.begin();
    for (; itClients!=allClientsResults.end(); ++itClients)
    {
        vector<pair<wstring, JobStatus*> >::const_iterator itJobs = itClients->second->begin();
        for (; itJobs != itClients->second->end(); ++itJobs)
        {
            JobStatus* status = itJobs->second;
            GetBufferAttachments(status, attachments);
            GetFileAttachments(status, attachments);
        }
    }
}

void WorkResultData::GetBufferAttachments(JobStatus *status, vector<wstring> &attachments) const
{
    vector<pair<wstring, wstring> > buffers;
    status->GetFileBuffers(buffers);
    vector<pair<wstring, wstring> >::const_iterator it=buffers.begin();
    for (; it != buffers.end(); ++it)
        attachments.push_back(it->second);
}

void WorkResultData::GetFileAttachments(JobStatus *status, vector<wstring> &attachments) const
{
    vector<wstring> filenames;
    status->GetExternalFilenames(filenames);
    vector<wstring>::const_iterator it=filenames.begin();
    for (; it != filenames.end(); ++it)
        attachments.push_back(FileTools::GetTextFileContent(*it));
}
