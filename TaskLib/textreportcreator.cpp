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

void TextReportCreator::AddHeader()
{
}

void TextReportCreator::AddClientData(const pair<string, ClientJobResults*>& clientData)
{
    report << Tools::Tabs(3) << clientData.first << endl;
    report << endl;
    report << endl;

    UpdateNameCellSize(clientData.second);
}

void TextReportCreator::AddJobData(const string &jobName, JobStatus *status)
{
    string stringOutput(status->GetDescription());

    report << "\t" << jobName << Tools::Spaces(nameCellSize-jobName.size()) << status->GetCodeDescription();
    if (useProfiling)
        report << "\t" << Tools::FormatTimeString(status->GetDuration());
    report << endl;

    if (stringOutput != "")
        report << Tools::Tabs(2) << stringOutput << endl;
}

void TextReportCreator::AddSummaryData(const int code, const time_t duration)
{
    report << endl;
    report << "\t" << overallString << Tools::Spaces(nameCellSize-overallString.size());
    report << JobStatus::GetCodeDescription(code);
    if (useProfiling)
        report << "\t" << Tools::FormatTimeString(duration) << endl;
    report << endl;
}

void TextReportCreator::AddConfigurationErrorsData(const std::vector<string> &errors)
{
    if (errors.size() > 0)
    {
        report << Tools::Tabs(1) << "Configuration file has some errors :" << endl;
        for (vector<string>::const_iterator it=errors.begin(); it!=errors.end(); ++it)
            report << Tools::Tabs(2) << *it << endl;
        report << endl;
    }
}

void TextReportCreator::AddProgramData(const string &version)
{
    report << "Task Manager version " << version << endl;
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
