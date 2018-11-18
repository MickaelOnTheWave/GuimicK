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
      const Agent* agent = standaloneConfiguration->GetAgent();
      displayName = agent->GetName();
      emailData = agent->GetEmailData();

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
   smtpUrl += emailData.GetSmtpServer() + ":";
	stringstream s;
   s << emailData.GetSmtpPort();
	smtpUrl += s.str();
	return smtpUrl;
}
