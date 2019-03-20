#include "abstractemailreportdispatcher.h"

#include <sstream>
#include "serverconfiguration.h"
#include "standaloneconfiguration.h"

using namespace std;

const wstring AbstractEmailReportDispatcher::subject = L"Maintenance Report";

AbstractEmailReportDispatcher::AbstractEmailReportDispatcher()
    : outputDebugInformation(false), isVerbose(false)
{
}

AbstractEmailReportDispatcher::~AbstractEmailReportDispatcher()
{
}

wstring AbstractEmailReportDispatcher::GetName() const
{
    return L"Email";
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
      cc = L"";
      bcc = L"";
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

wstring AbstractEmailReportDispatcher::GetSmtpUrl() const
{
	wstring smtpUrl(L"smtp://");
   smtpUrl += emailData.GetSmtpServer() + L":";
	wstringstream s;
   s << emailData.GetSmtpPort();
	smtpUrl += s.str();
	return smtpUrl;
}
