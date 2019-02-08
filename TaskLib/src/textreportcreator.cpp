#include "textreportcreator.h"

#include <tools.h>

using namespace std;

static const string overallString = "Overall";

TextReportCreator::TextReportCreator()
    : AbstractStructuredReportCreator()
{
}

TextReportCreator::TextReportCreator(const TextReportCreator& other)
   : AbstractStructuredReportCreator(other), nameCellSize(other.nameCellSize)
{
}

TextReportCreator::~TextReportCreator()
{
}

AbstractReportCreator* TextReportCreator::Copy() const
{
   return new TextReportCreator(*this);
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

void TextReportCreator::AddClientHeaderData(const pair<string, ClientJobResults*>& clientData)
{
    reportCore << Tools::Tabs(3) << clientData.first << endl;
    reportCore << endl;
    reportCore << endl;

    UpdateNameCellSize(clientData.second);
}

void TextReportCreator::AddJobData(const string &jobName, JobStatus *status)
{
    string stringOutput(status->GetDescription());

    const string spaces = Tools::Spaces(nameCellSize - static_cast<int>(jobName.size()));
    reportCore << "\t" << jobName << spaces << status->GetCodeDescription();
    if (useProfiling)
        reportCore << "\t" << Tools::FormatTimeString(status->GetDuration());
    reportCore << endl;

    if (stringOutput != "")
        reportCore << Tools::Tabs(2) << stringOutput << endl;
}

void TextReportCreator::AddSummaryData(const int code, const time_t duration)
{
    const string spaces = Tools::Spaces(nameCellSize - static_cast<int>(overallString.size()));
    reportCore << endl;
    reportCore << "\t" << overallString << spaces;
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
    nameCellSize = static_cast<int>(overallString.size());

    ClientJobResults::iterator itJob=data->begin();
    ClientJobResults::iterator endJob=data->end();
    for (; itJob!=endJob; itJob++)
    {
        pair<string, JobStatus*> jobData = *itJob;
        if (jobData.first.size() > nameCellSize)
            nameCellSize = static_cast<int>(jobData.first.size());
    }

    nameCellSize += 3;
}
