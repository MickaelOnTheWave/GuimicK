#include "abstractemailreportdispatcher.h"

#include <sstream>
#include "serverconfiguration.h"
#include "standaloneconfiguration.h"

using namespace std;

const string AbstractEmailReportDispatcher::subject = "Maintenance Report";

AbstractEmailReportDispatcher::AbstractEmailReportDispatcher()
    : outputDebugInformation(false), isVerbose(false)
{
}

AbstractEmailReportDispatcher::~AbstractEmailReportDispatcher()
{
}

string AbstractEmailReportDispatcher::GetName() const
{
    return string("Email");
}

void AbstractEmailReportDispatcher::Initialize(
   const AbstractTypeConfiguration* configuration
)
{
   const StandaloneConfiguration* standaloneConfiguration =
         dynamic_cast<const StandaloneConfiguration*>(configuration);
   if (standaloneConfiguration)
   {
      const SelfIdentity* self = standaloneConfiguration->GetAgent();
      displayName = self->name;
      emailAddress = self->email;
      smtpServer = self->emailSmtpServer;
      smtpPort = self->emailSmtpPort;
      password = self->emailPassword;
      useSsl = self->emailUseSsl;

      isHtml = standaloneConfiguration->IsReportHtml();
      destEmail = standaloneConfiguration->GetMasterEmail();
      cc = string("");
      bcc = string("");
   }
}

void AbstractEmailReportDispatcher::SetOutputDebugInformationOnFailure(const bool value)
{
    outputDebugInformation = value;
}

void AbstractEmailReportDispatcher::SetVerboseMode()
{
    isVerbose = true;
}

string AbstractEmailReportDispatcher::GetSmtpUrl() const
{
	string smtpUrl("smtp://");
	smtpUrl += smtpServer + ":";
	stringstream s;
	s << smtpPort;
	smtpUrl += s.str();
	return smtpUrl;
}
