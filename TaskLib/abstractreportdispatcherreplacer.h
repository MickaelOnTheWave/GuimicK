#ifndef ABSTRACTREPORTDISPATCHERREPLACER_H
#define ABSTRACTREPORTDISPATCHERREPLACER_H

#include "AbstractReportDispatcher.h"
#include "serverconfiguration.h"

class AbstractReportDispatcherReplacer
{
public:
    virtual ~AbstractReportDispatcherReplacer() {}
    virtual AbstractReportDispatcher* Run(  AbstractReportDispatcher* input,
                                            const ServerConfiguration& configuration) = 0;

};

#endif // ABSTRACTREPORTDISPATCHERREPLACER_H
