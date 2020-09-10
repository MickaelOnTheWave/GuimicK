#include "curllibreportdispatcher.h"

#include "curllibemailsender.h"

using namespace std;

CurlLibReportDispatcher::CurlLibReportDispatcher()
{
}

bool CurlLibReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    CurlLibEmailSender sender;
    sender.SetSenderData(emailAccountData);

    vector<wstring> externalFiles;
    vector<pair<wstring,wstring> > fileBuffers;
    reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);

    return sender.Send(CreateEmailData(reportCreator),
                       ToUtf8(externalFiles),
                       ToUtf8(fileBuffers));
}

EmailData CurlLibReportDispatcher::CreateEmailData(AbstractReportCreator *reportCreator) const
{
   EmailData emailData;
   emailData.SetTo(destEmail);
   emailData.SetCc(cc);
   emailData.SetBcc(bcc);
   emailData.SetSubject(subject);
   emailData.SetBody(reportCreator->GetReportContent());
   emailData.SetIsHtml(isHtml);
   return emailData;
}
