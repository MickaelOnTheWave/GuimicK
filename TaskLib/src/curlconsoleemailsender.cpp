#include "curlconsoleemailsender.h"

#include <fstream>
#include <stdio.h>

#include "consolejob.h"
#include "filetools.h"
#include "jobdebuginformationmanager.h"
#include "mimetools.h"

using namespace std;

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
        _wremove(mailFileName.c_str());
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
    params += L"--mail-from \"" + emailData.GetAddress() + L"\" ";
    params += L"--mail-rcpt \"" + destEmail + L"\" ";
    params += L"--upload-file " + mailFilename + L" ";
    params += L"--user \"" + emailData.GetAddress() + L":" + emailData.GetPassword() + L"\" ";
    params += L"--insecure --show-error ";
    params += isVerbose ? L"--verbose" : L"--silent";
    return params;
}

void CurlConsoleReportDispatcher::WriteReportContentToFile(AbstractReportCreator *reportCreator,
                                                           const wstring &filename)
{
    vector<wstring> externalFiles;
    vector<pair<wstring,wstring> > fileBuffers;
    reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);

    MimeTools mimeCreator;

    wofstream mailFile;
    mailFile.open(filename.c_str());
    mailFile << mimeCreator.CreateEmailContent(isHtml, displayName, emailData.GetAddress(),
                                               destEmail, cc, bcc,
                                               subject, reportCreator->GetReportContent(),
                                               externalFiles, fileBuffers);
    mailFile.close();
}
