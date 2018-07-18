#ifndef EMAILDISPATCHERFACTORY_H
#define EMAILDISPATCHERFACTORY_H

#include "abstractemailreportdispatcher.h"

class EmailDispatcherFactory
{
public:
    static AbstractEmailReportDispatcher* Create();
};

#endif // EMAILDISPATCHERFACTORY_H
