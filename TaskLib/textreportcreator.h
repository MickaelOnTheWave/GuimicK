#ifndef CONSOLEREPORT_H
#define CONSOLEREPORT_H

#include <ctime>
#include <string>

#include "abstractstructuredreportcreator.h"
#include "jobstatus.h"

class TextReportCreator : public AbstractStructuredReportCreator
{
public:
	TextReportCreator();
    virtual ~TextReportCreator();

protected:
    virtual void AddHeader(void);
    virtual void AddClientData(const std::pair<std::string, ClientJobResults *> &clientData);
    virtual void AddJobData(const std::string& jobName, JobStatus* status);
    virtual void AddSummaryData(const int code, const time_t duration);
    virtual void AddConfigurationErrorsData(const std::vector<std::string>& errors);
    virtual void AddProgramData(const std::string& version);

    void UpdateNameCellSize(ClientJobResults* data);

    unsigned int nameCellSize;
};

#endif // CONSOLEREPORT_H
