#ifndef CURLDISPATCHERREPLACER_H
#define CURLDISPATCHERREPLACER_H

#include "abstractreportdispatcherreplacer.h"

class CurlDispatcherReplacer : public AbstractReportDispatcherReplacer
{
public:
    AbstractReportDispatcher* Run(AbstractReportDispatcher* input,
                                  const Configuration& configuration);

};

#endif // CURLDISPATCHERREPLACER_H
