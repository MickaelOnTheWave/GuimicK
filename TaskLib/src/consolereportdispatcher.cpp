#include "consolereportdispatcher.h"

#include <iostream>

using namespace std;

string ConsoleReportDispatcher::GetName() const
{
    return string("Console");
}

void ConsoleReportDispatcher::Initialize(const ServerConfiguration *)
{
}

bool ConsoleReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    cout << reportCreator->GetReportContent() << endl;
    cout << "Attachments:" << endl;
    ShowAttachments(reportCreator);
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
        cout << "\t" << itBuffers->first << endl;
}
