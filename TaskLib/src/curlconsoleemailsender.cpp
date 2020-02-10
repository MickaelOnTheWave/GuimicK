#include "curlconsoleemailsender.h"

#include <fstream>
#include <stdio.h>

#include "consolejob.h"
#include "filetools.h"
#include "jobdebuginformationmanager.h"
#include "mimetools.h"
#include "stringtools.h"

using namespace std;

vector<string> ToUtf8(const vector<wstring>& input)
{
   vector<string> output;
   output.reserve(input.size());
   vector<wstring>::const_iterator it = input.begin();
   vector<wstring>::const_iterator end = input.end();
   for (; it != end; ++it)
      output.push_back(StringTools::UnicodeToUtf8(*it));
   return output;
}

vector<pair<string, string> > ToUtf8(const vector<pair<wstring, wstring> >& input)
{
   vector<pair<string, string> > output;
   output.reserve(input.size());
   vector<pair<wstring, wstring> >::const_iterator it = input.begin();
   vector<pair<wstring, wstring> >::const_iterator end = input.end();
   for (; it != end; ++it)
   {
      const string first = StringTools::UnicodeToUtf8(it->first);
      const string second = StringTools::UnicodeToUtf8(it->second);
      output.push_back(make_pair(first, second));
   }
   return output;
}

static const wstring mailFileName = L"mailContents.txt";

// @TODO Make proper implementation without clear password
// @TODO Improve security handling SSL correctly without insecure option
CurlConsoleReportDispatcher::CurlConsoleReportDispatcher()
    : AbstractEmailReportDispatcher()
{

}

bool CurlConsoleReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    JobDebugInformationManager debugInfo(outputDebugInformation, L"EmailSend");

    WriteReportContentToFile(reportCreator, mailFileName);

    const wstring curlParams = BuildCurlParams(mailFileName);

    debugInfo.AddDataLine<wstring>(L"Params", curlParams);
    ConsoleJob curl(L"curl", curlParams);
    JobStatus* status = curl.Run();
    if (status->GetCode() == JobStatus::Ok)
        FileTools::RemoveFile(mailFileName);
    else
    {
        debugInfo.AddDataLine<wstring>(L"Command executable", curl.GetCommand());
        debugInfo.AddDataLine<int>(L"Return code", curl.GetCommandReturnCode());
        debugInfo.AddDataLine<wstring>(L"Output", curl.GetCommandOutput());
        debugInfo.AddDataLine<wstring>(L"Curl version", GetCurlVersion());
        debugInfo.WriteToFile();
    }


    return (status->GetCode() == JobStatus::Ok);
}

wstring CurlConsoleReportDispatcher::GetCurlVersion() const
{
    ConsoleJob command(L"curl", L"--version");
    command.RunWithoutStatus();
    return command.GetCommandOutput();
}

wstring CurlConsoleReportDispatcher::BuildCurlParams(const wstring &mailFilename) const
{
    wstring params;
    params += L" --url \"" + GetSmtpUrl() + L"\" --ssl-reqd ";
    params += L"--mail-from \"" + emailAccountData.GetAddress() + L"\" ";
    params += L"--mail-rcpt \"" + destEmail + L"\" ";
    params += L"--upload-file " + mailFilename + L" ";
    params += L"--user \"" + emailAccountData.GetAddress() + L":" + emailAccountData.GetPassword() + L"\" ";
    params += L"--insecure --show-error ";
    params += isVerbose ? L"--verbose" : L"--silent";
    return params;
}

void CurlConsoleReportDispatcher::WriteReportContentToFile(AbstractReportCreator *reportCreator,
                                                           const wstring &filename)
{
    const string emailContent = CreateEmailContent(reportCreator);

    ofstream mailFile;
    mailFile.open(StringTools::UnicodeToUtf8(filename).c_str());
    mailFile << emailContent;
    mailFile.close();
}

std::string CurlConsoleReportDispatcher::CreateEmailContent(AbstractReportCreator* reportCreator) const
{
   vector<wstring> externalFiles;
   vector<pair<wstring, wstring> > fileBuffers;
   reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);

   const EmailData emailData = CreateEmailData(reportCreator);
   MimeTools mimeCreator;

   const string emailContent = mimeCreator.CreateEmailContent(
      StringTools::UnicodeToUtf8(displayName),
      StringTools::UnicodeToUtf8(emailAccountData.GetAddress()),
      emailData,
      ToUtf8(externalFiles),
      ToUtf8(fileBuffers)
   );
   return emailContent;
}

EmailData CurlConsoleReportDispatcher::CreateEmailData(AbstractReportCreator* reportCreator) const
{
   EmailData data;
   data.SetTo(destEmail);
   data.SetCc(cc);
   data.SetBcc(bcc);
   data.SetSubject(subject);
   data.SetBody(reportCreator->GetReportContent());
   data.SetIsHtml(isHtml);
   return data;
}
