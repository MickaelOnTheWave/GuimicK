#include "dummyemailreportdispatcher.h"

std::wstring DummyEmailReportDispatcher::GetName() const
{
    return L"Dummy Email";
}

bool DummyEmailReportDispatcher::Dispatch(AbstractReportCreator *)
{
    return false;
}
