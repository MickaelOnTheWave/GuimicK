#include "consolereportdispatcher.h"

#include <iostream>
#include "filetools.h"

using namespace std;

wstring ConsoleReportDispatcher::GetName() const
{
   return L"Console";
}

wstring ConsoleReportDispatcher::GetLastError() const
{
   return L"";
}

void ConsoleReportDispatcher::Initialize(const AbstractTypeConfiguration*)
{
}

bool ConsoleReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    wcout << reportCreator->GetReportContent() << endl;
    if (reportCreator->HasAttachments())
    {
       wcout << "Attachments:" << endl;
       ShowAttachments(reportCreator);
    }
    return true;
}

void ConsoleReportDispatcher::ShowAttachments(AbstractReportCreator *reportCreator)
{
   ReportFileData fileData;
   reportCreator->GetAssociatedFiles(fileData);

    vector<wstring>::const_iterator itFiles = fileData.externalFiles.begin();
    for (; itFiles != fileData.externalFiles.end(); ++itFiles)
        wcout << "\t" << *itFiles << " (see file)" << endl;

    vector<pair<wstring,wstring> >::const_iterator itBuffers = fileData.fileBuffers.begin();
    for (; itBuffers != fileData.fileBuffers.end(); ++itBuffers)
    {
        wcout << "\t" << itBuffers->first;
        const bool ok = FileTools::WriteBufferToFile(itBuffers->first,
                                                         itBuffers->second);
        if (ok == false)
           wcout << " (Error trying to create attachment)";
        wcout << endl;
    }
}
