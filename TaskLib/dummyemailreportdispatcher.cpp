#include "dummyemailreportdispatcher.h"

std::string DummyEmailReportDispatcher::GetName() const
{
    return std::string("Dummy Email");
}

bool DummyEmailReportDispatcher::Dispatch(AbstractReportCreator *)
{
    return true;
}
