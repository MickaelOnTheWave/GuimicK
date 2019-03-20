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
    virtual void Initialize(const AbstractTypeConfiguration* configuration);

    void SetOutputDebugInformationOnFailure(const bool value);
    void SetVerboseMode();

protected:
    std::wstring GetSmtpUrl() const;

	std::wstring displayName;
   EmailData emailData;
   bool outputDebugInformation;
   bool isVerbose;

   bool isHtml;
   std::wstring destEmail;
   std::wstring cc;
   std::wstring bcc;
   static const std::wstring subject;
};

#endif // ABSTRACTEMAILREPORTDISPATCHER_H
