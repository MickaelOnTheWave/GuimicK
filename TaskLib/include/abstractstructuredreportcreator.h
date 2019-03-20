#ifndef ABSTRACTSTRUCTUREDREPORTCREATOR_H
#define ABSTRACTSTRUCTUREDREPORTCREATOR_H

#include "abstractreportcreator.h"

class AbstractStructuredReportCreator : public AbstractReportCreator
{
public:
    AbstractStructuredReportCreator();
    AbstractStructuredReportCreator(const AbstractStructuredReportCreator& other);
    virtual ~AbstractStructuredReportCreator();

    virtual void Generate(WorkResultData* data,
                          const std::vector<std::wstring>& configErrors,
                          const std::wstring& version);

private:
    virtual void AddHeader(void) = 0;
    virtual void AddClientHeaderData(const ClientResult& clientData) = 0;
    virtual void AddJobData(const std::wstring& jobName, JobStatus* status) = 0;
    virtual void AddSummaryData(const int code, const time_t duration) = 0;
    virtual void AddConfigurationErrorsData(const std::vector<std::wstring>& errors) = 0;
    virtual void AddProgramData(const std::wstring& version) = 0;

    void AddJobResultData(const ClientResult& clientData);
    void UpdateAttachments(JobStatus* status);
    int GetUpdatedCode(const int inputCode, JobStatus* status) const;

    int globalCode;
    time_t globalDuration;

};

#endif // ABSTRACTSTRUCTUREDREPORTCREATOR_H
