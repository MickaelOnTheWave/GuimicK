#include "filereportdispatcher.h"

#include <stdio.h>
#include "filetools.h"

using namespace std;

// TODO : make this configurable. Maybe consider a .log default?
// or check if it is html or not?
static const string reportFolder = "report/";
static const string reportFile = "report.html";

FileReportDispatcher::FileReportDispatcher()
{
}

string FileReportDispatcher::GetName() const
{
    return string("File");
}

void FileReportDispatcher::Initialize(const ServerConfiguration *)
{
}

bool FileReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    if (FileTools::FolderExists(reportFolder))
        FileTools::RemoveFolder(reportFolder, false);
    FileTools::CreateFolder(reportFolder);

    bool ok = FileTools::WriteBufferToFile(reportFolder+reportFile,
                                           reportCreator->GetReportContent());
    if (ok)
        ok = WriteAttachments(reportCreator);

    return ok;
}

bool FileReportDispatcher::WriteAttachments(AbstractReportCreator *reportCreator)
{
    vector<string> files;
    vector<pair<string,string> > buffers;
    reportCreator->GetAssociatedFiles(files, buffers);

    bool allOk = true;
    vector<pair<string,string> >::const_iterator itBuffers = buffers.begin();
    for (; itBuffers!=buffers.end(); ++itBuffers)
    {
        if (!FileTools::WriteBufferToFile(reportFolder+itBuffers->first, itBuffers->second))
            allOk = false;
    }
    return allOk;
}
