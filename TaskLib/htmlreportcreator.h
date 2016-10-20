#ifndef HTMLREPORTCREATOR_H
#define HTMLREPORTCREATOR_H

#include "AbstractReport.h"

#include <sstream>

class HtmlReportCreator : public AbstractReportCreator
{
public:
    HtmlReportCreator();

    virtual ~HtmlReportCreator() {}

    virtual std::string Generate(WorkResultData* data, const std::string& version);
    virtual void GetAssociatedFiles(std::list<std::string>& fileList);

private:

    void AddJobData(const std::string& jobName, const std::string& jobDescription, const std::string& jobStatusCode, const std::string& jobDuration);

    std::string GetFileContents(const std::string& file);

    std::vector<std::string> jobsFilenames;
    std::stringstream report;
};

#endif // HTMLREPORTCREATOR_H
