#include "curlconsoleemailsender.h"

#include <fstream>
#include <stdio.h>
#include <tools.h>
#include "consolejob.h"

using namespace std;

// @TODO Make proper implementation without clear password
// @TODO Improve security handling SSL correctly without insecure option
bool CurlConsoleEmailSender::Send(const bool isHtml, const std::string &destEmail, const std::string &cc, const std::string &bcc, const std::string &subject, const std::string &body,
                                             const std::vector<std::string>& fileList)
{
    const std::string mailFileName("mailContents.txt");

	ofstream mailFile;
	mailFile.open(mailFileName.c_str());
    mailFile << Tools::CreateMIMEEmailContent(isHtml, displayName, emailAddress, destEmail, cc, bcc, subject, body, fileList);
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
