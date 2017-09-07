#include "textreportcreator.h"

#include <tools.h>

using namespace std;

static const string overallString = "Overall";

TextReportCreator::TextReportCreator()
    : AbstractStructuredReportCreator()
{
}

TextReportCreator::~TextReportCreator()
{
}

void TextReportCreator::UpdateWithDispatchError(const std::string& failedDispatcher,
                                                const std::string& fallbackDispatcher)
{
    fullReport = "";

    dispatchErrors << failedDispatcher << " failed.";
    if (fallbackDispatcher != "")
        dispatchErrors << " Using " << fallbackDispatcher << ".";
    dispatchErrors << endl;

    fullReport = reportCore.str() + dispatchErrors.str() + programVersion.str();
}

void TextReportCreator::AddHeader()
{
}

void TextReportCreator::AddClientData(const pair<string, ClientJobResults*>& clientData)
{
    reportCore << Tools::Tabs(3) << clientData.first << endl;
    reportCore << endl;
    reportCore << endl;

    UpdateNameCellSize(clientData.second);
}

void TextReportCreator::AddJobData(const string &jobName, JobStatus *status)
{
    string stringOutput(status->GetDescription());

    reportCore << "\t" << jobName << Tools::Spaces(nameCellSize-jobName.size()) << status->GetCodeDescription();
    if (useProfiling)
        reportCore << "\t" << Tools::FormatTimeString(status->GetDuration());
    reportCore << endl;

    if (stringOutput != "")
        reportCore << Tools::Tabs(2) << stringOutput << endl;
}

void TextReportCreator::AddSummaryData(const int code, const time_t duration)
{
    reportCore << endl;
    reportCore << "\t" << overallString << Tools::Spaces(nameCellSize-overallString.size());
    reportCore << JobStatus::GetCodeDescription(code);
    if (useProfiling)
        reportCore << "\t" << Tools::FormatTimeString(duration) << endl;
    reportCore << endl;
}

void TextReportCreator::AddConfigurationErrorsData(const std::vector<string> &errors)
{
    if (errors.size() > 0)
    {
        reportCore << Tools::Tabs(1) << "Configuration file has some errors :" << endl;
        for (vector<string>::const_iterator it=errors.begin(); it!=errors.end(); ++it)
            reportCore << Tools::Tabs(2) << *it << endl;
        reportCore << endl;
    }
}

void TextReportCreator::AddProgramData(const string &version)
{
    programVersion << "Task Manager version " << version << endl;
}

void TextReportCreator::UpdateNameCellSize(ClientJobResults *data)
{
    nameCellSize = overallString.size();

    ClientJobResults::iterator itJob=data->begin();
    ClientJobResults::iterator endJob=data->end();
    for (; itJob!=endJob; itJob++)
    {
        pair<string, JobStatus*> jobData = *itJob;
        if (jobData.first.size() > nameCellSize)
            nameCellSize = jobData.first.size();
    }

    nameCellSize += 3;
}
