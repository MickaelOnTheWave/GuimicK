#include "abstractreportcreator.h"

using namespace std;

const wstring emptyString = L"";

AbstractReportCreator::AbstractReportCreator()
    : fullReport(emptyString), reportCore(emptyString), dispatchErrors(emptyString),
      programVersion(emptyString), useProfiling(true)
{
}

AbstractReportCreator::AbstractReportCreator(const AbstractReportCreator& other)
   : fullReport(other.fullReport), reportCore(emptyString),
     dispatchErrors(emptyString),
     programVersion(emptyString),
     useProfiling(other.useProfiling)
{
}

void AbstractReportCreator::UseProfileColumn(const bool value)
{
    useProfiling = value;
}

void AbstractReportCreator::Generate(WorkResultData *data, const wstring &version)
{
    vector<wstring> noErrors;
    Generate(data, noErrors, version);
}

wstring AbstractReportCreator::GetReportContent() const
{
    return fullReport;
}

void AbstractReportCreator::GetAssociatedFiles(ReportFileData& fileData)
{
    fileData.clear();
    copy(externalFiles.begin(), externalFiles.end(), back_inserter(fileData.externalFiles));
    copy(fileBuffers.begin(), fileBuffers.end(), back_inserter(fileData.fileBuffers));
}

bool AbstractReportCreator::HasAttachments() const
{
   return (!externalFiles.empty() || !fileBuffers.empty());
}
