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
    std::string GetCurlVersion() const;
    std::string BuildCurlParams(const std::string& mailFilename) const;
    void WriteReportContentToFile(AbstractReportCreator* reportCreator,
                                  const std::string& filename);
};

#endif // CURLCONSOLEEMAILSENDER_H
