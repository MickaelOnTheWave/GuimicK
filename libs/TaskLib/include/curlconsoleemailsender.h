#ifndef CURLCONSOLEEMAILSENDER_H
#define CURLCONSOLEEMAILSENDER_H

#include "abstractemailreportdispatcher.h"
#include <string>
#include <vector>

class CurlConsoleReportDispatcher : public AbstractEmailReportDispatcher
{
public:
    CurlConsoleReportDispatcher();

    virtual bool Dispatch(AbstractReportCreator* reportCreator);

private:
    std::wstring GetCurlVersion() const;
    std::wstring BuildCurlParams(const std::wstring& mailFilename) const;
    void WriteReportContentToFile(AbstractReportCreator* reportCreator,
                                  const std::wstring& filename);
    std::string CreateEmailContent(AbstractReportCreator* reportCreator) const;

    EmailData CreateEmailData(AbstractReportCreator* reportCreator) const;
};

#endif // CURLCONSOLEEMAILSENDER_H
