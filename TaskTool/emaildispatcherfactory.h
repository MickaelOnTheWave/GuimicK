#ifndef EMAILDISPATCHERFACTORY_H
#define EMAILDISPATCHERFACTORY_H

#include "emailreportdispatcher.h"

class EmailDispatcherFactory
{
public:

    static EmailReportDispatcher* Create();
};

#endif // EMAILDISPATCHERFACTORY_H
