#ifndef ABSTRACTEMAILREPORTDISPATCHER_H
#define ABSTRACTEMAILREPORTDISPATCHER_H

#include <string>

#include "AbstractReportDispatcher.h"
#include "emaildata.h"

class ServerConfiguration;

class AbstractEmailReportDispatcher : public AbstractReportDispatcher
{
public:
    AbstractEmailReportDispatcher();
    virtual ~AbstractEmailReportDispatcher();

    virtual std::wstring GetName(void) const;
    virtual std::wstring GetLastError(void) const;
    virtual void Initialize(const AbstractTypeConfiguration* configuration);

protected:
   std::wstring GetSmtpUrl() const;

   std::wstring displayName;
   EmailAccountData emailAccountData;
   bool outputDebugInformation;
   bool isVerbose;

   bool isHtml;
   std::wstring destEmail;
   std::wstring cc;
   std::wstring bcc;
   std::wstring lastError;

   static const std::wstring subject;
};

#endif // ABSTRACTEMAILREPORTDISPATCHER_H
