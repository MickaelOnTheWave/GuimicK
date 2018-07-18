#ifndef CURLDISPATCHERREPLACER_H
#define CURLDISPATCHERREPLACER_H

#include "abstractreportdispatcherreplacer.h"

class CurlDispatcherReplacer : public AbstractReportDispatcherReplacer
{
public:
    AbstractReportDispatcher* Run(AbstractReportDispatcher* input,
                                  const ServerConfiguration& configuration);

};

#endif // CURLDISPATCHERREPLACER_H
