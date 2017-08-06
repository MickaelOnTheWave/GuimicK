#include "emailreportdispatcher.h"

#include <sstream>

using namespace std;

const string EmailReportDispatcher::subject = "Maintenance Report";

EmailReportDispatcher::EmailReportDispatcher()
    : outputDebugInformation(false), isVerbose(false)
{
}

EmailReportDispatcher::~EmailReportDispatcher()
{
}

void EmailReportDispatcher::Initialize(SelfIdentity *self, const Configuration &configuration)
{
    displayName = self->name;
    emailAddress = self->email;
    smtpServer = self->emailSmtpServer;
    smtpPort = self->emailSmtpPort;
    password = self->emailPassword;
    useSsl = self->emailUseSsl;

    isHtml = configuration.IsReportHtml();
    destEmail = configuration.GetMasterEmail();
    cc = string("");
    bcc = string("");
}

void EmailReportDispatcher::SetOutputDebugInformationOnFailure(const bool value)
{
    outputDebugInformation = value;
}

void EmailReportDispatcher::SetVerboseMode()
{
    isVerbose = true;
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
