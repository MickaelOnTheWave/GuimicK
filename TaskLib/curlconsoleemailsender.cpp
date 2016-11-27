#include "curlconsoleemailsender.h"

#include <fstream>
#include <stdio.h>

#include <mimetools.h>
#include "consolejob.h"

using namespace std;

// @TODO Make proper implementation without clear password
// @TODO Improve security handling SSL correctly without insecure option
bool CurlConsoleEmailSender::Send(const bool isHtml,
                                  const string &destEmail, const string &cc, const string &bcc,
                                  const string &subject, const string &body,
                                  const vector<string>& fileList, const vector<pair<string,string> >& fileBuffers)
{
    const string mailFileName("mailContents.txt");

	ofstream mailFile;
	mailFile.open(mailFileName.c_str());
    mailFile << MimeTools::CreateEmailContent(isHtml, displayName, emailAddress, destEmail, cc, bcc, subject, body, fileList, fileBuffers);
	mailFile.close();

    string curlParams;
    curlParams += " --url \"" + GetSmtpUrl() + "\" --ssl-reqd ";
    curlParams += "--mail-from \"" + emailAddress + "\" ";
    curlParams += "--mail-rcpt \"" + destEmail + "\" ";
    curlParams += "--upload-file " + mailFileName + " ";
    curlParams += "--user \"" + emailAddress + ":" + password + "\" ";
    curlParams += "--insecure ";
    curlParams += "--silent --show-error";

    ConsoleJob curl("Curl", "curl", curlParams);
    curl.SetOutputTofile("EmailSent.txt");

    JobStatus* status = curl.Run();
    if (status->GetCode() == JobStatus::OK)
	{
		remove("EmailSent.txt");
		remove(mailFileName.c_str());
	}

    return (status->GetCode() == JobStatus::OK);
}
