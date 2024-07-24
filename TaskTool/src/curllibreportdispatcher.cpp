#include "curllibreportdispatcher.h"

//#include "curllibemailsender.h"

using namespace std;

CurlLibReportDispatcher::CurlLibReportDispatcher()
{
}

bool CurlLibReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    /*CurlLibEmailSender sender;
    sender.SetSenderData(emailAccountData);

    ReportFileData fileData;
    reportCreator->GetAssociatedFiles(fileData);

    return sender.Send(CreateEmailData(reportCreator),
                       ToUtf8(fileData.externalFiles),
                       ToUtf8(fileData.fileBuffers));*/
   return false;
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
