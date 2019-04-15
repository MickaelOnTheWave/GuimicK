#include "textreportcreator.h"

#include "stringtools.h"

using namespace std;

static const wstring overallString = L"Overall";

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

void TextReportCreator::UpdateWithDispatchError(const std::wstring& failedDispatcher,
                                                const std::wstring& fallbackDispatcher)
{
    fullReport = L"";

    dispatchErrors << failedDispatcher << " failed.";
    if (fallbackDispatcher != L"")
        dispatchErrors << " Using " << fallbackDispatcher << ".";
    dispatchErrors << endl;

    fullReport = reportCore.str() + dispatchErrors.str() + programVersion.str();
}

void TextReportCreator::AddHeader()
{
}

void TextReportCreator::AddClientHeaderData(const pair<wstring, ClientJobResults*>& clientData)
{
    reportCore << StringTools::Tabs(3) << clientData.first << endl;
    reportCore << endl;
    reportCore << endl;

    UpdateNameCellSize(clientData.second);
}

void TextReportCreator::AddJobData(const wstring &jobName, JobStatus *status)
{
    wstring wstringOutput(status->GetDescription());

    const wstring spaces = StringTools::Spaces(nameCellSize - static_cast<int>(jobName.size()));
    reportCore << "\t" << jobName << spaces << status->GetCodeDescription();
    if (useProfiling)
        reportCore << "\t" << StringTools::FormatTime(status->GetDuration());
    reportCore << endl;

    if (wstringOutput != L"")
        reportCore << StringTools::Tabs(2) << wstringOutput << endl;
}

void TextReportCreator::AddSummaryData(const int code, const time_t duration)
{
    const wstring spaces = StringTools::Spaces(nameCellSize - static_cast<int>(overallString.size()));
    reportCore << endl;
    reportCore << "\t" << overallString << spaces;
    reportCore << JobStatus::GetCodeDescription(code);
    if (useProfiling)
        reportCore << "\t" << StringTools::FormatTime(duration) << endl;
    reportCore << endl;
}

void TextReportCreator::AddConfigurationErrorsData(const std::vector<wstring> &errors)
{
    if (errors.size() > 0)
    {
        reportCore << StringTools::Tabs(1) << "Configuration file has some errors :" << endl;
        for (vector<wstring>::const_iterator it=errors.begin(); it!=errors.end(); ++it)
            reportCore << StringTools::Tabs(2) << *it << endl;
        reportCore << endl;
    }
}

void TextReportCreator::AddProgramData(const wstring &version)
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
        pair<wstring, JobStatus*> jobData = *itJob;
        if (jobData.first.size() > nameCellSize)
            nameCellSize = static_cast<int>(jobData.first.size());
    }

    nameCellSize += 3;
}
