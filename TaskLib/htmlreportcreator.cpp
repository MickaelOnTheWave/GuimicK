#include "htmlreportcreator.h"

#include <tools.h>
#include <filetools.h>

using namespace std;

HtmlReportCreator::HtmlReportCreator()
    : AbstractStructuredReportCreator(), cssFile("")
{
}

HtmlReportCreator::~HtmlReportCreator()
{
}

void HtmlReportCreator::SetCssFile(const string &_cssFile)
{
    cssFile = _cssFile;
}

void HtmlReportCreator::AddHeader()
{
    report << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << endl;
    report << "<html>" << endl;
    report << "  <head>" << endl;
    if (cssFile != "")
    {
        report << "    <META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << endl;
        report << FileTools::GetTextFileContent(cssFile) << endl;
    }
    report << "  </head>" << endl;
    report << "<body>" << endl;
}

void HtmlReportCreator::AddClientData(const pair<string, ClientJobResults *>& clientData)
{
    report << "<h1>" << clientData.first << "</h1>" << endl;
    report << "<table>" << endl;

    report << "  <tr>" << endl;
    report << "    <th>Task</th>" << endl;
    report << "    <th>Status</th>" << endl;
    if (useProfiling)
        report << "    <th>Time spent</th>" << endl;
    report << "  </tr>" << endl;
}

void HtmlReportCreator::AddJobData(const string &jobName, JobStatus* status)
{
    AddJobData(jobName, status->GetDescription(), status->GetCodeDescription(),
               Tools::FormatTimeString(status->GetDuration()));
}

void HtmlReportCreator::AddSummaryData(const int code, const time_t duration)
{
    AddJobData("Total", "", JobStatus::GetCodeDescription(code),
               Tools::FormatTimeString(duration));
    report << "</table>" << endl;
}

void HtmlReportCreator::AddConfigurationErrorsData(const std::vector<string> &errors)
{
    // TODO : fill this
}

void HtmlReportCreator::AddProgramData(const std::string& version)
{
    report << "<small>Task Manager version " << version << "</small>" << endl;
    report << "</body>" << endl;
    report << "</html>" << endl;
}

void HtmlReportCreator::AddJobData(const string &jobName, const string &jobDescription,
                                   const string &jobStatusCode, const string &jobDuration)
{
    report << "  <tr>" << endl;
    report << "    <td>" << endl;
    report << "      <div class=name>" << jobName << "</div>" << endl;
    report << "      <div class=comment>" << jobDescription << "</div>" << endl;
    report << "    </td>" << endl;
    report << "    <td>" << jobStatusCode << "</td>" << endl;
    if (useProfiling)
        report << "    <td>" << jobDuration << "</td>" << endl;
    report << "  </tr>" << endl;
}
