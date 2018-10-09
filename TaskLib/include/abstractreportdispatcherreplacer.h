#ifndef ABSTRACTREPORTDISPATCHERREPLACER_H
#define ABSTRACTREPORTDISPATCHERREPLACER_H

#include "AbstractReportDispatcher.h"
#include "standaloneconfiguration.h"

class AbstractReportDispatcherReplacer
{
public:
    virtual ~AbstractReportDispatcherReplacer() {}
    virtual AbstractReportDispatcher* Run(  AbstractReportDispatcher* input,
                                            const StandaloneConfiguration& configuration) = 0;

};

#endif // ABSTRACTREPORTDISPATCHERREPLACER_H
