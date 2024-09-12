#include "abstractemailreportdispatcher.h"

#include <sstream>
#include "serverconfiguration.h"
#include "standaloneconfiguration.h"
#include "stringtools.h"

using namespace std;

const wstring AbstractEmailReportDispatcher::subject = L"Maintenance Report";

AbstractEmailReportDispatcher::AbstractEmailReportDispatcher()
    : outputDebugInformation(DebugOutput::NEVER),
      isVerbose(false),
      lastError(L"")
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

      outputDebugInformation = agent->GetOutputDebugInformation();
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

vector<string> AbstractEmailReportDispatcher::ToUtf8(const vector<wstring>& input) const
{
   vector<string> output;
   output.reserve(input.size());
   vector<wstring>::const_iterator it = input.begin();
   vector<wstring>::const_iterator end = input.end();
   for (; it != end; ++it)
      output.push_back(StringTools::UnicodeToUtf8(*it));
   return output;
}

vector<pair<string, string> > AbstractEmailReportDispatcher::ToUtf8(
   const vector<pair<wstring, wstring> >& input
) const
{
   vector<pair<string, string> > output;
   output.reserve(input.size());
   vector<pair<wstring, wstring> >::const_iterator it = input.begin();
   vector<pair<wstring, wstring> >::const_iterator end = input.end();
   for (; it != end; ++it)
   {
      const string first = StringTools::UnicodeToUtf8(it->first);
      const string second = StringTools::UnicodeToUtf8(it->second);
      output.push_back(make_pair(first, second));
   }
   return output;
}
