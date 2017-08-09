#include "curlconsoleemailsender.h"

#include <fstream>
#include <stdio.h>

#include "consolejob.h"
#include "filetools.h"
#include "jobdebuginformationmanager.h"
#include "mimetools.h"

using namespace std;

static const string mailFileName = "mailContents.txt";

// @TODO Make proper implementation without clear password
// @TODO Improve security handling SSL correctly without insecure option
CurlConsoleReportDispatcher::CurlConsoleReportDispatcher()
    : EmailReportDispatcher()
{

}

bool CurlConsoleReportDispatcher::Dispatch(AbstractReportCreator *reportCreator)
{
    JobDebugInformationManager debugInfo(outputDebugInformation, "EmailSend");

    WriteReportContentToFile(reportCreator, mailFileName);

    const string curlParams = BuildCurlParams(mailFileName);

    debugInfo.AddDataLine<string>("Params", curlParams);
    ConsoleJob curl("curl", curlParams);
    JobStatus* status = curl.Run();
    if (status->GetCode() == JobStatus::OK)
        remove(mailFileName.c_str());
    else
    {
        debugInfo.AddDataLine<string>("Command executable", curl.GetCommand());
        debugInfo.AddDataLine<int>("Return code", curl.GetCommandReturnCode());
        debugInfo.AddDataLine<string>("Output", curl.GetCommandOutput());
        debugInfo.AddDataLine<string>("Curl version", GetCurlVersion());
        debugInfo.WriteToFile();
    }


    return (status->GetCode() == JobStatus::OK);
}

string CurlConsoleReportDispatcher::GetCurlVersion() const
{
    ConsoleJob command("curl", "--version");
    command.RunWithoutStatus();
    return command.GetCommandOutput();
}

string CurlConsoleReportDispatcher::BuildCurlParams(const string &mailFilename) const
{
    string params;
    params += " --url \"" + GetSmtpUrl() + "\" --ssl-reqd ";
    params += "--mail-from \"" + emailAddress + "\" ";
    params += "--mail-rcpt \"" + destEmail + "\" ";
    params += "--upload-file " + mailFilename + " ";
    params += "--user \"" + emailAddress + ":" + password + "\" ";
    params += "--insecure --show-error ";
    params += isVerbose ? "--verbose" : "--silent";
    return params;
}

void CurlConsoleReportDispatcher::WriteReportContentToFile(AbstractReportCreator *reportCreator,
                                                           const string &filename)
{
    vector<string> externalFiles;
    vector<pair<string,string> > fileBuffers;
    reportCreator->GetAssociatedFiles(externalFiles, fileBuffers);

    MimeTools mimeCreator;

    ofstream mailFile;
    mailFile.open(filename.c_str());
    mailFile << mimeCreator.CreateEmailContent(isHtml, displayName, emailAddress,
                                               destEmail, cc, bcc,
                                               subject, reportCreator->GetReportContent(),
                                               externalFiles, fileBuffers);
    mailFile.close();
}
