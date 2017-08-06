#ifndef EMAILREPORTDISPATCHER_H
#define EMAILREPORTDISPATCHER_H

#include <string>
#include "ReportDispatcher.h"

class EmailReportDispatcher : public AbstractReportDispatcher
{
public:
	EmailReportDispatcher();
    virtual ~EmailReportDispatcher();

    virtual void Initialize(SelfIdentity* self, const Configuration& configuration);

    void SetOutputDebugInformationOnFailure(const bool value);
    void SetVerboseMode();

protected:
	std::string GetSmtpUrl();

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

#endif // EMAILREPORTDISPATCHER_H
