#ifndef ABSTRACTSTRUCTUREDREPORTCREATOR_H
#define ABSTRACTSTRUCTUREDREPORTCREATOR_H

#include "abstractreportcreator.h"

class AbstractStructuredReportCreator : public AbstractReportCreator
{
public:
    AbstractStructuredReportCreator();
    virtual ~AbstractStructuredReportCreator();

    virtual void Generate(WorkResultData* data,
                          const std::vector<std::string>& configErrors,
                          const std::string& version);

private:
    virtual void AddHeader(void) = 0;
    virtual void AddClientData(const std::pair<std::string, ClientJobResults*>& clientData) = 0;
    virtual void AddJobData(const std::string& jobName, JobStatus* status) = 0;
    virtual void AddSummaryData(const int code, const time_t duration) = 0;
    virtual void AddConfigurationErrorsData(const std::vector<std::string>& errors) = 0;
    virtual void AddProgramData(const std::string& version) = 0;

    void UpdateAttachments(JobStatus* status);
    int GetUpdatedCode(const int inputCode, JobStatus* status);

};

#endif // ABSTRACTSTRUCTUREDREPORTCREATOR_H
