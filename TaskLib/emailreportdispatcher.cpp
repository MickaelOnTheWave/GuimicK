#include "emailreportdispatcher.h"

#include <sstream>

using namespace std;

EmailReportDispatcher::EmailReportDispatcher()
{
}


void EmailReportDispatcher::SetSenderData(
		const string& _displayName,
		const string &_emailAddress,
		const string &_password,
		const string &_smtpServer,
		const int _smtpPort,
		const bool _useSsl
)
{
	displayName = _displayName;
	emailAddress = _emailAddress;
	smtpServer = _smtpServer;
	smtpPort = _smtpPort;
	password = _password;
	useSsl = _useSsl;
}


string EmailReportDispatcher::GetSmtpUrl()
{
	string smtpUrl("smtp://");
	smtpUrl += smtpServer + ":";
	stringstream s;
	s << smtpPort;
	smtpUrl += s.str();
	return smtpUrl;
}
