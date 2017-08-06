#ifndef CURLCONSOLEEMAILSENDER_H
#define CURLCONSOLEEMAILSENDER_H

#include "emailreportdispatcher.h"
#include <string>
#include <vector>

class CurlConsoleEmailSender : public EmailReportDispatcher
{
public:
    CurlConsoleEmailSender();

    bool Dispatch(AbstractReportCreator* reportCreator);

private:
    std::string GetCurlVersion() const;
};

#endif // CURLCONSOLEEMAILSENDER_H
