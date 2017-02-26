#ifndef HTMLREPORTCREATOR_H
#define HTMLREPORTCREATOR_H

#include "abstractreportcreator.h"

class HtmlReportCreator : public AbstractReportCreator
{
public:
    HtmlReportCreator();

    virtual ~HtmlReportCreator() {}

    void SetCssFile(const std::string& _cssFile);

    virtual void Generate(WorkResultData* data, const std::string& version);

private:

    void AddJobData(const std::string& jobName, const std::string& jobDescription, const std::string& jobStatusCode, const std::string& jobDuration);

    std::string cssFile;
};

#endif // HTMLREPORTCREATOR_H
