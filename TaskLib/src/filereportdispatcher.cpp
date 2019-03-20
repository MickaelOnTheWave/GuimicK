#include "filereportdispatcher.h"

#include <stdio.h>
#include "filetools.h"
#include "standaloneconfiguration.h"

using namespace std;

static const wstring defaultReportFolder = L"report/";
static const wstring defaultReportFile = L"report.log";

FileReportDispatcher::FileReportDispatcher()
   : reportFolder(defaultReportFolder),
     reportFile(defaultReportFile)
{
}

wstring FileReportDispatcher::GetName() const
{
    return L"File";
}

void FileReportDispatcher::Initialize(const AbstractTypeConfiguration* configuration)
{
   const StandaloneConfiguration* standaloneConfiguration =
         dynamic_cast<const StandaloneConfiguration*>(configuration);
   if (standaloneConfiguration)
   {
      const Agent* self = standaloneConfiguration->GetAgent();
      reportFolder = self->GetReportFolder();
      reportFile = self->GetReportFile();
   }
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

void FileReportDispatcher::SetFolderName(const wstring& value)
{
   reportFolder = value;
}

void FileReportDispatcher::SetFileName(const wstring& value)
{
   reportFile = value;
}

bool FileReportDispatcher::WriteAttachments(AbstractReportCreator *reportCreator)
{
    vector<wstring> files;
    vector<pair<wstring,wstring> > buffers;
    reportCreator->GetAssociatedFiles(files, buffers);

    bool allOk = true;
    vector<pair<wstring,wstring> >::const_iterator itBuffers = buffers.begin();
    for (; itBuffers!=buffers.end(); ++itBuffers)
    {
        if (!FileTools::WriteBufferToFile(reportFolder+itBuffers->first, itBuffers->second))
            allOk = false;
    }
    return allOk;
}
