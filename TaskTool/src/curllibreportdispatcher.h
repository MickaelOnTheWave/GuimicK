#ifndef CURLLIBREPORTDISPATCHER_H
#define CURLLIBREPORTDISPATCHER_H

#include "abstractemailreportdispatcher.h"
#include "emaildata.h"

class CurlLibReportDispatcher : public AbstractEmailReportDispatcher
{
public:
    CurlLibReportDispatcher();

    virtual bool Dispatch(AbstractReportCreator* reportCreator);

private:
    EmailData CreateEmailData(AbstractReportCreator *reportCreator) const;
};

#endif // CURLLIBREPORTDISPATCHER_H
