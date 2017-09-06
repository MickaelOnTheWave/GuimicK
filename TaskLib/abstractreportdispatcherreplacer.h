#ifndef ABSTRACTREPORTDISPATCHERREPLACER_H
#define ABSTRACTREPORTDISPATCHERREPLACER_H

#include "AbstractReportDispatcher.h"
#include "configuration.h"

class AbstractReportDispatcherReplacer
{
public:
    virtual ~AbstractReportDispatcherReplacer() {}
    virtual AbstractReportDispatcher* Run(  AbstractReportDispatcher* input,
                                            const Configuration& configuration) = 0;

};

#endif // ABSTRACTREPORTDISPATCHERREPLACER_H
