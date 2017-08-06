#include "curllibreportdispatcher.h"

#include "curllibemailsender.h"

using namespace std;

CurlLibReportDispatcher::CurlLibReportDispatcher()
{
}

bool CurlLibReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    CurlLibEmailSender sender;
    sender.SetSenderData(displayName, emailAddress, password, smtpServer, smtpPort, useSsl);

    vector<string> externalFiles;
    vector<pair<string,string> > fileBuffers;
    reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);
    const string body = reportCreator->GetReportContent();

    return sender.Send(isHtml, destEmail, cc, bcc, subject, body, externalFiles, fileBuffers);
}
