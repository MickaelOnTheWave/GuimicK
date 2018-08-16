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

typedef std::pair<std::string, ClientJobResults*> ClientResult;

class WorkResultData
{
public:
	WorkResultData();
	~WorkResultData();

	void AddClientData(const std::string &clientName, ClientJobResults* clientData);

    size_t GetClientCount(void) const;

    void GetAttachmentContents(std::vector<std::string>& attachments) const;

   std::vector<ClientResult> allClientsResults;

private:
    void GetBufferAttachments(JobStatus* status, std::vector<std::string>& attachments) const;
    void GetFileAttachments(JobStatus* status, std::vector<std::string>& attachments) const;
};

#endif // WORKRESULTDATA_H
