#ifndef CONSOLEREPORTDISPATCHER_H
#define CONSOLEREPORTDISPATCHER_H

#include "AbstractReportDispatcher.h"

class ServerConfiguration;

class ConsoleReportDispatcher : public AbstractReportDispatcher
{
public:
    virtual std::string GetName(void) const;
    virtual void Initialize(const AbstractTypeConfiguration* configuration);
    virtual bool Dispatch(AbstractReportCreator* reportCreator);

private:
    void ShowAttachments(AbstractReportCreator* reportCreator);
};

#endif // CONSOLEREPORTDISPATCHER_H
