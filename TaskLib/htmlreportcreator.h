#ifndef HTMLREPORTCREATOR_H
#define HTMLREPORTCREATOR_H

#include "abstractstructuredreportcreator.h"

#include "jobstatus.h"

class HtmlReportCreator : public AbstractStructuredReportCreator
{
public:
    HtmlReportCreator();
    virtual ~HtmlReportCreator();

    virtual void UpdateWithDispatchError(const std::string& failedDispatcher,
                                         const std::string& fallbackDispatcher);

    void SetCssFile(const std::string& _cssFile);    

private:
    virtual void AddHeader(void);
    virtual void AddClientData(const std::pair<std::string, ClientJobResults *> &clientData);
    virtual void AddJobData(const std::string& jobName, JobStatus* status);
    virtual void AddSummaryData(const int code, const time_t duration);
    virtual void AddConfigurationErrorsData(const std::vector<std::string>& errors);
    virtual void AddProgramData(const std::string& version);

    void AddJobData(const std::string& jobName, const std::string& jobDescription,
                    const std::string& jobStatusCode, const std::string& jobDuration);

    std::string cssFile;
};

#endif // HTMLREPORTCREATOR_H
