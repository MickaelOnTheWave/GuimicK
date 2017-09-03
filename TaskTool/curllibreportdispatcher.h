#ifndef CURLLIBREPORTDISPATCHER_H
#define CURLLIBREPORTDISPATCHER_H

#include "abstractemailreportdispatcher.h"

class CurlLibReportDispatcher : public AbstractEmailReportDispatcher
{
public:
    CurlLibReportDispatcher();

    virtual bool Dispatch(AbstractReportCreator* reportCreator);
};

#endif // CURLLIBREPORTDISPATCHER_H
