#ifndef HTMLREPORTCREATOR_H
#define HTMLREPORTCREATOR_H

#include "abstractreportcreator.h"

#include <sstream>

class HtmlReportCreator : public AbstractReportCreator
{
public:
    HtmlReportCreator();

    virtual ~HtmlReportCreator() {}

    virtual std::string Generate(WorkResultData* data, const std::string& version);

private:

    void AddJobData(const std::string& jobName, const std::string& jobDescription, const std::string& jobStatusCode, const std::string& jobDuration);

    std::stringstream report;
};

#endif // HTMLREPORTCREATOR_H
