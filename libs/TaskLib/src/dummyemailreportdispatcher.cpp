#include "dummyemailreportdispatcher.h"

std::wstring DummyEmailReportDispatcher::GetName() const
{
   return L"Dummy Email";
}

std::wstring DummyEmailReportDispatcher::GetLastError() const
{
   return L"";
}

bool DummyEmailReportDispatcher::Dispatch(AbstractReportCreator *)
{
    return false;
}
