#ifndef HTMLREPORTCREATOR_H
#define HTMLREPORTCREATOR_H

#include "abstractstructuredreportcreator.h"

#include "jobstatus.h"

class HtmlReportCreator : public AbstractStructuredReportCreator
{
public:
    HtmlReportCreator();
    HtmlReportCreator(const HtmlReportCreator& other);
    virtual ~HtmlReportCreator();

    virtual AbstractReportCreator* Copy() const;

    virtual void UpdateWithDispatchError(const std::wstring& failedDispatcher,
                                         const std::wstring& dispatchError,
                                         const std::wstring& fallbackDispatcher);

    void SetCssFile(const std::wstring& _cssFile);    

private:
    virtual void AddHeader(void);
    virtual void AddClientHeaderData(const std::pair<std::wstring, ClientJobResults *> &clientData);
    virtual void AddJobData(const std::wstring& jobName, JobStatus* status);
    virtual void AddSummaryData(const int code, const time_t duration);
    virtual void AddConfigurationErrorsData(const std::vector<std::wstring>& errors);
    virtual void AddProgramData(const std::wstring& version);

    void AddJobData(const std::wstring& jobName, const std::wstring& jobDescription,
                    const std::wstring& jobStatusCode, const std::wstring& jobDuration);

    std::wstring cssFile;
};

#endif // HTMLREPORTCREATOR_H
