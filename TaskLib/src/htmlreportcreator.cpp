#include "htmlreportcreator.h"

#include <tools.h>
#include <filetools.h>

using namespace std;

HtmlReportCreator::HtmlReportCreator()
    : AbstractStructuredReportCreator(), cssFile("")
{
}

HtmlReportCreator::HtmlReportCreator(const HtmlReportCreator& other)
   : AbstractStructuredReportCreator(other), cssFile(other.cssFile)
{
}

HtmlReportCreator::~HtmlReportCreator()
{
}

AbstractReportCreator* HtmlReportCreator::Copy() const
{
   return new HtmlReportCreator(*this);
}

void HtmlReportCreator::UpdateWithDispatchError(const string &failedDispatcher,
                                                const string &fallbackDispatcher)
{
    fullReport = "";

    dispatchErrors << "<tr><td>" << failedDispatcher << " dispatch failed.";
    if (fallbackDispatcher != "")
        dispatchErrors << " Using " << fallbackDispatcher << " dispatch.";
    dispatchErrors << "</tr></td>" << endl;

    stringstream dispatchTable;
    dispatchTable << "<table><tr><th>Dispatching errors</th></tr>";
    dispatchTable << dispatchErrors.str();
    dispatchTable << "</table>";

    fullReport = reportCore.str() + dispatchTable.str() + programVersion.str();
}

void HtmlReportCreator::SetCssFile(const string &_cssFile)
{
    cssFile = _cssFile;
}

void HtmlReportCreator::AddHeader()
{
    reportCore << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << endl;
    reportCore << "<html>" << endl;
    reportCore << "  <head>" << endl;
    if (cssFile != "")
    {
        reportCore << "    <META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << endl;
        reportCore << FileTools::GetTextFileContent(cssFile) << endl;
    }
    reportCore << "  </head>" << endl;
    reportCore << "<body>" << endl;
}

void HtmlReportCreator::AddClientHeaderData(const pair<string, ClientJobResults *>& clientData)
{
    reportCore << "<h1>" << clientData.first << "</h1>" << endl;
    reportCore << "<table>" << endl;

    reportCore << "  <tr>" << endl;
    reportCore << "    <th>Task</th>" << endl;
    reportCore << "    <th>Status</th>" << endl;
    if (useProfiling)
        reportCore << "    <th>Time spent</th>" << endl;
    reportCore << "  </tr>" << endl;
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
    reportCore << "</table>" << endl;
}

void HtmlReportCreator::AddConfigurationErrorsData(const std::vector<string> &errors)
{
    if (errors.size() == 0)
        return;

    reportCore << "<table>" << endl;
    reportCore << "<tr><th>Configuration file errors</th></tr>" << endl;
    vector<string>::const_iterator it = errors.begin();
    for (; it != errors.end(); ++it)
        reportCore << "<tr><td>" << *it << "</td></tr>" << endl;
    reportCore << "</table>" << endl;
}

void HtmlReportCreator::AddProgramData(const std::string& version)
{
    programVersion << "<small>Task Manager version " << version << "</small>" << endl;
    programVersion << "</body>" << endl;
    programVersion << "</html>" << endl;
}

void HtmlReportCreator::AddJobData(const string &jobName, const string &jobDescription,
                                   const string &jobStatusCode, const string &jobDuration)
{
    reportCore << "  <tr>" << endl;
    reportCore << "    <td>" << endl;
    reportCore << "      <div class=name>" << jobName << "</div>" << endl;
    reportCore << "      <div class=comment>" << jobDescription << "</div>" << endl;
    reportCore << "    </td>" << endl;
    reportCore << "    <td>" << jobStatusCode << "</td>" << endl;
    if (useProfiling)
        reportCore << "    <td>" << jobDuration << "</td>" << endl;
    reportCore << "  </tr>" << endl;
}
