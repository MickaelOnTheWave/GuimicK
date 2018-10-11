#ifndef CURLDISPATCHERREPLACER_H
#define CURLDISPATCHERREPLACER_H

#include "abstractreportdispatcherreplacer.h"

class CurlDispatcherReplacer : public AbstractReportDispatcherReplacer
{
public:
    AbstractReportDispatcher* Run(AbstractReportDispatcher* input,
                                  const StandaloneConfiguration& configuration);

};

#endif // CURLDISPATCHERREPLACER_H
