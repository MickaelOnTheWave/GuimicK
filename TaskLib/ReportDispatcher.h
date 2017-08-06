#ifndef REPORTDISPATCHER_H
#define REPORTDISPATCHER_H

#include "abstractreportcreator.h"
#include "configuration.h"
#include "SelfIdentity.h"

/**
  Interface for all classes that manages dispatching of
  maintenance report to user (email sending etc...).
  */
class AbstractReportDispatcher
{
public :
    virtual ~AbstractReportDispatcher() {}
    virtual void Initialize(SelfIdentity* self, const Configuration& configuration) = 0;
    virtual bool Dispatch(AbstractReportCreator* reportCreator) = 0;
};


#endif // REPORTDISPATCHER_H
