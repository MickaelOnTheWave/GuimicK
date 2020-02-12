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
    vector<wstring> files;
    vector<pair<wstring,wstring> > buffers;
    reportCreator->GetAssociatedFiles(files, buffers);

    vector<wstring>::const_iterator itFiles = files.begin();
    for (; itFiles != files.end(); ++itFiles)
        wcout << "\t" << *itFiles << " (see file)" << endl;

    vector<pair<wstring,wstring> >::const_iterator itBuffers = buffers.begin();
    for (; itBuffers != buffers.end(); ++itBuffers)
    {
        wcout << "\t" << itBuffers->first;
        const bool ok = FileTools::WriteBufferToFile(itBuffers->first,
                                                         itBuffers->second);
        if (ok == false)
           wcout << " (Error trying to create attachment)";
        wcout << endl;
    }
}
