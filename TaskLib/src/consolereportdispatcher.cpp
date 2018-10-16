#include "consolereportdispatcher.h"

#include <iostream>
#include "filetools.h"

using namespace std;

string ConsoleReportDispatcher::GetName() const
{
    return string("Console");
}

void ConsoleReportDispatcher::Initialize(const AbstractTypeConfiguration*)
{
}

bool ConsoleReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    cout << reportCreator->GetReportContent() << endl;
    if (reportCreator->HasAttachments())
    {
       cout << "Attachments:" << endl;
       ShowAttachments(reportCreator);
    }
    return true;
}

void ConsoleReportDispatcher::ShowAttachments(AbstractReportCreator *reportCreator)
{
    vector<string> files;
    vector<pair<string,string> > buffers;
    reportCreator->GetAssociatedFiles(files, buffers);

    vector<string>::const_iterator itFiles = files.begin();
    for (; itFiles != files.end(); ++itFiles)
        cout << "\t" << *itFiles << " (see file)" << endl;

    vector<pair<string,string> >::const_iterator itBuffers = buffers.begin();
    for (; itBuffers != buffers.end(); ++itBuffers)
    {
        cout << "\t" << itBuffers->first;
        const bool ok = FileTools::WriteBufferToFile(itBuffers->first,
                                                         itBuffers->second);
        if (ok == false)
           cout << " (Error trying to create attachment)";
        cout << endl;
    }
}
