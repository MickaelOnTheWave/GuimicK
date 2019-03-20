#ifndef DUMMYEMAILREPORTDISPATCHER_H
#define DUMMYEMAILREPORTDISPATCHER_H

#include "abstractemailreportdispatcher.h"

class DummyEmailReportDispatcher : public AbstractEmailReportDispatcher
{
public:
    virtual std::wstring GetName() const;
    virtual bool Dispatch(AbstractReportCreator* reportCreator);
};

#endif // DUMMYEMAILREPORTDISPATCHER_H
