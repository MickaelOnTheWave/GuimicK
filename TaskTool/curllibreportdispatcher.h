#ifndef CURLLIBREPORTDISPATCHER_H
#define CURLLIBREPORTDISPATCHER_H

#include "emailreportdispatcher.h"

class CurlLibReportDispatcher : public EmailReportDispatcher
{
public:
    CurlLibReportDispatcher();

    virtual bool Dispatch(AbstractReportCreator* reportCreator);
};

#endif // CURLLIBREPORTDISPATCHER_H
