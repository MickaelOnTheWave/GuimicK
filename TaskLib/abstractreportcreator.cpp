#include "abstractreportcreator.h"

using namespace std;

AbstractReportCreator::AbstractReportCreator()
    : reportContent(""), useProfiling(true)
{
}

void AbstractReportCreator::UseProfileColumn(const bool value)
{
    useProfiling = value;
}

string AbstractReportCreator::GetReportContent() const
{
    return reportContent;
}

void AbstractReportCreator::GetAssociatedFiles(vector<string> &_externalFiles,
                                               vector<pair<string,string> >& _fileBuffers)
{
    _externalFiles.clear();
    copy(externalFiles.begin(), externalFiles.end(), back_inserter(_externalFiles));
    _fileBuffers.clear();
    copy(fileBuffers.begin(), fileBuffers.end(), back_inserter(_fileBuffers));
}
