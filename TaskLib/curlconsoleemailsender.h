#ifndef CURLCONSOLEEMAILSENDER_H
#define CURLCONSOLEEMAILSENDER_H

#include "emailreportdispatcher.h"
#include <string>
#include <vector>

class CurlConsoleEmailSender : public EmailReportDispatcher
{
public:
    CurlConsoleEmailSender();

    bool Send(const bool isHtml,
            const std::string& destEmail,
            const std::string& cc,
            const std::string& bcc,
            const std::string &subject,
            const std::string &body,
            const std::vector<std::string> &fileList,
            const std::vector<std::pair<std::string,std::string> >& fileBuffers
    );

private:
    std::string GetCurlVersion() const;
};

#endif // CURLCONSOLEEMAILSENDER_H
