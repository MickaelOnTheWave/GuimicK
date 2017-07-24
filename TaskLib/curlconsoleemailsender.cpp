#include "curlconsoleemailsender.h"

#include <fstream>
#include <stdio.h>

#include "consolejob.h"
#include "filetools.h"
#include "jobdebuginformationmanager.h"
#include "mimetools.h"

using namespace std;

// @TODO Make proper implementation without clear password
// @TODO Improve security handling SSL correctly without insecure option
CurlConsoleEmailSender::CurlConsoleEmailSender()
    : EmailReportDispatcher()
{

}

bool CurlConsoleEmailSender::Send(const bool isHtml,
                                  const string &destEmail, const string &cc, const string &bcc,
                                  const string &subject, const string &body,
                                  const vector<string>& fileList, const vector<pair<string,string> >& fileBuffers)
{
    JobDebugInformationManager debugInfo("EmailSending", outputDebugInformation);
    const string mailFileName("mailContents.txt");

    MimeTools mimeCreator;

	ofstream mailFile;
	mailFile.open(mailFileName.c_str());
    mailFile << mimeCreator.CreateEmailContent(isHtml, displayName, emailAddress, destEmail, cc, bcc, subject, body, fileList, fileBuffers);
	mailFile.close();

    string curlParams;
    curlParams += " --url \"" + GetSmtpUrl() + "\" --ssl-reqd ";
    curlParams += "--mail-from \"" + emailAddress + "\" ";
    curlParams += "--mail-rcpt \"" + destEmail + "\" ";
    curlParams += "--upload-file " + mailFileName + " ";
    curlParams += "--user \"" + emailAddress + ":" + password + "\" ";
    curlParams += "--insecure --show-error ";
    curlParams += isVerbose ? "--verbose" : "--silent";

    debugInfo.AddDataLine<string>("Params", curlParams);
    ConsoleJob curl("curl", curlParams);
    JobStatus* status = curl.Run();
    if (status->GetCode() != JobStatus::OK)
    {
        debugInfo.AddDataLine<string>("Command executable", curl.GetCommand());
        debugInfo.AddDataLine<int>("Return code", curl.GetCommandReturnCode());
        debugInfo.AddDataLine<string>("Output", curl.GetCommandOutput());
        debugInfo.AddDataLine<string>("Curl version", GetCurlVersion());
        debugInfo.WriteToFile();
    }
    else
        remove(mailFileName.c_str());

    return (status->GetCode() == JobStatus::OK);
}

string CurlConsoleEmailSender::GetCurlVersion() const
{
    ConsoleJob command("curl", "--version");
    command.RunWithoutStatus();
    return command.GetCommandOutput();
}
