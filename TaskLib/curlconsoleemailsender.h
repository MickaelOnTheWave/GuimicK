#ifndef CURLCONSOLEEMAILSENDER_H
#define CURLCONSOLEEMAILSENDER_H

#include "emailreportdispatcher.h"
#include <list>
#include <string>

class CurlConsoleEmailSender : public EmailReportDispatcher
{
public:

    bool Send(const bool isHtml,
            const std::string& destEmail,
            const std::string& cc,
            const std::string& bcc,
            const std::string &subject,
            const std::string &body,
            const std::list<std::string> &fileList
    );

protected:

};

#endif // CURLCONSOLEEMAILSENDER_H
