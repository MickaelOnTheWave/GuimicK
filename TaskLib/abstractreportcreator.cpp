#include "abstractreportcreator.h"

using namespace std;

void AbstractReportCreator::GetAssociatedFiles(vector<string> &_externalFiles,
                                               vector<pair<string,string> >& _fileBuffers)
{
    _externalFiles.clear();
    copy(externalFiles.begin(), externalFiles.end(), back_inserter(_externalFiles));
    _fileBuffers.clear();
    copy(fileBuffers.begin(), fileBuffers.end(), back_inserter(_fileBuffers));
}
