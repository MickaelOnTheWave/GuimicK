#ifndef ABSTRACTEMAILREPORTDISPATCHER_H
#define ABSTRACTEMAILREPORTDISPATCHER_H

#include <string>
#include "AbstractReportDispatcher.h"

class ServerConfiguration;

class AbstractEmailReportDispatcher : public AbstractReportDispatcher
{
public:
    AbstractEmailReportDispatcher();
    virtual ~AbstractEmailReportDispatcher();

    virtual std::string GetName(void) const;
    virtual void Initialize(const AbstractTypeConfiguration* configuration);

    void SetOutputDebugInformationOnFailure(const bool value);
    void SetVerboseMode();

protected:
    std::string GetSmtpUrl() const;

	std::string displayName;
	std::string emailAddress;
	std::string password;
	std::string smtpServer;
	int smtpPort;
	bool useSsl;
    bool outputDebugInformation;
    bool isVerbose;

    bool isHtml;
    std::string destEmail;
    std::string cc;
    std::string bcc;
    static const std::string subject;
};

#endif // ABSTRACTEMAILREPORTDISPATCHER_H
