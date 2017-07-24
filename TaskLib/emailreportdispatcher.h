#ifndef EMAILREPORTDISPATCHER_H
#define EMAILREPORTDISPATCHER_H

#include <string>

// @TODO make this class implement ReportDispatcher interface and stick to it
class EmailReportDispatcher
{
public:
	EmailReportDispatcher();

	void SetSenderData(
		const std::string& _displayName,
		const std::string& _emailAddress,
		const std::string& _password,
		const std::string& _smtpServer,
		const int _smtpPort,
		const bool _useSsl
    );

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
};

#endif // EMAILREPORTDISPATCHER_H
