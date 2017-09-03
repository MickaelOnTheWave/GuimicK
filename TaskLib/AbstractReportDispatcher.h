#ifndef REPORTDISPATCHER_H
#define REPORTDISPATCHER_H

#include "abstractreportcreator.h"
#include "SelfIdentity.h"

class Configuration;

/**
  Interface for all classes that manages dispatching of
  maintenance report to user (email sending etc...).
  */
class AbstractReportDispatcher
{
public :
    virtual ~AbstractReportDispatcher() {}
    virtual std::string GetName(void) const = 0;
    virtual void Initialize(const Configuration* configuration) = 0;
    virtual bool Dispatch(AbstractReportCreator* reportCreator) = 0;
};


#endif // REPORTDISPATCHER_H
