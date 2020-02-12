#include "abstractemailreportdispatcher.h"

#include <sstream>
#include "serverconfiguration.h"
#include "standaloneconfiguration.h"

using namespace std;

const wstring AbstractEmailReportDispatcher::subject = L"Maintenance Report";

AbstractEmailReportDispatcher::AbstractEmailReportDispatcher()
    : outputDebugInformation(false), isVerbose(false),
      lastError(L"")
{
}

AbstractEmailReportDispatcher::~AbstractEmailReportDispatcher()
{
}

wstring AbstractEmailReportDispatcher::GetName() const
{
   return L"Email";
}

wstring AbstractEmailReportDispatcher::GetLastError() const
{
   return lastError;
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
      emailAccountData = agent->GetEmailData();

      outputDebugInformation = agent->ShouldOutputDebugInformation();
      isVerbose = agent->IsDispatcherVerbose();

      isHtml = standaloneConfiguration->IsReportHtml();
      destEmail = standaloneConfiguration->GetMasterEmail();
      cc = L"";
      bcc = L"";
   }
}

wstring AbstractEmailReportDispatcher::GetSmtpUrl() const
{
	wstring smtpUrl(L"smtp://");
   smtpUrl += emailAccountData.GetSmtpServer() + L":";
	wstringstream s;
   s << emailAccountData.GetSmtpPort();
	smtpUrl += s.str();
	return smtpUrl;
}
