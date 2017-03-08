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

    string curlCommand("curl ");
    curlCommand += " --url \"" + GetSmtpUrl() + "\" --ssl-reqd ";
    curlCommand += "--mail-from \"" + emailAddress + "\" ";
    curlCommand += "--mail-rcpt \"" + destEmail + "\" ";
    curlCommand += "--upload-file " + mailFileName + " ";
    curlCommand += "--user \"" + emailAddress + ":" + password + "\" ";
    curlCommand += "--insecure ";
    curlCommand += "--silent --show-error";

    ConsoleJob curl("Curl", curlCommand);
    curl.SetOutputTofile("EmailSent.txt");

    JobStatus* status = curl.Run();
    if (status->GetCode() == JobStatus::OK)
	{
		remove("EmailSent.txt");
		remove(mailFileName.c_str());
	}

    return (status->GetCode() == JobStatus::OK);
}
